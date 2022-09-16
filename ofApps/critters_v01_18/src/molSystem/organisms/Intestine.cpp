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


        Spring *s = new Spring(systemPtr);
        s->connect(mA, mB);
        springs.push_back(s);

        if (i > 0) {

            Spring *sA = new Spring(systemPtr);
            Molecule *mA0 = intestineMolecules[ intestineMolecules.size() - 4 ];
            sA->connect(mA, mA0);
            springs.push_back(sA);

            Spring *sB = new Spring(systemPtr);
            Molecule *mB0 = intestineMolecules[ intestineMolecules.size() - 3 ];
            sB->connect(mB, mB0);
            springs.push_back(sB);

        }


    }

    // for (int i = 0; i < num; i++)
    // {
    //     Spring *s = new Spring(systemPtr);

    //     int j = (i < num - 1) ? i + 1 : 0; // add the next - if reached the end: do the circle
    //     s->connect(intestineMolecules[i], intestineMolecules[j]);

    //     intestineMolecules[i]->addBonding(intestineMolecules[j]);
    //     intestineMolecules[j]->addBonding(intestineMolecules[i]);

    //     springs.push_back(s);
    // }
    // this->systemPtr = system;
}

//------------------------------------------------------------------
void Intestine::update()
{

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

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->draw();
    }

    // if(intestineMolecules.size() > 14) {
    //     ofSetHexColor(0x2bdbe6);        // orginal light blue
    //     // ofSetHexColor(0x1b9080);

    //     // ofSetHexColor(0x14b57f);
    //     // ofSetHexColor(0x00d9c0);
    //     // ofSetHexColor(0x0069c2);     // deep blue
    //     // ofSetHexColor(0x448fc2);        // light blue
        
        

        
        
    // } else {
    //     // ofSetHexColor(0xf25992);
    //     ofSetHexColor(0xf22571);            // original magenta   
    //     // ofSetHexColor(0xff447f);        // mellow magenta
    //     // ofSetHexColor(0xda0080);        // strong magenta
                
    // }
    // // ofSetHexColor(0x2bdbe6);
    ofSetColor(ofColor::mediumVioletRed);
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
            // else if (i == intestineMolecules.size() - 1)
            // {
            //     glm::vec2 firstVertexPos = intestineMolecules[0]->position;
            //     ofCurveVertex(vertexPos);
            //     ofCurveVertex(firstVertexPos); // to draw a curve from pt 6 to pt 0
            //     ofCurveVertex(firstVertexPos); // we duplicate the first point twice
            // }
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
// float Intestine::getInflation() {

//     float averageStretch = 0.0;
//     for(unsigned int i = 0; i < springs.size(); i++){
//         averageStretch += springs[i]->stretch;
//     }
//     averageStretch /= springs.size();
//     averageStretch = ofMap(averageStretch, 7.0, 15.0, 0.0, 1.0, false);

//     return averageStretch;
// }