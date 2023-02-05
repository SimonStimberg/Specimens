#include "organisms.h"
#include "ofApp.h"

//------------------------------------------------------------------
Pumper::Pumper()
{
    systemPtr = NULL;
}

Pumper::Pumper(molecularSystem *system)
{

    Pumper();
    this->systemPtr = system;
    mature = false;
    isDead = false;

}

void Pumper::set(int num, int x, int y)
{

    maxGrowth = ofRandom(13, 15);
    // nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(3000, 4000));
    nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(guiPtr->cellNextGrowth->x, guiPtr->cellNextGrowth->x + guiPtr->cellNextGrowth->x*guiPtr->cellNextGrowth->y));

    
    uniqueVal = ofRandom(100);
    fmStored = 1.0;
    phaseCompensation = 0.0;
    cycleCheck = true;
    cycleCount = 0;
    maxNumCycles = (int)ofRandom(55, 60);

    arousal = 0.0;
    valence = 0.0;
    

    for (int i = 0; i < num; i++)
    {
        float radius = 2.5 * num;
        float angle = 360.0 / num * i;
        float xPos = x + radius * cos(ofDegToRad(angle));
        float yPos = y + radius * sin(ofDegToRad(angle));

        Molecule *m = new Molecule(systemPtr, this);
        m->reset(xPos, yPos, 0., 0.);

        cellMolecules.push_back(m);
        systemPtr->allMolecules.push_back(m);
    }

    for (int i = 0; i < num; i++)
    {
        Spring *s = new Spring(systemPtr);

        int j = (i < num - 1) ? i + 1 : 0; // add the next - if reached the end: do the circle
        s->connect(cellMolecules[i], cellMolecules[j]);

        springs.push_back(s);
        systemPtr->allSprings.push_back(s);
    }


    // SETUP AUDIO MODULE
    int allPitches[7] = {60, 63, 67, 70, 74, 80, 84};
    int pick = floor(ofRandom(7));
    // audioModule.setup(allPitches[pick]);

    audioModule.initiate();
    trigPhase >> audioModule.in_trigPhase();
    trig >> audioModule.in_trig();
    trig.trigger(1.0);

    setPhase >> audioModule.in_phase();
    setVelocity >> audioModule.in_velocity();


    float rates[3] = {0.25, 0.5, 1.};
    pick = floor(ofRandom(3));
    // float impRate = ofRandom(0.25, 0.5);
    rates[pick] >> audioModule.in_impulseRate();
    
    // 0.5 >> audioModule.in_impulseRate();

    

    // audioModule.out("control") >> impulseVal;

    // ampCtrl >> audioModule.in_gain();
    // ampCtrl.enableSmoothing(50.0f);
    // filterCutoff >> audioModule.in_cutoff();
    // filterCutoff.enableSmoothing(50.0f);
    
    
}



//------------------------------------------------------------------
void Pumper::update()
{
    adaptArousal(-0.01);
    adaptValence();

    grow();
    updatePosition();
    if(mature) sync();
    contract();

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->update();
    }

    for (unsigned int i = 0; i < cellMolecules.size(); i++)
    {
        cellMolecules[i]->update();
    }

    // if(subType == cellType::BREATHER && mature && getInflation() >  1.4) die();
    


    // float newLevel  = ofMap(getInflation(), 0., 1., -36.0, 0.0, true);
    // ampCtrl.set(dB(newLevel));

    // float newCutoff = ofMap(getInflation(), 0., 1., -20., 30.);
    // filterCutoff.set(newCutoff);

    // ampCtrl.set(getInflation());

    // getInflation() >> voicePtr->in_gain();

    // alternate the pitch slightly after each beat for variation
    if(audioModule.impulseCount() > cycleCount) {
        floor(ofRandom(-5, 5)) >> audioModule.in_pitch();
        cycleCount++;
    }



    
    // float impulseRate = ofMap(audioModule.impulseCount(), 0, maxNumCycles, 0.25, 3.0);   // linear maping from impules count to impulse rate

    // ofLogNotice("impulse Count: " + ofToString(audioModule.impulseCount()));

    // cubic mapping from impulse count to impulse rate
    // float impulseRate = audioModule.impulseCount() / (maxNumCycles-1);  // first normalize the ratio of impulsecount to max impulses
    // ofLogNotice("impulse Normal: " + ofToString(impulseRate));
    // impulseRate *= impulseRate;     // then square it
    // ofLogNotice("impulse Squared: " + ofToString(impulseRate));



    // SET THE IMPULSE RATE ACCORDING TO AROUSAL LEVEL
    float impulseRate = arousal * arousal;   // use a squared curve for mapping
    // impulseRate = (arousal > 0.1) ? ofMap(impulseRate, 0., 1., 0.25, 1.5) : 0.0;    // map the normalized and squared ratio to the desired frequency range
    // map the arousal level to frequency
    impulseRate = ofMap(impulseRate, 0.75, 1., 0.5, 1.5, true);  // map max arousal to 1.5 Hz and reach idle frequency at arousal level of 0.75. Idle frequency should be 0.5 Hz
    impulseRate >> audioModule.in_impulseRate();    // set the rate
    
    // if arousal level falls below 0.1: stop beating
    if(arousal < 0.1) {
        setPhase.set(0.9);      // beating is prevented by constantly resetting the phase
        trigPhase.trigger(1.0); // the phase reset has to be retriggered in order to take effect
    }

    // set the sound velocity according to the arousal 
    setVelocity.set(ofMap(arousal, 0., 1., -9., 0.));   // dB non-linear mapping is used. also dim the sound only until -9 dB


    if(mature && audioModule.impulseCount() >= maxNumCycles) die();

    // ofLogNotice("cycleCount: " + ofToString(cycleCount));
    // float test = audioModule.impulseOut();
    // float test = impulseVal.get();
    // ofLogNotice("impulseOut: " + ofToString(test));
    // ofLogNotice("impulseRate in Hz: " + ofToString(impulseRate));
    // ofLogNotice("phase: " + ofToString(audioModule.meter()));
    ofLogNotice("arousal: " + ofToString(arousal));
    ofLogNotice("impulseOut: " + ofToString(audioModule.impulseOut()));
    
}

//------------------------------------------------------------------
void Pumper::draw()
{

    ofNoFill();


        // for Debug purpose (delete later)
        if(guiPtr->debugMode) {
            ofSetLineWidth(1);
            ofSetColor(ofColor::indianRed);
            ofDrawCircle(position, guiPtr->neuronSyncDistance);
        }


    ofSetLineWidth(3);

    ofColor col = ofColor::fromHex(0xf22571);

    if(mature) {
        float brtnss = col.getBrightness();
        brtnss = ofMap( audioModule.impulseOut(), 0.0, 1.0, brtnss, 255.0 );
        col.setBrightness(brtnss);

        float sat = col.getSaturation();
        sat = ofMap( audioModule.impulseOut(), 0.0, 1.0, sat, 55.0 );
        col.setSaturation(sat);
        // col = ofColor(255, 25, 0);
    }


            
    ofSetColor(col);
    ofBeginShape();
    for (int i = 0; i < cellMolecules.size(); i++)
    {
        glm::vec2 vertexPos = cellMolecules[i]->position;
        if (i == 0)
        {
            ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
            ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
        }
        else if (i == cellMolecules.size() - 1)
        {
            glm::vec2 firstVertexPos = cellMolecules[0]->position;
            ofCurveVertex(vertexPos);
            ofCurveVertex(firstVertexPos); // to draw a curve from pt 6 to pt 0
            ofCurveVertex(firstVertexPos); // we duplicate the first point twice
        }
        else
        {
            ofCurveVertex(vertexPos);
        }
    }
    ofEndShape();

    // for (unsigned int i = 0; i < springs.size(); i++)
    // {
    //     // springs[i]->draw();
    // }
    // for (unsigned int i = 0; i < cellMolecules.size(); i++)
    // {
    //     cellMolecules[i]->draw();
    // }
}



//------------------------------------------------------------------
void Pumper::grow()
{
    // int maxGrowth = (subType == BREATHER) ? 19 : 13;

    if ( ofGetElapsedTimeMillis() >= nextGrowth && !mature) {

        Molecule *first = cellMolecules[0];
        Molecule *last  = cellMolecules[cellMolecules.size()-1];
        

        Molecule *mA = new Molecule(systemPtr, this);
        glm::vec2 pos = (first->position + last->position) * 0.5;
        mA->reset(pos.x, pos.y, 0., 0.);


        // disconnect and reconnect the last Spring in the circle
        springs[springs.size()-1]->disconnect();
        springs[springs.size()-1]->connect(mA, last);

        Spring *s = new Spring(systemPtr);
        s->connect(mA, first);
        springs.push_back(s);

        cellMolecules.push_back(mA);
        systemPtr->allMolecules.push_back(mA);
        systemPtr->allSprings.push_back(s);




        // nextGrowth = 100000000000000;
        // nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(100, 200));
        nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(guiPtr->cellNextGrowth->x, guiPtr->cellNextGrowth->x + guiPtr->cellNextGrowth->x*guiPtr->cellNextGrowth->y));
        
        mature = (cellMolecules.size() >= maxGrowth) ? true : false;
        if(mature) {
            audioModule.startImpulse();
            timeOfMaturity = ofGetElapsedTimef();
        }

    }

}



//------------------------------------------------------------------
void Pumper::contract() {

	
	if (mature == true && guiPtr->switchOscillation ) {

		// get the position of the center of the cell aka the average position
		glm::vec2 cellCenter(0,0);
		for (int i = 0; i < cellMolecules.size(); i++) { 
			// Molecule * other = cellMolecules.at(i);
			cellCenter += cellMolecules[i]->position;	// sum the positions of all cells
		}
		cellCenter /= cellMolecules.size(); 	// get the average / dividing by the total amount


        
        float oscillate = -audioModule.impulseOut() * guiPtr->pmprImpulseAmt;

        // ofLogNotice("osc: " + ofToString(oscillate));
        // ofLogNotice("cycles: " + ofToString(cycleCount));

        // ofLogNotice("phase comp: " + ofToString(phaseCompensation));

        // int phasor = ((int)(primePhase * 100000)) % 628318;
        // ofLogNotice("phase: " + ofToString(phasor));


        // map the oscillator amount to the arousal level
        float oscAmount = 1 - (1 - arousal) * (1 - arousal); // use a negative squared curve for mapping
        // oscAmount *= oscAmount;
        // oscAmount = 1 - oscAmount;
        oscillate *= oscAmount;     // apply the amount to the oscillation


        for (int i = 0; i < cellMolecules.size(); i++) {
        
            // glm::vec2 expansionForce(0,0);



            // steer towards the average position
            glm::vec2 desiredPos = cellCenter - cellMolecules[i]->position;
            // expansionForce = desiredPos - velocity;		// ????? velocity???
        
            float length = glm::length(desiredPos);		// can be optimized with length2 -> no squareroot calculation!
            
            float expansionRadius = cellMolecules.size() * guiPtr->tuneExpansionRadius;
            
            length = ofMap(length, 0.0, expansionRadius, 1.0, 0.0, true);       // ok, CAN'T be optimized via length2 because the ratios are then squared as well :'((


            

            

            // expansionForce *= 0.001;
            // expansionForce = limitVec(expansionForce, 0.2);	// limit the force
            glm::vec2 expansionForce = glm::normalize(desiredPos) * -1.0 * length;
            expansionForce *= powf(2, guiPtr->tuneExpansionForce);
            // expansionForce *= -1.f;		// inverse it to steer away from the center


            expansionForce *= oscillate;
            // ofLogNotice(ofToString(oscillate));

            cellMolecules[i]->addForce(expansionForce);

            // ofLogNotice(ofToString(expansionForce));
        }

	}

}



//------------------------------------------------------------------
void Pumper::sync()
{
    // glm::vec2 cellCenter(0,0);
    // for (int i = 0; i < cellMolecules.size(); i++) { 
    //     cellCenter += cellMolecules[i]->position;	// sum the positions of all cells
    // }
    // cellCenter /= cellMolecules.size();
    // position = cellCenter;

    for (int i = 0; i < systemPtr->pumpers.size(); i++) { 
        Pumper * other = systemPtr->pumpers[i];
        // glm::vec2 newForce = other->position - position;
        if (other->mature) {
            float distance = glm::length2(other->position - position);
            float threshold = guiPtr->pumperSyncDistance;

            if (distance < threshold*threshold && distance > 0.) {
                if (other->audioModule.meter() < 0.01 && (audioModule.meter() > 0.6 || audioModule.meter() < 0.4)) {        // before < 0.4 || > 0.6
                // if (other->audioModule.meter() < 0.01) {
                    // if (audioModule.meter() > 0.55) {
                    //     setPhase.set(audioModule.meter() - 0.05);
                    // } else if (audioModule.meter() < 0.45) {
                    //     setPhase.set(audioModule.meter() + 0.05);
                    // }

                    // setPhase.set(audioModule.meter() + 0.1);
                    setPhase.set(0.5);      // before 0.5
                    trigPhase.trigger(1.0);

                }

            }
        }
    }
}



//------------------------------------------------------------------
void Pumper::die()
{
    
    // get the position of the center of the cell aka the average position
    glm::vec2 cellCenter(0,0);
    for (int i = 0; i < cellMolecules.size(); i++) { 
        cellMolecules[i]->removeMe = true;      // mark the Molecules to be removed
        // Molecule * other = cellMolecules.at(i);
        cellCenter += cellMolecules[i]->position;	// sum the positions of all cells

            // also spawn new Liquid molecules at the former neuronMolecules positions
            float x = cellMolecules[i]->position.x + ofRandom(-10.0, 10.0);   // add some randomness to create more dynamic while spawning
            float y = cellMolecules[i]->position.y + ofRandom(-10.0, 10.0);
            systemPtr->addLiquid(x, y);
    }
    cellCenter /= cellMolecules.size(); 	// get the average / dividing by the total amount

    for (int i = 0; i < springs.size(); i++) { 
        springs[i]->removeMe = true;    // mark the Springs to be removed
    }

    // if (position.x < systemPtr->worldSize.x) {
    //     int numSpawnNeurons = 3;
    //     for (int i = 0; i < numSpawnNeurons; i++) { 
    //         float x = cellCenter.x + ofRandom(-10.0, 10.0);
    //         float y = cellCenter.y + ofRandom(-10.0, 10.0);
    //         systemPtr->addNeuron(x, y);
    //     }
    // }


    audioModule.disconnectAll();
    

    isDead = true;      // mark itself to be removed
    // systemPtr->cleanUp = true;
    // systemPtr->organismsToRemove = (subType == cellType::BREATHER) ? BREATHER : PUMPER;

    systemPtr->organismsToRemove[PUMPER] = true;
    systemPtr->thereAreCadavers = true;


}



//------------------------------------------------------------------
void Pumper::adaptArousal(float amount)
{
    arousal += (amount / (float)cellMolecules.size()) * 0.5;
    arousal = ofClamp(arousal, 0.0, 1.0);

}

//------------------------------------------------------------------
void Pumper::adaptValence()
{
    valence = systemPtr->getSystemPressure();
}



//------------------------------------------------------------------
void Pumper::updatePosition()
{
    position = glm::vec2(0,0);
    for (int i = 0; i < cellMolecules.size(); i++) { 
        position += cellMolecules[i]->position;	// sum the positions of all cells
    }
    position /= cellMolecules.size();
    
    if (position.x > systemPtr->worldSize.x) {
        ofLogNotice("Relieve me, I'm out of the screen!");
        die();
    }
}



//------------------------------------------------------------------
float Pumper::getInflation() {

    float averageStretch = 0.0;
    for(unsigned int i = 1; i < springs.size(); i++){   // i = 1 -> for the breathers exclude the middle spring
        averageStretch += springs[i]->stretch;
    }
    averageStretch /= springs.size() - 1;    // without middle spring

    // float idleLength = guiPtr->tuneSpringLength;

    // the value scaling/mapping is based on observing the actual values in the simulation
    // averageStretch = ofMap(averageStretch, -1.2, 4.0, 0.0, 1.0, false);
    // averageStretch = ofMap(averageStretch, -0.5, 3.0, 0.0, 1.0, false);
    averageStretch = ofMap(averageStretch, -1.0, 3.5, 0.0, 1.0, false);

    // ofLogNotice("inflation: " + ofToString(averageStretch));

    return averageStretch;
}