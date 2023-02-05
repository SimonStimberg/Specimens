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
    nextGrowth = ofGetElapsedTimeMillis() + (guiPtr->neuronGrowSpeed) + (int)(ofRandom(guiPtr->neuronGrowSpeed*0.5));
    mature = false;
    isDead = false;
    debugVal = 10000000;
    arousalThreshold = ofRandom(0.1, 0.35);
    freqDivergence = ofRandom(-50., 50.);

    isSignaling = false;
    signalCounter = 0;
    maxNumSignals = (int)ofRandom(50, 60);
    // maxNumSignals = 20;
    // int intervalTime = (int)ofMap(signalCounter, 0., maxNumSignals, guiPtr->neuronSignalInterval, 50.f);
    // nextSignal = ofGetElapsedTimeMillis() + (guiPtr->neuronSignalInterval) + (int)(ofRandom(guiPtr->neuronSignalInterval*2));
    // nextSignal = ofGetElapsedTimeMillis() + intervalTime + (int)ofRandom(intervalTime * 2);
    nextSignal = ofGetElapsedTimeMillis();
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


    // SETUP AUDIO MODULE
    audioModule.init();
    200.0 >> audioModule.in_sigDuration();
    
    // int allPitches[6] = {84, 87, 91, 94, 98, 104};
    int allPitches[6] = {0, 3, 7, 10, 14, 20};
    // int allPitches[6] = {84, 91, 96, 84, 91, 96};       // root, 5th, 8th
    // int allPitches[6] = {0, 7, 12, 0, 7, 12};       // root, 5th, 8th
    // int allPitches[6] = {84, 91, 98, 84, 91, 98};
    // int allPitches[6] = {84, 91, 94, 84, 91, 94};
    // int allPitches[6] = {84, 87, 91, 84, 87, 91};
    int pick = floor(ofRandom(6));
    // allPitches[pick] >> audioModule.in_pitch();
    pdsp::f2p(880) >> audioModule.in_pitch();
    // audioModule.setup(allPitches[pick]);

    impulse >> audioModule.in_trig();
    impulse.trigger(1.0);

}


//------------------------------------------------------------------
void Neuron::update()
{
    adaptArousal(-0.01);
    adaptValence();

    grow();
    updatePosition();
    if(mature) getSynced();

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->update();
    }

    for (unsigned int i = 0; i < neuronMolecules.size(); i++)
    {
        neuronMolecules[i]->update();
    }



    // if very aroused, mess up syncing by detuning the frequency towards its initial untuned value
    float rate = arousal * arousal;   // use a squared curve for mapping
    rate = ofMap(rate, 0.75, 1., 0., 1., true);     // the detune amount depends on the arousal level. detune above 0.85 accordingly
    float frequency = ofLerp(880, 880+freqDivergence, rate);     // interpolate between current frequency and initial untuned frequency
    
    pdsp::f2p(frequency) >> audioModule.in_pitch();     // update frequency



    signal();
    if(signalCounter >= maxNumSignals) die();
    // if(debugVal <= ofGetElapsedTimeMillis()) die();

    // ofLogNotice("num connections: " + ofToString(connections.size()));
    // ofLogNotice("arousal: " + ofToString(arousal));
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


        // for Debug purpose (delete later)
        if(guiPtr->debugMode) {
            ofNoFill();
            ofSetLineWidth(1);
            ofSetColor(ofColor::limeGreen);
            ofDrawCircle(position, guiPtr->neuronSyncDistance);
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
            

            nextGrowth = ofGetElapsedTimeMillis() + (guiPtr->neuronGrowSpeed) + (int)(ofRandom(guiPtr->neuronGrowSpeed*0.5));    // choose the next growth timestamp        

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

        systemPtr->neuronConnections.push_back(s);  // the inter-Neuron connections are being handled from the higher-order molSystem class

        addConnection(s);               // the Neuron keeps a copy in order to draw it and to know that it has connections
        otherNeuron->addConnection(s);  // give a copy also to the connected Neuron so it knows too (then its drawn twice but that's ok)
      
        
        debugVal = ofGetElapsedTimeMillis() + 6000;
    }

}


//------------------------------------------------------------------
void Neuron::signal()
{
    // only signal if the time has come to
    if ( ofGetElapsedTimeMillis() >= nextSignal && isSignaling == false && arousal > arousalThreshold) {

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


            // displace or "shake" the middle joint on the beginning of each signal
            // increasing the intensity towards the end of the Neurons life span
            glm::vec2 shakeIt = glm::vec2(0., 0.);
            // float angle = ofRandom(0., 12.76);
            // glm::vec2 shakeIt = glm::rotate(shakeIt, angle, glm::vec3(0.0f, 0.0f, 1.0f));
            // float radius = 0.5 + getSignalStatusSquared() * 2.0;
            float radius = 0.5 + arousal * 2.0;
            float angle =  ofDegToRad(ofRandom(360));
            shakeIt.x = radius * cos(angle);
            shakeIt.y = radius * sin(angle);
            // float val = getSignalStatusSquared() * 10.0;
            // shakeIt.x += ofRandom(-val, val);
            // shakeIt.y += ofRandom(-val, val);
            // shakeIt.x += ofRandom(-2., 2.);
            // shakeIt.y += ofRandom(-2., 2.);
            neuronMolecules[0]->addForce(shakeIt);

            
            // set the signal duration for the audio signal, depending on the length of the signal path and the signal speed
            float arousalFactor = ofMap((arousal*arousal), 0.75, 1.0, 1.0, 0.85, true);
            float signalDuration = guiPtr->nronSignalSpeed * arousalFactor * signalFlow.size();
            signalDuration >> audioModule.in_sigDuration();

            impulse.trigger(1.0);   // trigger audio


            // tell the connected Neuron that me has fired and to sync itself
            // dendrites[pick].back()->bondings[1]->neuronPtr->sync();    // send sync signal only to the Neuron me was firing to
            
            // send sync signal to all Neurons that are connected to me
            for (int i = 0; i < connections.size(); i++) { 
                if(connections[i]->moleculeA->neuronPtr == this) connections[i]->moleculeB->neuronPtr->sync();
                else connections[i]->moleculeA->neuronPtr->sync();
            }

            

        }
    }

    // if the Neuron is in the process of signaling do the following
    if (isSignaling) {

        // ofLogNotice("fire signal!");
  
        float arousalFactor = ofMap((arousal*arousal), 0.75, 1.0, 1.0, 0.85, true);
        int signalSpeed = (int)round(guiPtr->nronSignalSpeed * arousalFactor); // in milliseconds the signal needs for crossing one segment
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

            // set the next signal timestamp 
            // int intervalTime = (int)ofMap(getSignalStatusSquared(), 0., 1., guiPtr->neuronSignalInterval, 0.f);  // make it dependent on the total amount of signals send
            // nextSignal = ofGetElapsedTimeMillis() + intervalTime + (int)ofRandom(intervalTime * 2);
            // nextSignal = ofGetElapsedTimeMillis() + guiPtr->neuronSignalInterval + (int)ofRandom(guiPtr->neuronSignalInterval * 2);
            float shorten = guiPtr->neuronSignalInterval * 0.25 + ofRandom(guiPtr->neuronSignalInterval * 0.75);
            shorten = ofMap((arousal*arousal), 0.75, 1.0, 0.0, 1.0, true) * shorten;
            
            nextSignal = ofGetElapsedTimeMillis() + guiPtr->neuronSignalInterval - (int)shorten;   // use a fixed signal rate -> needed for emergent synchronization

            signalFlow.clear();
            signalCounter++;
        }

    }

}



//------------------------------------------------------------------
void Neuron::getSynced()
{
    for (int i = 0; i < systemPtr->pumpers.size(); i++) { 
        Pumper * other = systemPtr->pumpers[i];
        // glm::vec2 newForce = other->position - position;
        if (other->mature) {
            float distance = glm::length2(other->position - position);
            float threshold = guiPtr->neuronSyncDistance;

            if (distance < threshold*threshold) {
                if (other->audioModule.meter() < 0.01) {
                // if (other->audioModule.meter() < 0.01) {
                    // if (audioModule.meter() > 0.55) {
                    //     setPhase.set(audioModule.meter() - 0.05);
                    // } else if (audioModule.meter() < 0.45) {
                    //     setPhase.set(audioModule.meter() + 0.05);
                    // }

                    // setPhase.set(audioModule.meter() + 0.1);
                    sync();
                    sync();
                    sync();
                    sync();
                    sync();
                    ofLogNotice("sync me Mr Pumper pleaseee");

                }

            }
        }
    }
}


//------------------------------------------------------------------
void Neuron::sync()
{
    // if my neighbouring Neuron is firing: reduce the time span to my next signal by a certain amount -> firefly effect
    nextSignal -= guiPtr->neuronSignalInterval * 0.2;
}


//------------------------------------------------------------------
void Neuron::die()
{
    

    for (int i = 0; i < neuronMolecules.size(); i++) { 
        neuronMolecules[i]->removeMe = true;      // mark the Molecules to be removed

        // also spawn new Liquid molecules at the former neuronMolecules positions
        float x = neuronMolecules[i]->position.x + ofRandom(-10.0, 10.0);   // add some randomness to create more dynamic while spawning
        float y = neuronMolecules[i]->position.y + ofRandom(-10.0, 10.0);
        systemPtr->addLiquid(x, y);
    }
    
    for (int i = 0; i < springs.size(); i++) { 
        springs[i]->removeMe = true;    // mark the Springs to be removed
    }

    for (int i = 0; i < connections.size(); i++) {         
        connections[i]->removeMe = true;    // mark the connections -> as every connected Neuron has a copy, all will now that this connection will have to go
    }

    audioModule.disconnectAll();
    

    isDead = true;      // mark itself to be removed
    // systemPtr->organismsToRemove = NEURON;
    systemPtr->organismsToRemove[NEURON] = true;
    systemPtr->thereAreCadavers = true;

}



//------------------------------------------------------------------
void Neuron::adaptArousal(float amount)
{
    arousal += (amount / (float)neuronMolecules.size()) * 0.5;
    arousal = ofClamp(arousal, 0.0, 1.0);
}

//------------------------------------------------------------------
void Neuron::adaptValence()
{
    valence = systemPtr->getSystemPressure();
}



//------------------------------------------------------------------
void Neuron::updatePosition()
{
    position = glm::vec2(0,0);
    for (int i = 0; i < neuronMolecules.size(); i++) { 
        position += neuronMolecules[i]->position;	// sum the positions of all cells
    }
    position /= neuronMolecules.size();

    if (position.x > systemPtr->worldSize.x) {
        ofLogNotice("Relieve me, I'm out of the screen!");
        die();
    }
}



//------------------------------------------------------------------
void Neuron::addConnection(Spring * c)
{
    connections.push_back(c);   // add the connection to my array
    if(connections.size() == 1) nextSignal = ofGetElapsedTimeMillis() + (int)floor(ofRandom(guiPtr->neuronSignalInterval));     // if its the first connection set a random start time for the first signal
}


//------------------------------------------------------------------
void Neuron::removeDeadConnections()
{
    
    // be sure to stop signaling in order not to remove a connection which is still being used for signaling
    if (isSignaling) {
        signalFlow.clear();
        isSignaling = false;    
    }

    // we could also check if its a connection we are signaling on instead of stopping all signals - but this would be a bit too much and nobody notices
    // if (isSignaling && std::find(signalFlow.begin(), signalFlow.end(), value) != signalFlow.end()) {    }

    // erase the connection to the about to die Neuron
    for (int i = 0; i < connections.size(); i++) { 
        if (connections[i]->removeMe) {
            connections.erase(connections.begin() + i);
            // ofLogNotice("removal calling!");
        }
    }

}


//------------------------------------------------------------------
float Neuron::getSignalStatusSquared()
{
    float progress = ofMap(signalCounter, 0., maxNumSignals, 0.f, 1.f);
    // progress *= progress;    // squared? 
    return progress;
}