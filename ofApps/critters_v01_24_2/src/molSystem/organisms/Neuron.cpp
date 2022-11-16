#include "organisms.h"
#include "ofApp.h"

//------------------------------------------------------------------
Neuron::Neuron()
{
    systemPtr = NULL;
}

Neuron::Neuron(molecularSystem *system)
{

    Neuron();
    this->systemPtr = system;

}

void Neuron::set(int arms, int elements, int x, int y)
{

    dendrites.resize(arms);
    maxArmElements = elements;
    nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(1000, 2000));
    mature = false;
    isDead = false;
    debugVal = 10000000;

    isSignaling = false;
    signalCounter = 0;
    nextSignal = ofGetElapsedTimeMillis() + (int)(ofRandom(1000, 2000));
        // signalPos = glm::vec2(0.0, 0.0);
        // startTime = 0;

    // add at first the Middle Joint Molecule
    Molecule *n = new Molecule(systemPtr, this);
    n->reset(x, y, 0., 0.);
    neuronMolecules.push_back(n);
    systemPtr->allMolecules.push_back(n);
   

    for (int i = 0; i < arms; i++)
    {
        // float radius = 2.5 * arms;
        // float radius = guiPtr->tuneRepulsionThresh;
        float radius = guiPtr->tuneSpringLength;
        float angle = 360.0 / arms * i;
        
        float xPos = x + ( radius * cos(ofDegToRad(angle)) );
        float yPos = y + ( radius * sin(ofDegToRad(angle)) );

        Molecule *m = new Molecule(systemPtr, this);
        m->reset(xPos, yPos, 0., 0.);

        dendrites[i].push_back(m);
        neuronMolecules.push_back(m);   // add a copy to the all-molecules vector for managing purpose
        systemPtr->allMolecules.push_back(m);   // and another copy to the overall Molecule vector of the System


        // connect the new Molecule to the middle joint through a Spring
        Spring *s = new Spring(systemPtr);
        s->connect(neuronMolecules[0], m);

        springs.push_back(s);
        systemPtr->allSprings.push_back(s);     // add to all Strings as well


    }

}


//------------------------------------------------------------------
void Neuron::update()
{
    grow();

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->update();
    }

    // for (unsigned int i = 0; i < connections.size(); i++)
    // {
    //     connections[i]->update();
    // }

    for (unsigned int i = 0; i < neuronMolecules.size(); i++)
    {
        neuronMolecules[i]->update();
    }

    // signal();
    // if(signalCounter >= 2) die();

    if(debugVal <= ofGetElapsedTimeMillis()) die();
}


//------------------------------------------------------------------
void Neuron::draw()
{
    if(isSignaling) 
    {
        ofSetColor(ofColor::white);
        ofFill();
        ofDrawCircle(signalPos, 4);
    }

    
    // ofSetHexColor(0x2bdbe6);
    ofSetHexColor(0xfcfdbd);    // very bright yellow
    // ofSetHexColor(0xd8d97b);    // mellow yellow
    // ofSetHexColor(0xbfcf55);       // grass green
    // ofSetHexColor(0x8fb57e);       // mellow grass green
    // if (mature) {
    //     ofSetHexColor(0x69b57e);       // mellow grass green 2
    // } 
    
    // ofSetColor(63, 255, 208);
    // ofSetColor(ofColor::greenYellow);


    ofFill();
    ofDrawCircle(neuronMolecules[0]->position, 3);


    ofNoFill();
    ofSetLineWidth(3);

    for (int i = 0; i < dendrites.size(); i++) {
        ofBeginShape();
        ofCurveVertex(neuronMolecules[0]->position);
        ofCurveVertex(neuronMolecules[0]->position);

        for (int j = 0; j < dendrites[i].size(); j++) { 
            
            if (j == dendrites[i].size()-1) {
                ofCurveVertex(dendrites[i][j]->position);
                ofCurveVertex(dendrites[i][j]->position);
                // ofCurveVertex(neuronMolecules[indx]->position);
            } else {
                ofCurveVertex(dendrites[i][j]->position);
            }
            
        }
        ofEndShape();

    }

    // for (unsigned int i = 0; i < springs.size(); i++)
    // {
    //     springs[i]->draw();
    // }
    // for (unsigned int i = 0; i < neuronMolecules.size(); i++)
    // {
    //     neuronMolecules[i]->draw();
    // }
    
    ofSetColor(ofColor::darkSlateGrey);
    for (unsigned int i = 0; i < connections.size(); i++)
    {
        connections[i]->draw();
    }


}



//------------------------------------------------------------------
void Neuron::grow()
{
    // here resources could be saved if a flag would notice that there is no more Dendrite to grow on

    // only grow if the time has come to
    if ( ofGetElapsedTimeMillis() >= nextGrowth && !mature) {

        // choose a random dendrite to grow on
        int pick = floor(ofRandom(dendrites.size()));
        int dendriteSize = dendrites[pick].size();
        Molecule * lastElement = dendrites[pick][dendriteSize-1];


        // only grow on that Dendrite if it hasn't reached the max length yet
        // and only grow if it hasn't connected to another Neuron yet (aka having just 1 bonding)
        if (dendriteSize < maxArmElements && lastElement->bondings.size() == 1) {

            glm::vec2 pos = lastElement->position + (lastElement->position - neuronMolecules[0]->position)*0.1;    // the position of the new Molecule should be one arm-length from the previous one - making it shorter (*0.25) make them pop out -> looks more organic
            Molecule *m = new Molecule(systemPtr, this);
            m->reset(pos.x, pos.y, 0., 0.);

            dendrites[pick].push_back(m);
            neuronMolecules.push_back(m);           // add a copy to the all-molecules vector of the Neuron
            systemPtr->allMolecules.push_back(m);      // and adding a copy to the vector containing totally-all-molecules in the whole System (to be considered in the force calculations like repulsion)

            // connect the spring
            Spring *s = new Spring(systemPtr);
            s->connect(lastElement, m);

            // lastElement->addBonding(m);
            //         m->addBonding(lastElement);
            springs.push_back(s);
            systemPtr->allSprings.push_back(s);
            

            nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(1000, 2000));    // choose the next growth timestamp        

        }

        mature = true;
        for (unsigned int i = 0; i < dendrites.size(); i++) {
            if (dendrites[i].size() < maxArmElements) {
                mature = false;
                break;
            }
        }
    }
}


//------------------------------------------------------------------
void Neuron::connect(Molecule * myMolecule, Molecule * otherMolecule)
{
    Neuron * otherNeuron = otherMolecule->neuronPtr;
    bool connect = true;

    // check if Me already has a connection with the other Molecule's Neuron - if so DON'T connect
    // therefore iterate over all arms (dendrites)
    for (unsigned int i = 0; i < dendrites.size(); i++) {
        
        // find the arm end (= last index in the array)
        int idxArmEnd = dendrites[i].size()-1;

        // check if the end Molecule already is connected to that other Neuron
        for (unsigned int j = 0; j < dendrites[i][idxArmEnd]->bondings.size(); j++) {   
            if (dendrites[i][idxArmEnd]->bondings[j]->neuronPtr == otherNeuron) { 
                connect = false;    // if so set the flag to avoid connection
                break; 
            }
        }

        if(!connect) break;
    }

    if(connect) {
        Spring *s = new Spring(systemPtr);
        s->connect(myMolecule, otherMolecule);

        connections.push_back(s);
        otherNeuron->connections.push_back(s);

        springs.push_back(s);
        systemPtr->allSprings.push_back(s);
        
        debugVal = ofGetElapsedTimeMillis() + 3000;
    }

}


//------------------------------------------------------------------
void Neuron::signal()
{
    // only signal if the time has come to
    if ( ofGetElapsedTimeMillis() >= nextSignal && isSignaling == false) {

        // choose a random Dendrite for the signal
        int pick = floor(ofRandom(dendrites.size()));
        int dendriteSize = dendrites[pick].size();
        Molecule * lastElement = dendrites[pick][dendriteSize-1];

        // check if the choosen Dendrite is already bound to another Neuron - if so initialize the signal process
        if (lastElement->bondings.size() == 2) {

            isSignaling = true;
            startTime = ofGetElapsedTimeMillis();


            // make a vector of Molecules to define the signal's path
                signalFlow.clear();
                signalFlow.push_back(neuronMolecules[0]); // add the middle joint as starting point

                // add the molecules of the Dendrite that has been picked
                for (int i = 0; i < dendrites[pick].size(); i++) { 
                    signalFlow.push_back(dendrites[pick][i]);
                }
                
                signalFlow.push_back(dendrites[pick].back()->bondings[1]);  // add the first Molecule of the connected Neuron's Dendrite (has to be index 1, because it was added the last)

                while(signalFlow.back()->bondings.size() == 2) {   // move along its Dendrite until we reach the middle joint and add those to our vector
                    signalFlow.push_back(signalFlow.back()->bondings[0]);   // index 0 -> because choose always the first bonded Molecule "back propagating"
                }


        }
    }

    // if the Neuron is in the process of signaling do the following
    if (isSignaling) {

        // ofLogNotice("fire signal!");
  

        int signalSpeed = 50; // in milliseconds the signal needs for crossing one segment
        int timeIdx = ofGetElapsedTimeMillis()-startTime;   // the time used as basis for the animation
        int segmentIdx = floor(timeIdx / signalSpeed);      // the index for the Molecules in the signal path is incremented in steps defined by the signal speed

        // ofLogNotice("segIdx: " + ofToString(segmentIdx));


        // as long as we have not reached the end of the signal path: shift the point's position along the signal path
        if (segmentIdx < signalFlow.size() - 1) {

            // mapping the time for each segment animation
            float prop = ofMap( (timeIdx) % signalSpeed, 0, signalSpeed, 0.0f, 1.0f);   // resulting in a value between 0.0 and 1.0 for the position to interpolate between the two Molecules position
            // ofLogNotice("prop: " + ofToString(prop));

            // calculate the new position on the signal path by interpolating between the position of the two Molecules defining the actual segment of the signal path
            signalPos = signalFlow[segmentIdx]->position * (1-prop) + signalFlow[segmentIdx + 1]->position * prop;


        } else {    // if we have reached the end of the signal path, stop the signaling
            isSignaling = false;
            nextSignal = ofGetElapsedTimeMillis() + (int)(ofRandom(3000, 4000));    // choose the next signal timestamp 
            signalFlow.clear();
            signalCounter++;
        }

    }

}


//------------------------------------------------------------------
void Neuron::die()
{
    
    // get the position of the center of the cell aka the average position
    // glm::vec2 center(0,0);
    for (int i = 0; i < neuronMolecules.size(); i++) { 
        neuronMolecules[i]->removeMe = true;      // mark the Molecules to be removed
        // center += neuronMolecules[i]->position;	// sum the positions of all cells
    }
    // center /= neuronMolecules.size(); 	// get the average / dividing by the total amount

    for (int i = 0; i < springs.size(); i++) { 
        springs[i]->removeMe = true;    // mark the Springs to be removed
    }

    for (int i = 0; i < connections.size(); i++) { 
        
        Neuron * otherNeuron = (connections[i]->moleculeA->neuronPtr == this) ? connections[i]->moleculeB->neuronPtr : connections[i]->moleculeA->neuronPtr;

        otherNeuron->isSignaling = false;
        otherNeuron->signalFlow.clear();
        
        connections[i]->disconnect();


        // for (int j = 0; j < otherNeuron->connections.size(); j++) { 

        //     if (otherNeuron->connections[j] == connections[i]) {

        //         otherNeuron->connections.erase( otherNeuron->connections.begin() + j);
		// 	    break;

        //     }

        // }

        // connections.erase( connections.begin() + i );




    }

    // for (int i = 0; i < dendrites.size(); i++) { 
    //     int idxArmEnd = dendrites[i].size()-1;

    //     if (dendrites[i][idxArmEnd]->bondings.size() == 2) {

    //     }

    //             for (unsigned int j = 0; j < dendrites[i][idxArmEnd]->bondings.size(); j++) {   
    //         if (dendrites[i][idxArmEnd]->bondings[j]->neuronPtr == otherNeuron) { 
    //             connect = false;    // if so set the flag to avoid connection
    //             break; 
    //         }
    //     }

    //     springs[i]->removeMe = true;    // mark the Springs to be removed
    // }



    // int numSpawnNeurons = (subType == cellType::BREATHER) ? 5 : 3;
    // for (int i = 0; i < numSpawnNeurons; i++) { 
    //     float x = center.x + ofRandom(-10.0, 10.0);
    //     float y = center.y + ofRandom(-10.0, 10.0);
    //     systemPtr->addNeuron(x, y);
    // }
    

    isDead = true;      // mark itself to be removed
    // systemPtr->cleanUp = true;
    systemPtr->organismsToRemove = NEURON;

}