#include "organisms.h"
#include "ofApp.h"

//------------------------------------------------------------------
Cell::Cell()
{
    systemPtr = NULL;
}

Cell::Cell(molecularSystem *system, cellType myType)
{

    Cell();
    this->systemPtr = system;
    subType = myType;
    mature = false;
    isDead = false;

}


void Cell::set(int num, int x, int y)
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
    maxNumCycles = (int)ofRandom(15, 20);
    

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
    int allPitches[6] = {60, 63, 67, 70, 74, 80};
    // int allPitches[6] = {48, 51, 55, 58, 62, 68};
    int pick = floor(ofRandom(6));
    audioModule.setup(allPitches[pick]);

    ampCtrl >> audioModule.in_level();
    ampCtrl.enableSmoothing(50.0f);
    filterCutoff >> audioModule.in_cutoff();
    filterCutoff.enableSmoothing(50.0f);
    detune >> audioModule.in_tune();
    detune.enableSmoothing(50.0f);
    0.25 >> audioModule.in_breathRate();
    
    
}



//------------------------------------------------------------------
void Cell::update()
{
    grow();
    updatePosition();
    inflate();

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->update();
    }

    for (unsigned int i = 0; i < cellMolecules.size(); i++)
    {
        cellMolecules[i]->update();
    }

    // if(subType == cellType::BREATHER && mature && getInflation() >  1.4) die();
    
    
    

    float newLevel  = ofMap(getInflation(), 0., 1., -36.0, 0.0, true);
    ampCtrl.set(dB(newLevel));

    float newCutoff = ofMap(getInflation(), 0., 1., -30., 30.);
    filterCutoff.set(newCutoff);

    detune.set(systemPtr->getSystemPressure() * 0.5);

    // detune.set(getVelocity());
    // ampCtrl.set(getInflation());
    // ampCtrl.set(getVelocity());


    // cubic mapping from cylce count to breath rate
    float breathRate = (float)audioModule.cycleCount() / (maxNumCycles-1);  // first normalize the ratio of impulsecount to max impulses
    // ofLogNotice("impulse Normal: " + ofToString(breathRate));
    breathRate *= breathRate;     // then square it
    // ofLogNotice("impulse Squared: " + ofToString(breathRate));
    breathRate = ofMap(breathRate, 0., 1., 0.25, 1.0);    // map the normalized and squared ratio to the desired frequency range
    breathRate >> audioModule.in_breathRate();    // set the rate


    if(mature && audioModule.cycleCount() >= maxNumCycles) die();


    // getInflation() >> voicePtr->in_gain();
    // ofLogNotice("inflation: " + ofToString(getInflation()));
    // ofLogNotice("velocity: " + ofToString(getVelocity()));
    // ofLogNotice("LFO: " + ofToString(audioModule.ctrlLfoOut()));
    // ofLogNotice("cycleCount: " + ofToString(audioModule.cycleCount()));
}

//------------------------------------------------------------------
void Cell::draw()
{

    ofNoFill();

        // for Debug purpose (delete later)
        if(guiPtr->debugMode) {
            // ofSetLineWidth(1);
            // ofSetColor(ofColor::indianRed);
            // ofDrawCircle(dbgCellCenter, guiPtr->tuneBreatherExpansionRadius);
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
void Cell::grow()
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
void Cell::inflate() {

	
	if (mature == true && guiPtr->switchOscillation ) {

		// get the position of the center of the cell aka the average position
		glm::vec2 cellCenter(0,0);
		for (int i = 0; i < cellMolecules.size(); i++) { 
			// Molecule * other = cellMolecules.at(i);
			cellCenter += cellMolecules[i]->position;	// sum the positions of all cells
		}
		cellCenter /= cellMolecules.size(); 	// get the average / dividing by the total amount

        dbgCellCenter = cellCenter;     // for debug visualisation of the expansion Radius



		// float freqMultiplier = ofMap(cellMolecules.size(), 13, 31, 2.0, 0.5) + uniqueVal * 0.01;

        // freqMultiplier *= ofMap(cycleCount, 0, maxNumCycles, 1., 1.5, true);

		// float oscillate = 0.0;

        // // work around to compensate the phase shift that occours as the frequency is altered
        // if(freqMultiplier != fmStored) {
        //     float oldPhase = (ofGetElapsedTimef()-timeOfMaturity) * fmStored;
        //     float newPhase = (ofGetElapsedTimef()-timeOfMaturity) * freqMultiplier;

        //     phaseCompensation += oldPhase - newPhase;
        //     // ofLogNotice("Phase compensated!");
        // }

		// // float oscillate = (sin(ofGetElapsedTimef())+1) * 0.5;  // oscillate between 0. and 1.
		// if (subType == cellType::BREATHER) {
		// 	oscillate = sin( (ofGetElapsedTimef()-timeOfMaturity) * freqMultiplier + phaseCompensation) * guiPtr->tuneBreatherOscillationAmount + guiPtr->tuneBreatherOscillationAmount*0.5;	// oscillate between -0.5 and 1.5
        //     // oscillate = sin( (ofGetElapsedTimef()-timeOfMaturity) * freqMultiplier + phaseCompensation) * ofMap(cycleCount, 0, maxNumCycles, 0.057, 0.095);	// oscillate between -0.5 and 1.5
        //     // oscDebug = sin( primePhase);
		// } 
        // else if (subType == cellType::PUMPER) {
		// 	oscillate = ( (ofGetElapsedTimeMillis() + uniqueVal * 100) % (int)(2000*freqMultiplier) <= 300) ? -0.05 : 0.01;	// oscillate between 0.04 : -0.005
		// }

        // if(oscillate > 0.05 && !cycleCheck) {
        //     cycleCount++;
        //     cycleCheck = true;
        //     // ofLogNotice("max cy: " + ofToString(maxNumCycles));
        //     // ofLogNotice("cycles: " + ofToString(cycleCount));
        // } else if (oscillate < 0.0 && cycleCheck) cycleCheck = false;


        // fmStored = freqMultiplier;

        // // ofLogNotice("osc: " + ofToString(oscillate));
        // // ofLogNotice("cycles: " + ofToString(cycleCount));




        // get the control oscillation from the LFO in the audioModule
        float oscillate = ofMap(audioModule.ctrlLfoOut(), -1., 1., -0.2, 1.);     // mapping it from -0.2 instead of 0. - 1. lets the Breather contract a bit between inflations

        // oscillate *= guiPtr->tuneBreatherOscillationAmount;



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
void Cell::die()
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
void Cell::updatePosition()
{
    position = glm::vec2(0,0);
    for (int i = 0; i < cellMolecules.size(); i++) { 
        position += cellMolecules[i]->position;	// sum the positions of all cells
    }
    position /= cellMolecules.size();

    if (position.x > systemPtr->worldSize.x) die();
}



//------------------------------------------------------------------
float Cell::getInflation() {

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
float Cell::getVelocity() {

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