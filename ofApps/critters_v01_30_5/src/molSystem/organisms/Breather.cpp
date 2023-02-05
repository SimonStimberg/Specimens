#include "organisms.h"
#include "ofApp.h"

//------------------------------------------------------------------
Breather::Breather()
{
    systemPtr = NULL;
}

Breather::Breather(molecularSystem *system, cellType myType)
{

    Breather();
    this->systemPtr = system;
    subType = myType;
    mature = false;
    isDead = false;

}


void Breather::set(int num, int x, int y)
{

    // maxGrowth = floor(ofRandom(17, 21));
    maxGrowth = 18;
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

    // add one Middle Spring for the Breathers
    int middle = round(num / 2);    
    // int middle = round(num / 2) + ofRandom(-1, 1);
    // int middle = ofRandom(3, cellMolecules.size()-3);

    Spring *s = new Spring(systemPtr, JOINT);
    s->connect(cellMolecules[0], cellMolecules[middle]);
    springs.push_back(s);
    systemPtr->allSprings.push_back(s);
    

    for (int i = 0; i < num; i++)
    {
        Spring *s = new Spring(systemPtr);

        int j = (i < num - 1) ? i + 1 : 0; // add the next - if reached the end: do the circle
        s->connect(cellMolecules[i], cellMolecules[j]);

        springs.push_back(s);
        systemPtr->allSprings.push_back(s);
    }


    // SETUP AUDIO MODULE
    // int allPitches[6] = {60, 63, 67, 70, 74, 80};
    // float allFreqs[5] = {220, 330, 440, 550, 660};
    // int allPitches[6] = {48, 51, 55, 58, 62, 68};
    // int pick = floor(ofRandom(5));

    frequency = ofRandom(220., 660.);
    audioModule.setup(pdsp::f2p(frequency));
    initFrequency = frequency;
    // audioModule.setup(pdsp::f2p(allFreqs[pick]));

    ampCtrl >> audioModule.in_level();
    ampCtrl.enableSmoothing(50.0f);
    filterCutoff >> audioModule.in_cutoff();
    filterCutoff.enableSmoothing(50.0f);
    detune >> audioModule.in_tune();
    detune.enableSmoothing(50.0f);
    0.25 >> audioModule.in_breathRate();

    // ofLogNotice("pitch 60 in Hz: " + ofToString(pdsp::p2f(60)));
    // ofLogNotice("Frequency is: " + ofToString(allFreqs[pick]));
    ofLogNotice("Frequency is: " + ofToString(frequency));
    
    
}



//------------------------------------------------------------------
void Breather::update()
{
    adaptArousal(-0.01);
    adaptValence();

    grow();
    updatePosition();
    if(mature) syncFrequency();
    inflate();

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->update();
    }

    for (unsigned int i = 0; i < cellMolecules.size(); i++)
    {
        cellMolecules[i]->update();
    }


    

    // if very aroused, mess up syncing by detuning the frequency towards its initial untuned value
    float rate = arousal * arousal;   // use a squared curve for mapping
    rate = ofMap(rate, 0.85, 1., 0., 1., true);     // the detune amount depends on the arousal level. detune above 0.85 accordingly
    frequency = ofLerp(frequency, initFrequency, rate);     // interpolate between current frequency and initial untuned frequency
    
    pdsp::f2p(frequency) >> audioModule.in_pitch();     // update frequency



    float newLevel  = ofMap(getInflation(), 0., 1., -36.0, 0.0, true);
    ampCtrl.set(dB(newLevel));

    float newCutoff = ofMap(getInflation(), 0., 1., -30., 30.);
    filterCutoff.set(newCutoff);


    // ofLogNotice("my freq: " + ofToString(frequency));


    // ALTERNATE SOUND ACCORDING TO VALENCE
    // valence * 0.25 >> audioModule.in_roughness();
    // detune.set(valence * 0.5);



    // cubic mapping from cylce count to breath rate
    // float breathRate = (float)audioModule.cycleCount() / (maxNumCycles-1);  // first normalize the ratio of impulsecount to max impulses
    float breathRate = arousal;     // map the breathRate to the amount of arousal
    // ofLogNotice("impulse Normal: " + ofToString(breathRate));
    breathRate *= breathRate;     // then square it
    // ofLogNotice("impulse Squared: " + ofToString(breathRate));
    breathRate = ofMap(breathRate, 0., 1., 0.25, 0.6);    // map the normalized and squared ratio to the desired frequency range
    breathRate >> audioModule.in_breathRate();    // set the rate


    if(mature && audioModule.cycleCount() >= maxNumCycles) die();


    // getInflation() >> voicePtr->in_gain();
    // ofLogNotice("inflation: " + ofToString(getInflation()));
    // ofLogNotice("velocity: " + ofToString(getVelocity()));
    // ofLogNotice("LFO: " + ofToString(audioModule.ctrlLfoOut()));
    // ofLogNotice("cycleCount: " + ofToString(audioModule.cycleCount()));
    ofLogNotice("arousal: " + ofToString(arousal));
    ofLogNotice("frquency: " + ofToString(frequency));

    // ofLogNotice("valence: " + ofToString(valence));
}

//------------------------------------------------------------------
void Breather::draw()
{

    ofNoFill();

        // for Debug purpose (delete later)
        if(guiPtr->debugMode) {
            ofSetLineWidth(1);
            ofSetColor(ofColor::indianRed);
            // ofDrawCircle(dbgCellCenter, guiPtr->tuneBreatherExpansionRadius);
                    // for Debug purpose (delete later)

            ofDrawCircle(position, guiPtr->pumperSyncDistance);

        }

    ofSetLineWidth(3);




    ofColor col = ofColor::fromHex(0x2bdbe6);

    // ofColor col;
    if(guiPtr->debugMode) {
        if(maxGrowth == 17) {
            col = ofColor::fromHex(0x2bdbe6);
        } else if (maxGrowth == 18) {
            col = ofColor::fromHex(0x2bdb00);
        } else if (maxGrowth == 19) {
            col = ofColor::fromHex(0x2b00e6);
        } else {
            col = ofColor::fromHex(0xff0000);
        }
    }


    if(mature) {
        float brtnss = col.getBrightness();
        brtnss = ofMap( getInflation(), 0.0, 1.0, brtnss, 255.0 );
        col.setBrightness(brtnss);

        float sat = col.getSaturation();
        sat = ofMap( getInflation(), 0.0, 1.0, sat, 15.0 );
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
    for (unsigned int i = 0; i < cellMolecules.size(); i++)
    {
        cellMolecules[i]->draw();
    }
}



//------------------------------------------------------------------
void Breather::grow()
{

    if ( ofGetElapsedTimeMillis() >= nextGrowth && !mature) {

        Molecule *first = cellMolecules[0];
        Molecule *last  = cellMolecules[cellMolecules.size()-1];
        

        Molecule *mA = new Molecule(systemPtr, this);
        glm::vec2 pos = (first->position + last->position) * 0.5;
        mA->reset(pos.x, pos.y, 0., 0.);

        // glm::vec2 averagePos(0,0);
		// for (int i = 0; i < cellMolecules.size(); i++) { 
		// 	// Molecule * other = cellPtr->cellMolecules.at(i);
		// 	averagePos += cellMolecules[i]->position;	// sum the positions of all cells
		// }
		// averagePos /= cellMolecules.size(); 	// get the average / dividing by the total amount

        // pos += (averagePos-pos) * 0.3;
        // mA->reset(averagePos.x, averagePos.y, 0., 0.);

        // disconnect and reconnect the last Spring in the circle
        springs[springs.size()-1]->disconnect();
        springs[springs.size()-1]->connect(mA, last);

        Spring *s = new Spring(systemPtr);
        s->connect(mA, first);
        springs.push_back(s);

        cellMolecules.push_back(mA);
        systemPtr->allMolecules.push_back(mA);
        systemPtr->allSprings.push_back(s);


        // add one Middle Spring for the Breathers
        int middle = round(cellMolecules.size() / 2);    
        
        springs[0]->disconnect();
        springs[0]->connect(cellMolecules[0], cellMolecules[middle]);

        // Spring *s = new Spring(systemPtr);
        // s->connect(cellMolecules[0], cellMolecules[middle]);
        // springs.push_back(s);



        // nextGrowth = 100000000000000;
        // nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(100, 200));
        nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(guiPtr->cellNextGrowth->x, guiPtr->cellNextGrowth->x + guiPtr->cellNextGrowth->x*guiPtr->cellNextGrowth->y));
        
        mature = (cellMolecules.size() >= maxGrowth) ? true : false;
        if(mature) {
            timeOfMaturity = ofGetElapsedTimef();
            audioModule.startBreathing();
        }

    }

}



//------------------------------------------------------------------
void Breather::inflate() {

	
	if (mature == true && guiPtr->switchOscillation ) {

		// get the position of the center of the cell aka the average position
		glm::vec2 cellCenter(0,0);
		for (int i = 0; i < cellMolecules.size(); i++) { 
			// Molecule * other = cellMolecules.at(i);
			cellCenter += cellMolecules[i]->position;	// sum the positions of all cells
		}
		cellCenter /= cellMolecules.size(); 	// get the average / dividing by the total amount

        dbgCellCenter = cellCenter;     // for debug visualisation of the expansion Radius



        // get the control oscillation from the LFO in the audioModule
        float oscillate = ofMap(audioModule.ctrlLfoOut(), -1., 1., -0.2, 1.);     // mapping it from -0.2 instead of 0. - 1. lets the Breather contract a bit between inflations

        // oscillate *= guiPtr->tuneBreatherOscillationAmount;

        // map the oscillator amount to the arousal level
        float oscAmount = 1 - (1 - arousal) * (1 - arousal); // use a negative squared curve for mapping
        oscillate *= oscAmount;



        for (int i = 0; i < cellMolecules.size(); i++) {
        


            // calculate the vector towards the cell center for each molecule
            glm::vec2 desiredPos = cellCenter - cellMolecules[i]->position;     
            float length = glm::length(desiredPos);		// can be optimized with length2 -> no squareroot calculation!
            
            // the max radius around the cell center towards all molecules will strive
            float expansionRadius = guiPtr->tuneBreatherExpansionRadius;
            // map the length towards the radius -> if closer to the center, more force / if already closer to the yielded expansion radius, the force will be less
            length = ofMap(length, 0.0, expansionRadius, 1.0, 0.0, true);       // ok, CAN'T be optimized via length2 because the ratios are then squared as well :'((

            // define the force (negative to steer away from the center)
            glm::vec2 expansionForce = glm::normalize(desiredPos) * -1.0 * length;
            expansionForce *= powf(2, guiPtr->tuneBreatherExpansionForce);   // the force can be tuned
            

            // multiply force by the oscillation
            expansionForce *= oscillate;

            // finally apply force
            cellMolecules[i]->addForce(expansionForce);

            // ofLogNotice(ofToString(expansionForce));
        }

	}

}



//------------------------------------------------------------------
void Breather::syncFrequency()
{

    // float freqCorrect = 0.0;
    // int count = 0;

    for (int i = 0; i < systemPtr->breathers.size(); i++) { 
        Breather * other = systemPtr->breathers[i];
        // glm::vec2 newForce = other->position - position;
        if (other->mature) {
            float distance = glm::length2(other->position - position);
            float threshold = guiPtr->pumperSyncDistance;

            if (distance < threshold*threshold && distance > 0.) {
                // float ratio = (frequency > other->frequency) ? frequency / other->frequency : other->frequency / frequency;
                // float targetRatio = (ratio > 1.25) ? 1.5 : 1.0;
                // 1.0, 1.33333, 1.5, 2.0
                float targetFreq = 0.0;
                if (frequency < 275) {
                    targetFreq = 220;
                } 
                else if (frequency < 385) {
                    targetFreq = 330;
                }
                else if (frequency < 495) {
                    targetFreq = 440;
                }
                else if (frequency < 605) {
                    targetFreq = 550;
                }
                else {
                    targetFreq = 660;
                }
                // float difference = abs(ratio - targetRatio)

                // float targetFreq = (frequency > other->frequency) ? targetRatio * other->frequency : other->frequency / targetRatio;
                float difference = targetFreq - frequency;
                difference *= 0.01;

               
                // freqCorrect += difference;
                frequency += difference;
                // count++;

                // frequency = ofClamp(frequency, 220., 550.);

                // ofLogNotice("sync it!");

            }
        }
    }

    // if (count > 0) freqCorrect /= count;
    // freqCorrect *= 0.5;
    // frequency += freqCorrect;

    // frequency = ofClamp(frequency, 220., 440.);

}



//------------------------------------------------------------------
void Breather::die()
{
    
    // get the position of the center of the cell aka the average position
    glm::vec2 cellCenter(0,0);
    for (int i = 0; i < cellMolecules.size(); i++) { 
        cellMolecules[i]->removeMe = true;      // mark the Molecules to be removed
        // Molecule * other = cellMolecules.at(i);
        cellCenter += cellMolecules[i]->position;	// sum the positions of all cells
    }
    cellCenter /= cellMolecules.size(); 	// get the average / dividing by the total amount

    for (int i = 0; i < springs.size(); i++) { 
        springs[i]->removeMe = true;    // mark the Springs to be removed
    }


    if (position.x < systemPtr->worldSize.x) {
        int numSpawnNeurons = 5;
        for (int i = 0; i < numSpawnNeurons; i++) { 
            float x = cellCenter.x + ofRandom(-10.0, 10.0);
            float y = cellCenter.y + ofRandom(-10.0, 10.0);
            systemPtr->addNeuron(x, y);
        }
    }

    audioModule.disconnectAll();
    

    isDead = true;      // mark itself to be removed
    // systemPtr->cleanUp = true;
    // systemPtr->organismsToRemove = (subType == cellType::BREATHER) ? BREATHER : PUMPER;

    systemPtr->organismsToRemove[BREATHER] = true;
    systemPtr->thereAreCadavers = true;

}



//------------------------------------------------------------------
void Breather::adaptArousal(float amount)
{
    arousal += (amount / (float)cellMolecules.size()) * 0.5;
    arousal = ofClamp(arousal, 0.0, 1.0);

}

//------------------------------------------------------------------
void Breather::adaptValence()
{
    valence = systemPtr->getSystemPressure();
}


//------------------------------------------------------------------
void Breather::updatePosition()
{
    position = glm::vec2(0,0);
    for (int i = 0; i < cellMolecules.size(); i++) { 
        position += cellMolecules[i]->position;	// sum the positions of all cells
    }
    position /= cellMolecules.size();

    if (position.x > systemPtr->worldSize.x) die();
}



//------------------------------------------------------------------
float Breather::getInflation() {

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

//------------------------------------------------------------------
float Breather::getVelocity() {

    glm::vec2 averageVelocity(0., 0.);
    for(unsigned int i = 0; i < cellMolecules.size(); i++){   // i = 1 -> for the breathers exclude the middle spring
        averageVelocity += cellMolecules[i]->velocity;
        
    }
    averageVelocity /= cellMolecules.size();    // without middle spring

    // float idleLength = guiPtr->tuneSpringLength;

    // the value scaling/mapping is based on observing the actual values in the simulation
    // averageStretch = ofMap(averageStretch, -1.2, 4.0, 0.0, 1.0, false);
    // averageStretch = ofMap(averageStretch, -0.5, 3.0, 0.0, 1.0, false);
    // averageStretch = ofMap(averageStretch, -1.0, 3.5, 0.0, 1.0, false);

    // ofLogNotice("inflation: " + ofToString(averageStretch));

    return glm::length(averageVelocity);
}