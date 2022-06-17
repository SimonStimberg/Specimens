#include "molecularSystem.h"
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
    numDendrites = 0;
}

void Neuron::set(int arms, int elements, int x, int y)
{

    numDendrites = arms;
    numElements = elements;

    Molecule *n = new Molecule(systemPtr, this);
    // Molecule *m = new Molecule(systemPtr);
    n->reset(x, y, 0., 0.);

    neuronMolecules.push_back(n);
   

    for (int i = 0; i < arms; i++)
    {
        // float radius = 2.5 * arms;
        // float radius = guiPtr->tuneRepulsionThresh;
        float radius = guiPtr->tuneSpringLength;
        float angle = 360.0 / arms * i;

        
        for (int j = 0; j < numElements; j++) {
            float xPos = x + ( (j+1)*radius * cos(ofDegToRad(angle)) );
            float yPos = y + ( (j+1)*radius * sin(ofDegToRad(angle)) );

            Molecule *m = new Molecule(systemPtr, this);
            // Molecule *m = new Molecule(systemPtr);
            m->reset(xPos, yPos, 0., 0.);

            neuronMolecules.push_back(m);

            Spring *s = new Spring(systemPtr);

            int bondIdxA = (j == 0) ? 0 : neuronMolecules.size() - 2;       // get the right Molecule index to connect the Spring: if its the first of a new arm, its the middle joint (the first index) - otherwise its the second last
            int bondIdxB = neuronMolecules.size() - 1;                      // the other joint is always the last added Molecule (the one we want to connect)

            s->reset(neuronMolecules[bondIdxA], neuronMolecules[bondIdxB]);

            neuronMolecules[bondIdxA]->addBonding(neuronMolecules[bondIdxB]);
            neuronMolecules[bondIdxB]->addBonding(neuronMolecules[bondIdxA]);

            springs.push_back(s);
        }

    }

}

//------------------------------------------------------------------
void Neuron::update()
{

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
}


//------------------------------------------------------------------
void Neuron::connect(Molecule * moleculeA, Molecule * moleculeB)
{

    Spring *s = new Spring(systemPtr);
    s->reset(moleculeA, moleculeB);

    moleculeA->addBonding(moleculeB);
    moleculeB->addBonding(moleculeA);

    // springs.push_back(s);
    connections.push_back(s);


    // for (int i = 0; i < neuronMolecules.size(); i++) {
    //     if (neuronMolecules[i]->bondings.size() == 1) {

    //         for (int i = 0; i < systemPtr->neurons.size(); i++) {
    //             for (int j = 0; j < systemPtr->neurons[i]->neuronMolecules.size(); j++) {
    //                 if()
    //             }
    //         }
    //     }

    // }

}



//------------------------------------------------------------------
void Neuron::draw()
{

    ofNoFill();
    ofSetLineWidth(3);

    // ofSetHexColor(0x2bdbe6);
    ofSetColor(63, 255, 208);
    // ofSetColor(ofColor::greenYellow);

    for (int i = 0; i < numDendrites; i++) {
        ofBeginShape();
        ofCurveVertex(neuronMolecules[0]->position);
        ofCurveVertex(neuronMolecules[0]->position);

        for (int j = 0; j < numElements; j++) { 
            int indx = 1 + i * numElements + j;
            if (j == numElements - 1) {
                ofCurveVertex(neuronMolecules[indx]->position);
                ofCurveVertex(neuronMolecules[indx]->position);
            } else {
                ofCurveVertex(neuronMolecules[indx]->position);
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
    for (unsigned int i = 0; i < connections.size(); i++)
    {
        connections[i]->draw();
    }
}