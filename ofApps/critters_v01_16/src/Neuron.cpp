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
}

void Neuron::set(int num, int x, int y)
{

    Molecule *n = new Molecule(systemPtr, this);
    // Molecule *m = new Molecule(systemPtr);
    n->reset(x, y, 0., 0.);

    neuronMolecules.push_back(n);


    

    for (int i = 0; i < num; i++)
    {
        float radius = 2.5 * num;
        float angle = 360.0 / num * i;
        float xPos = x + radius * cos(ofDegToRad(angle));
        float yPos = y + radius * sin(ofDegToRad(angle));

        Molecule *m = new Molecule(systemPtr, this);
        // Molecule *m = new Molecule(systemPtr);
        m->reset(xPos, yPos, 0., 0.);

        neuronMolecules.push_back(m);

        Spring *s = new Spring(systemPtr);

        // int j = (i < num - 1) ? i + 1 : 0; // add the next - if reached the end: do the circle
        s->reset(neuronMolecules[0], neuronMolecules[i+1]);

        neuronMolecules[0]->addBonding(neuronMolecules[i+1]);
        neuronMolecules[i+1]->addBonding(neuronMolecules[0]);

        springs.push_back(s);

    }

    // this->systemPtr = system;
}

//------------------------------------------------------------------
void Neuron::update()
{

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->update();
    }

    for (unsigned int i = 0; i < neuronMolecules.size(); i++)
    {
        neuronMolecules[i]->update();
    }
}

// //------------------------------------------------------------------
// void Neuron::update()
// {

// }



//------------------------------------------------------------------
void Neuron::draw()
{

    // ofNoFill();
    // ofSetLineWidth(3);
    // if(neuronMolecules.size() > 14) {
    //     ofSetHexColor(0x2bdbe6);
    // } else {
    //     // ofSetHexColor(0xf25992);
    //     ofSetHexColor(0xf22571);        
    // }
    // // ofSetHexColor(0x2bdbe6);
    // ofBeginShape();
    // for (int i = 0; i < neuronMolecules.size(); i++)
    // {
    //     glm::vec2 vertexPos = neuronMolecules[i]->position;
    //     if (i == 0)
    //     {
    //         ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
    //         ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
    //     }
    //     else if (i == neuronMolecules.size() - 1)
    //     {
    //         glm::vec2 firstVertexPos = neuronMolecules[0]->position;
    //         ofCurveVertex(vertexPos);
    //         ofCurveVertex(firstVertexPos); // to draw a curve from pt 6 to pt 0
    //         ofCurveVertex(firstVertexPos); // we duplicate the first point twice
    //     }
    //     else
    //     {
    //         ofCurveVertex(vertexPos);
    //     }
    // }
    // ofEndShape();

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->draw();
    }
    for (unsigned int i = 0; i < neuronMolecules.size(); i++)
    {
        neuronMolecules[i]->draw();
    }
}