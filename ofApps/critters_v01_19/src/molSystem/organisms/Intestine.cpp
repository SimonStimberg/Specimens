#include "molecularSystem.h"
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

    nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(1000, 2000));

    for (int i = 0; i < num; i++)
    {


        // float radius = 2.5 * num;
        // float angle = 360.0 / num * i;
        // float xPos = x + radius * cos(ofDegToRad(angle));
        // float yPos = y + radius * sin(ofDegToRad(angle));



        float initThickness = 10.;

        
        Molecule *mA = new Molecule(systemPtr, this);
        mA->reset(x + i * initThickness, y - initThickness, 0., 0.);

        Molecule *mB = new Molecule(systemPtr, this);
        mB->reset(x + i * initThickness, y + initThickness, 0., 0.);

        intestineMolecules.push_back(mA);
        intestineMolecules.push_back(mB);


        Spring *s = new Spring(systemPtr, STRUCTURE);
        s->connect(mA, mB);
        springs.push_back(s);

        if (i > 0) {

            Spring *sA = new Spring(systemPtr, MEMBRANE);
            Molecule *mA0 = intestineMolecules[ intestineMolecules.size() - 4 ];
            sA->connect(mA, mA0);
            springs.push_back(sA);

            Spring *sB = new Spring(systemPtr, MEMBRANE);
            Molecule *mB0 = intestineMolecules[ intestineMolecules.size() - 3 ];
            sB->connect(mB, mB0);
            springs.push_back(sB);

        }

    }

}

//------------------------------------------------------------------
void Intestine::update()
{
    grow();

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->update();
    }

    for (unsigned int i = 0; i < intestineMolecules.size(); i++)
    {
        intestineMolecules[i]->update();
    }
}

//------------------------------------------------------------------
void Intestine::draw()
{

    ofNoFill();
    ofSetLineWidth(3);

    ofSetColor(50);
    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->draw();
    }

    // // ofSetHexColor(0x2bdbe6);
    // ofSetColor(ofColor::mediumVioletRed);
    // ofSetColor(ofColor::limeGreen);
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


    // for (unsigned int i = 0; i < intestineMolecules.size(); i++)
    // {
    //     intestineMolecules[i]->draw();
    // }
}

//------------------------------------------------------------------
void Intestine::grow()
{
    if ( ofGetElapsedTimeMillis() >= nextGrowth && intestineMolecules.size() < 150) {


        
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
        systemPtr->molecules.push_back(mA);      // and adding a copy to the vector containing totally-all-molecules in the whole System (to be considered in the force calculations like repulsion)
        intestineMolecules.push_back(mB);
        systemPtr->molecules.push_back(mB);


        Spring *s = new Spring(systemPtr, STRUCTURE);
        s->connect(mA, mB);
        springs.push_back(s);


            Spring *sA = new Spring(systemPtr, MEMBRANE);
            sA->connect(mA, mA0);
            springs.push_back(sA);

            Spring *sB = new Spring(systemPtr, MEMBRANE);
            sB->connect(mB, mB0);
            springs.push_back(sB);


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



        nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(1000, 2000));

    }

}

//------------------------------------------------------------------
// float Intestine::getInflation() {

//     float averageStretch = 0.0;
//     for(unsigned int i = 0; i < springs.size(); i++){
//         averageStretch += springs[i]->stretch;
//     }
//     averageStretch /= springs.size();
//     averageStretch = ofMap(averageStretch, 7.0, 15.0, 0.0, 1.0, false);

//     return averageStretch;
// }