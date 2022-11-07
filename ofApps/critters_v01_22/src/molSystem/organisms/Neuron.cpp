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

    isSignaling = false;
    nextSignal = ofGetElapsedTimeMillis() + (int)(ofRandom(1000, 2000));
        signalPos = glm::vec2(0.0, 0.0);
        startTime = 0;
        signalIdx[0]=0;
        signalIdx[1]=0;

    // add at first the Middle Joint Molecule
    Molecule *n = new Molecule(systemPtr, this);
    n->reset(x, y, 0., 0.);
    neuronMolecules.push_back(n);
   

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


        // connect the new Molecule to the middle joint through a Spring
        Spring *s = new Spring(systemPtr);
        s->connect(neuronMolecules[0], m);

        springs.push_back(s);


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

    for (unsigned int i = 0; i < connections.size(); i++)
    {
        connections[i]->update();
    }

    for (unsigned int i = 0; i < neuronMolecules.size(); i++)
    {
        neuronMolecules[i]->update();
    }

    signal();
}


//------------------------------------------------------------------
void Neuron::grow()
{
    // here resources could be saved if a flag would notice that there is no more Dendrite to grow on

    // only grow if the time has come to
    if ( ofGetElapsedTimeMillis() >= nextGrowth ) {

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
            systemPtr->molecules.push_back(m);      // and adding a copy to the vector containing totally-all-molecules in the whole System (to be considered in the force calculations like repulsion)

            // connect the spring
            Spring *s = new Spring(systemPtr);
            s->connect(lastElement, m);

            // lastElement->addBonding(m);
            //         m->addBonding(lastElement);
            springs.push_back(s);
            

            nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(1000, 2000));    // choose the next growth timestamp        

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

        // springs.push_back(s);
        connections.push_back(s);
    }

}


//------------------------------------------------------------------
void Neuron::signal()
{
    // only signal if the time has come to
    if ( ofGetElapsedTimeMillis() >= nextSignal && isSignaling == false) {

        // choose a random dendrite to grow on
        int pick = floor(ofRandom(dendrites.size()));
        int dendriteSize = dendrites[pick].size();
        Molecule * lastElement = dendrites[pick][dendriteSize-1];


        // only grow on that Dendrite if it hasn't reached the max length yet
        // and only grow if it hasn't connected to another Neuron yet (aka having just 1 bonding)
        if (lastElement->bondings.size() == 2) {
            isSignaling = true;
            startTime = ofGetElapsedTimeMillis();
            signalIdx[0] = pick;
            signalIdx[1] = 0;

        }
    }
    if (isSignaling) {

        ofLogNotice("fire signal!");
        // isSignaling = false;
        // nextSignal = ofGetElapsedTimeMillis() + (int)(ofRandom(3000, 4000));    // choose the next signal timestamp     

        int dendriteLength = dendrites[signalIdx[0]].size();
        int timeIdx = ofGetElapsedTimeMillis()-startTime;
        int segmentIdx = floor(timeIdx * 0.001);

        ofLogNotice("segIdx: " + ofToString(segmentIdx));



        if (segmentIdx < dendrites[signalIdx[0]].size() - 1) {

            //with prop a function of the time : 
            // float prop = ofMap( (ofGetElapsedTimeMillis()-startTime) % 1000, 0, 1000, 1.0f, 0.0f);
            float prop = ofMap( (timeIdx) % 1000, 0, 1000, 0.0f, 1.0f);
            ofLogNotice("prop: " + ofToString(prop));

            // float prop = 0.5;
            // ofVec3f heyPos = vbo.getVertex(indexVertex) * prop + vbo.getVertex(indexVertex+1) * (1-prop);
            // signalPos = dendrites[signalIdx[0]][signalIdx[1]]->position * prop + dendrites[signalIdx[0]][signalIdx[1] + 1]->position * (1-prop);
            signalPos = dendrites[signalIdx[0]][segmentIdx]->position * (1-prop) + dendrites[signalIdx[0]][segmentIdx + 1]->position * prop;
            // signalPos = dendrites[signalIdx[0]][signalIdx[1] +1]->position;

            // if (prop < 0.05) signalIdx[1]++;
            
            // if (signalIdx[1] >= dendrites[signalIdx[0]].size() -1) {
            // if (segmentIdx >= dendrites[signalIdx[0]].size() -1) {
        } else {
            isSignaling = false;
            nextSignal = ofGetElapsedTimeMillis() + (int)(ofRandom(3000, 4000));    // choose the next signal timestamp 
        }

    }

}



//------------------------------------------------------------------
void Neuron::draw()
{

    ofNoFill();
    ofSetLineWidth(3);

    // ofSetHexColor(0x2bdbe6);
    ofSetHexColor(0xfcfdbd);    // very bright yellow
    // ofSetHexColor(0xd8d97b);    // mellow yellow
    // ofSetHexColor(0xbfcf55);       // grass green
    // ofSetHexColor(0x8fb57e);       // mellow grass green
    // ofSetHexColor(0x69b57e);       // mellow grass green 2
    
    
    
    
    // ofSetColor(63, 255, 208);
    // ofSetColor(ofColor::greenYellow);

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
    for (unsigned int i = 0; i < neuronMolecules.size(); i++)
    {
        neuronMolecules[i]->draw();
    }
    
    ofSetColor(ofColor::darkSlateGrey);
    for (unsigned int i = 0; i < connections.size(); i++)
    {
        connections[i]->draw();
    }

    if(isSignaling) 
    {
        ofSetColor(ofColor::indianRed);
        ofFill();
        ofDrawCircle(signalPos, 5.0);
    }
}