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
            membraneSprings.push_back(sA);

            Spring *sB = new Spring(systemPtr, MEMBRANE);
            Molecule *mB0 = intestineMolecules[ intestineMolecules.size() - 3 ];
            sB->connect(mB, mB0);
            membraneSprings.push_back(sB);

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

    for (unsigned int i = 0; i < membraneSprings.size(); i++)
    {
        membraneSprings[i]->update();
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
    // for (int membrane = 0; membrane < 2; membrane++) 
    // {

    //     ofBeginShape();
    //     for (int i = 0 + membrane; i < intestineMolecules.size(); i+=2)
    //     {
    //         glm::vec2 vertexPos = intestineMolecules[i]->position;
    //         if (i == 0 + membrane || i == intestineMolecules.size() - 2 + membrane)
    //         {
    //             ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
    //             ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
    //         }
    //         else
    //         {
    //             ofCurveVertex(vertexPos);
    //         }
    //     }
    //     ofEndShape();

    // }


    for (unsigned int i = 0; i < intestineMolecules.size(); i++)
    {
        intestineMolecules[i]->draw();
    }
}

//------------------------------------------------------------------
void Intestine::grow()
{
    if ( ofGetElapsedTimeMillis() >= nextGrowth && intestineMolecules.size() < 150) {

        // float initThickness = 10.;

        int insertionPointTest = floor( ofRandom(1, intestineMolecules.size() * 0.5 -1) ) * 2;
        // int insertionPointTest = ofRandom(intestineMolecules.size() / 2) * 2;
        ofLogNotice("Intestine Rows: " + ofToString(intestineMolecules.size()));
        ofLogNotice("Rand Ins Point: " + ofToString(insertionPointTest));
        int insertionPoint = insertionPointTest;



        Molecule * mA1 = intestineMolecules[insertionPoint];
        Molecule * mA0 = intestineMolecules[insertionPoint-2];  
        
        Molecule *mA = new Molecule(systemPtr, this);
        // glm::vec2 pos = m1->position + (m1->position - m2->position) * 0.1;    // the position of the new Molecule should be one arm-length from the previous one - making it shorter (*0.25) make them pop out -> looks more organic
        glm::vec2 pos = (mA0->position + mA1->position) * 0.5;
        mA->reset(pos.x, pos.y, 0., 0.);
        // mA->reset(m1->position.x + 5, m1->position.y + 5, 0., 0.);


        Molecule * mB1 = intestineMolecules[insertionPoint +1];
        Molecule * mB0 = intestineMolecules[insertionPoint -1];  

        Molecule *mB = new Molecule(systemPtr, this);
        // pos = m1->position + (m1->position - m2->position) * 0.1;    // the position of the new Molecule should be one arm-length from the previous one - making it shorter (*0.25) make them pop out -> looks more organic
        pos = (mB0->position + mB1->position) * 0.5;
        mB->reset(pos.x, pos.y, 0., 0.);
        // mB->reset(m2->position.x + 5, m2->position.y + 5, 0., 0.);


        membraneSprings[insertionPoint-2]

        Spring *s = new Spring(systemPtr, STRUCTURE);
        s->connect(mA, mB);
        springs.push_back(s);


            Spring *sA0 = new Spring(systemPtr, MEMBRANE);
            Spring *sA1 = new Spring(systemPtr, MEMBRANE);
            // Molecule *mA0 = intestineMolecules[ intestineMolecules.size() - 4 ];
            sA0->connect(mA, mA0);
            sA1->connect(mA, mA1);
            membraneSprings.push_back(sA0);
            membraneSprings.push_back(sA1);

            Spring *sB0 = new Spring(systemPtr, MEMBRANE);
            Spring *sB1 = new Spring(systemPtr, MEMBRANE);
            // Molecule *mB0 = intestineMolecules[ intestineMolecules.size() - 3 ];
            sB0->connect(mB, mB0);
            sB1->connect(mB, mB1);
            membraneSprings.push_back(sB0);
            membraneSprings.push_back(sB1);


        intestineMolecules.insert(intestineMolecules.begin() + insertionPoint, mA);
        intestineMolecules.insert(intestineMolecules.begin() + insertionPoint + 1, mB);
        systemPtr->molecules.push_back(mA);      // and adding a copy to the vector containing totally-all-molecules in the whole System (to be considered in the force calculations like repulsion)
        systemPtr->molecules.push_back(mB);



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