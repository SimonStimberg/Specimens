#include "organisms.h"
#include "ofApp.h"

//------------------------------------------------------------------
Intestine::Intestine()
{
    systemPtr = NULL;
}

Intestine::Intestine(molecularSystem *system)
{

    Intestine();
    this->systemPtr = system;
}

void Intestine::set(int num, int x, int y)
{

    nextGrowth = ofGetElapsedTimeMillis() + 500 + (guiPtr->intestineGrowInterval) + (int)(ofRandom(guiPtr->intestineGrowInterval*0.5));
    mature = false;
    isDead = false;

    isDigesting = false;
    nextDigestion = ofGetElapsedTimeMillis() + 2000 + (guiPtr->intestineDigestionInterval) + (int)(ofRandom(guiPtr->intestineDigestionInterval*0.5));
    digestionPos = glm::vec2(0, 0);
    maxElements = 80;

    freqDivergence = ofRandom(20.);


    for (int i = 0; i < num; i++)
    {

        float initThickness = 10.;

        
        Molecule *mA = new Molecule(systemPtr, this);
        mA->reset(x + i * initThickness, y - initThickness, 0., 0.);

        Molecule *mB = new Molecule(systemPtr, this);
        mB->reset(x + i * initThickness, y + initThickness, 0., 0.);

        intestineMolecules.push_back(mA);
        intestineMolecules.push_back(mB);
        systemPtr->allMolecules.push_back(mA);
        systemPtr->allMolecules.push_back(mB);


        Spring *s = new Spring(systemPtr, STRUCTURE);
        s->connect(mA, mB);
        springs.push_back(s);
        systemPtr->allSprings.push_back(s);

        if (i > 0) {

            Spring *sA = new Spring(systemPtr, MEMBRANE);
            Molecule *mA0 = intestineMolecules[ intestineMolecules.size() - 4 ];
            sA->connect(mA, mA0);
            springs.push_back(sA);
            systemPtr->allSprings.push_back(sA);

            Spring *sB = new Spring(systemPtr, MEMBRANE);
            Molecule *mB0 = intestineMolecules[ intestineMolecules.size() - 3 ];
            sB->connect(mB0, mB);
            springs.push_back(sB);
            systemPtr->allSprings.push_back(sB);

        }

    }

}


void Intestine::linkAudioModule(audioModule::Intestine & module)
{

    audioModule = &module;
    audioModule->blockModule();

    // ofLogNotice("linking audio module to intestine");


    // INITIALIZE AUDIO MODULE

    // audioModule->init();

    float choosePitch[3] = {0, 7, 12};
    pdsp::f2p(55) >> audioModule->in_pitch();
    soundCtrl >> audioModule->in_trig();
    pulseRate >> audioModule->in_LFOrate();
    pulseRate.enableSmoothing(50.0f);
    // soundCtrl.trigger(1.0);

    soundCtrl.off();



}


//------------------------------------------------------------------
void Intestine::update()
{

    // adaptArousal(-0.0015 * intestineMolecules.size());
    arousal = 0.7;
    adaptValence();

    if(mature && !isDigesting && !systemPtr->doNotDigest && arousal > 0.999) nextDigestion = ofGetElapsedTimeMillis();

    grow();
    updatePosition();
    if(mature && !isDigesting && !systemPtr->doNotDigest) getSynced();
    digest();

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->update();
    }

    keepShape();

    for (unsigned int i = 0; i < intestineMolecules.size(); i++)
    {
        intestineMolecules[i]->update();
    }

    if (intestineMolecules.size() >= maxElements) die();


    // if very aroused, mess up syncing by detuning the frequency towards its initial untuned value
    float rate = arousal * arousal;   // use a squared curve for mapping
    rate = ofMap(rate, 0.75, 1., 0., 15., true);     // the detune amount depends on the arousal level. detune above 0.85 accordingly
    rate >> audioModule->in_cutoff();


    // float frequency = ofLerp(55, 55+freqDivergence, rate);     // interpolate between current frequency and initial untuned frequency
    
    // pdsp::f2p(frequency) >> audioModule->in_pitch();     // update frequency

}



//------------------------------------------------------------------
void Intestine::draw()
{
        // for Debug purpose (delete later)
        if(guiPtr->debugMode) {
            ofNoFill();
            ofSetLineWidth(1);
            ofSetColor(ofColor::mediumPurple);
            ofDrawCircle(position, guiPtr->intestineSyncDistance);
        }

    if (isDigesting) {

        // map the digestion status to color and size -> transformation from liquid to an egg spawning new cells
        ofColor col;
        col.setHex(0x1b9080); // Liquid color

        float brtnss = col.getBrightness();
        brtnss = ofMap( getDigestionStatus(), 0.0, 1.0, brtnss, 255.0 );
        col.setBrightness(brtnss);

        float sat = col.getSaturation();
        sat = ofMap( getDigestionStatus(), 0.0, 1.0, sat, 15.0 );
        col.setSaturation(sat);

        ofSetColor(col);
        ofFill();

        float radius = ofMap( getDigestionStatus(), 0.0, 1.0, 3.0, 5.0 );
       
        ofDrawCircle(digestionPos, radius);
    }

    ofNoFill();
    ofSetLineWidth(3);

    ofSetColor(50);
    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->draw();
    }

    ofSetColor(guiPtr->membraneColor);
    for (int membrane = 0; membrane < 2; membrane++) 
    {

        ofBeginShape();
        for (int i = 0 + membrane; i < intestineMolecules.size(); i+=2)
        {
            glm::vec2 vertexPos = intestineMolecules[i]->position;
            if (i == 0 + membrane || i == intestineMolecules.size() - 2 + membrane)
            {
                ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
                ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
            }
            else
            {
                ofCurveVertex(vertexPos);
            }
        }
        ofEndShape();

    }



    for (unsigned int i = 0; i < intestineMolecules.size(); i++)
    {
        intestineMolecules[i]->draw();
    }
}



//------------------------------------------------------------------
void Intestine::grow()
{

    if ( ofGetElapsedTimeMillis() >= nextGrowth && intestineMolecules.size() < maxElements * 2 && !isDigesting && arousal > 0.4) {

        
        int insertionPoint = floor( ofRandom(1, intestineMolecules.size() * 0.5 -1) ) * 2;      // pick a random position for the insertion of the new element but exclude the first and the last element and be aware to pick an even number, as there are always 2 Molecules because of the two membranes


        // little hack in order not to splice up and insert into the existing array
        // first we just add an element at the end of the array and give it the same position as the last element

        Molecule *mA0 = intestineMolecules[intestineMolecules.size()-2];
        Molecule *mB0 = intestineMolecules[intestineMolecules.size()-1];
        

        Molecule *mA = new Molecule(systemPtr, this);
        glm::vec2 pos = mA0->position;
        mA->reset(pos.x, pos.y, 0., 0.);

        Molecule *mB = new Molecule(systemPtr, this);
        pos = mB0->position;
        mB->reset(pos.x, pos.y, 0., 0.);


        intestineMolecules.push_back(mA);
        systemPtr->allMolecules.push_back(mA);      // and adding a copy to the vector containing totally-all-molecules in the whole System (to be considered in the force calculations like repulsion)
        intestineMolecules.push_back(mB);
        systemPtr->allMolecules.push_back(mB);


        Spring *s = new Spring(systemPtr, STRUCTURE);
        s->connect(mA, mB);
        springs.push_back(s);
        systemPtr->allSprings.push_back(s);


            Spring *sA = new Spring(systemPtr, MEMBRANE);
            sA->connect(mA, mA0);
            springs.push_back(sA);
            systemPtr->allSprings.push_back(sA);

            Spring *sB = new Spring(systemPtr, MEMBRANE);
            sB->connect(mB0, mB);
            springs.push_back(sB);
            systemPtr->allSprings.push_back(sB);


        // the sneaky trick
        // we iterate from the back of the array and give every molecule the position of the element before until we reach the insertion point
        
        for (int i = intestineMolecules.size()-4; i > insertionPoint; i-=2) {
            intestineMolecules[i]->position = intestineMolecules[i-2]->position;
            intestineMolecules[i+1]->position = intestineMolecules[i-1]->position;
        }

        // the Molecules of the insertion point get the position interpolated between their previous position and the element before
        // the effect: looks like the new element was spawned at this position

        intestineMolecules[insertionPoint]  ->position = (intestineMolecules[insertionPoint]  ->position + intestineMolecules[insertionPoint-2]->position) * 0.5;
        intestineMolecules[insertionPoint+1]->position = (intestineMolecules[insertionPoint+1]->position + intestineMolecules[insertionPoint-1]->position) * 0.5;



        nextGrowth = ofGetElapsedTimeMillis() + (int)(guiPtr->intestineGrowInterval) + (int)(ofRandom(guiPtr->intestineGrowInterval*0.5));

    }

    if (intestineMolecules.size() >= 10) mature = true;     // from a certain length its mature to start digestion

}



//------------------------------------------------------------------
void Intestine::keepShape()
{
    // to avoid the Intestine to twist its shape, we need to keep the distance between the opposite Molecules constant
    // for each Membrane segment (Spring), check the distance to the opposite Molecules (two of them) and push them away if to close

    // float threshold = 5.0;
    float threshold = guiPtr->tuneRepulsionThresh * 0.2;    // the threshold around 5.0 (which is 20% of the repulsion radius) is arbitrary but turned out efficient
    

    for (int i = 0; i < springs.size(); i++) { 
            
        if(springs[i]->type == MEMBRANE) {

            // get the ends (Molecules) of the Membrane segment
            Molecule *segmentA = springs[i]->moleculeA;
            Molecule *segmentB = springs[i]->moleculeB;


            // get the opposite Molecule that will be pushed away if necessary
            Molecule *m = segmentA->bondings[0];     // bondings[0] is always the opposite Molecule (aka the one it is connected to via a STRUCTURE spring)

            float distance = minimumDistance2(segmentA->position, segmentB->position, m->position);         
            if(distance > 0 && distance < threshold*threshold) {
                glm::vec2 pushFrc = -springs[i]->normal;     // push into the opposite direction of the normal -> aka away from the Membrane
                distance = sqrt(distance);                
                pushFrc *= ofMap(distance, 0.0, threshold, 2.0, 0.0);   // map the force according to the distance (0.0 to 2.0 is arbitrary but turned out efficient)
                m->addForce(pushFrc);
                // future improvement: force direction should actually not be the normal, but the reflection vector according to the normal! but for the moment this is good enough
            }


            // do the same with the second opposite Molecule
            m = segmentB->bondings[0];     

            distance = minimumDistance2(segmentA->position, segmentB->position, m->position);         
            if(distance > 0 && distance < threshold*threshold) {
                glm::vec2 pushFrc = -springs[i]->normal;     
                distance = sqrt(distance);
                pushFrc *= ofMap(distance, 0.0, threshold, 2.0, 0.0);   
                m->addForce(pushFrc);
            }

        }

    }

    // curling or wursting force
    // for (int i = 0; i < intestineMolecules.size(); i++) { 

    //     int targetSegment = 4;

    //     if (i + (targetSegment * 2) < intestineMolecules.size()) {
    //         Molecule *me = intestineMolecules[i];
    //         Molecule *other = intestineMolecules[i+(targetSegment*2)];

    //         glm::vec2 newForce = other->position - me->position;
    //         newForce = glm::normalize(newForce);
    //         newForce *= 0.5;
    //         me->addForce(newForce);
    //     }

    // }

}



//------------------------------------------------------------------
void Intestine::digest()
{
    
    // only digest if the time has come to
    if ( ofGetElapsedTimeMillis() >= nextDigestion && mature && !isDigesting && arousal > 0.3 && !systemPtr->doNotDigest) {

        isDigesting = true;
        // systemPtr->doNotDigest = true;
        startTime = ofGetElapsedTimeMillis();
        soundCtrl.trigger(1.0);
  
    }


    // if the Intestine is in the process of digesting do the following
    if (isDigesting) {

        // ofLogNotice("fire signal!");

        float arousalFactor = ofMap((arousal*arousal), 0.5, 1.0, 1.0, 0.7, true);
        int signalSpeed = (int)round(guiPtr->intestineDigestionSpeed * arousalFactor); // in milliseconds the signal needs for crossing one segment  
        // int signalSpeed = (guiPtr->intestineDigestionSpeed); // in milliseconds the signal needs for crossing one segment
        int timeIdx = ofGetElapsedTimeMillis()-startTime;   // the time used as basis for the animation
        int segmentIdx = floor(timeIdx / signalSpeed) * 2;      // the index for the Molecules in the signal path is incremented in steps defined by the signal speed


        float newPulseRate = getDigestionStatus();
        newPulseRate *= newPulseRate;   // square for cubic mapping
        newPulseRate = ofMap(newPulseRate, 0., 1., 0.8, 10.0);  // map position of digested molecule (aka digestionStatus) to the frequency of the lfo (in Hz)
        pulseRate.set(newPulseRate);

        // ofLogNotice("segIdx: " + ofToString(segmentIdx));


        // as long as we have not reached the end of the Intestine: shift the point's position along the shape
        if (segmentIdx < intestineMolecules.size() - 2) {

            // mapping the time to the proportion of each segment for the animation
            float prop = ofMap( (timeIdx) % signalSpeed, 0, signalSpeed, 0.0f, 1.0f);   // resulting in a value between 0.0 and 1.0 for the position to interpolate between the two Molecules position
            // ofLogNotice("prop: " + ofToString(prop));


            // calculate the new position of the digested point by interpolating between the position of the two consecutive segment. each segment is the middlepoint between the to opposite Molecules that form the Intestine tunnel
            // this is used for the visualisation only
            digestionPos = (intestineMolecules[segmentIdx]->position + intestineMolecules[segmentIdx+1]->position) * 0.5 * (1-prop)  +  (intestineMolecules[segmentIdx+2]->position + intestineMolecules[segmentIdx+3]->position) * 0.5 * prop;



            // do the actual deformation of the Intestine
            float stretchFactor = 0.75;     // the length of the vector aka the force the Molecules will be pushed away from their idle position


            // displacing the first segment
            glm::vec2 stretchFrc = intestineMolecules[segmentIdx]->position - intestineMolecules[segmentIdx+1]->position;   // get the orientation of the displacement force by getting the vector that one Molecule points to the other
            stretchFrc = glm::normalize(stretchFrc) * stretchFactor;    // normalize it to get a force of 1 in that direction - then multiply it by the stretchFactor for the desired force

            intestineMolecules[segmentIdx]  ->addForce( stretchFrc * (1-prop));        // applying the displacement force proportionaly to the position
            intestineMolecules[segmentIdx+1]->addForce(-stretchFrc * (1-prop));        // do the same with the second Molecule in the opposite direction


            // displacment of the secend segment - in the same way
            stretchFrc = intestineMolecules[segmentIdx+2]->position - intestineMolecules[segmentIdx+3]->position;
            stretchFrc = glm::normalize(stretchFrc) * stretchFactor;

            intestineMolecules[segmentIdx+2]->addForce( stretchFrc * (prop));          // using the inverted proportion
            intestineMolecules[segmentIdx+3]->addForce(-stretchFrc * (prop));



        } else {    // if we have reached the end of the Intestine, stop the digestion

            isDigesting = false;
            // systemPtr->doNotDigest = false;
            nextDigestion = ofGetElapsedTimeMillis() + (int)(guiPtr->intestineDigestionInterval) + (int)(ofRandom(guiPtr->intestineDigestionInterval*0.5));    // choose the next signal timestamp 

            if( floor( ofRandom(2)) == 0) {
                // systemPtr->addBreather(digestionPos.x, digestionPos.y);
                systemPtr->addOnNextFrame(BREATHER, digestionPos.x, digestionPos.y);
            } else {
                // systemPtr->addPumper(digestionPos.x, digestionPos.y);
                systemPtr->addOnNextFrame(PUMPER, digestionPos.x, digestionPos.y);
            }

            // systemPtr->addLiquid(digestionPos.x, digestionPos.y);

            soundCtrl.off();

        }

    }

}



//------------------------------------------------------------------
void Intestine::getSynced()
{
    for (int i = 0; i < systemPtr->pumpers.size(); i++) { 
        Pumper * other = systemPtr->pumpers[i];
        // glm::vec2 newForce = other->position - position;
        if (other->mature) {
            float distance = glm::length2(other->position - position);
            float threshold = guiPtr->intestineSyncDistance;

            if (distance < threshold*threshold) {
                if (other->audioModule->meter() < 0.01) {

                    if(ofGetElapsedTimeMillis() >= nextDigestion - (int)(guiPtr->intestineDigestionInterval*0.5)) nextDigestion = ofGetElapsedTimeMillis();


                }

            }
        }
    }
}


//------------------------------------------------------------------
void Intestine::die()
{
    
    // get the position of the center of the cell aka the average position
    // glm::vec2 center(0,0);
    for (int i = 0; i < intestineMolecules.size(); i++) { 
        intestineMolecules[i]->removeMe = true;      // mark the Molecules to be removed
        // center += intestineMolecules[i]->position;	// sum the positions of all cells
        if (position.x < systemPtr->worldSize.x) {
            if(i % 2 == 0) {
                glm::vec2 spawnPos = (intestineMolecules[i]->position + intestineMolecules[i+1]->position) * 0.5;
                spawnPos.x += ofRandom(-10.0, 10.0);
                spawnPos.y += ofRandom(-10.0, 10.0);
                systemPtr->addLiquid(spawnPos.x, spawnPos.y);
            }
        }
    }
    // center /= intestineMolecules.size(); 	// get the average / dividing by the total amount

    for (int i = 0; i < springs.size(); i++) { 
        springs[i]->removeMe = true;    // mark the Springs to be removed
    }

    // audioModule->disconnectAll();
    // soundCtrl.off();
    if(isDigesting) systemPtr->doNotDigest = false;     // if the Intestine is still digesting when it dies, make sure to reset the doNotDigest flag
    

    isDead = true;      // mark itself to be removed
    systemPtr->organismsToRemove[INTESTINE] = true;
    systemPtr->thereAreCadavers = true;

}



// helper function to calculate the distance between a point and a line segment
// original code from: https://stackoverflow.com/a/1501725
//------------------------------------------------------------------
float Intestine::minimumDistance2(glm::vec2 v, glm::vec2 w, glm::vec2 p) {
    // Return minimum distance between line segment vw and point p
    const float l2 = glm::length2(v - w);  // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 == 0.0) return glm::length2(p - v);   // v == w case
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    // We clamp t from [0,1] to handle points outside the segment vw.
    const float t = glm::clamp( glm::dot(p - v, w - v) / l2, 0.0f, 1.0f);
    const glm::vec2 projection = v + t * (w - v);  // Projection falls on the segment
    return glm::length2(p - projection);
}



//------------------------------------------------------------------
void Intestine::adaptArousal(float amount)
{
    arousal += (amount / (float)intestineMolecules.size()) * 0.25;
    arousal = ofClamp(arousal, 0.0, 1.0);
}

//------------------------------------------------------------------
void Intestine::adaptValence()
{
    valence = systemPtr->getSystemPressure();
}



//------------------------------------------------------------------
void Intestine::updatePosition()
{
    position = glm::vec2(0,0);
    // for (int i = 0; i < intestineMolecules.size(); i++) { 
    for (int i = 0; i < 2; i++) {       // take only the first 2 Molecules to get the position of the "head"
        position += intestineMolecules[i]->position;	// sum the positions of all cells
    }
    // position /= intestineMolecules.size();
    position /= 2;

    if (position.x > systemPtr->worldSize.x) die();
}


//------------------------------------------------------------------
float Intestine::getDigestionStatus() 
{
    int signalSpeed = (guiPtr->intestineDigestionSpeed); // in milliseconds the signal needs for crossing one segment
    int timeIdx = ofGetElapsedTimeMillis()-startTime;   // the time used as basis for the animation
    // int segmentIdx = floor(timeIdx / signalSpeed) * 2; 

    int totalTime = (intestineMolecules.size()-2) * 0.5 * signalSpeed;

    return ofMap( timeIdx, 0, totalTime, 0.0f, 1.0f); 
}


