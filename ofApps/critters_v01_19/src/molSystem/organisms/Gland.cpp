#include "molecularSystem.h"
#include "ofApp.h"

//------------------------------------------------------------------
Gland::Gland()
{
    systemPtr = NULL;
}

Gland::Gland(molecularSystem *system)
{

    Gland();
    this->systemPtr = system;
}

void Gland::set(int num, int x, int y)
{

    for (int i = 0; i < num; i++)
    {
        float radius = 2.5 * num;
        float angle = 360.0 / num * i;
        float xPos = x + radius * cos(ofDegToRad(angle));
        float yPos = y + radius * sin(ofDegToRad(angle));

        Molecule *m = new Molecule(systemPtr, this);
        m->reset(xPos, yPos, 0., 0.);

        glandMolecules.push_back(m);
    }

    for (int i = 0; i < num; i++)
    {
        Spring *s = new Spring(systemPtr);

        int j = (i < num - 1) ? i + 1 : 0; // add the next - if reached the end: do the circle
        s->connect(glandMolecules[i], glandMolecules[j]);

        springs.push_back(s);
    }
    // this->systemPtr = system;


    // Double Wheel Shape
    // for (int i = 0; i < num; i++)
    // {
    //     float radius = 2.5 * num + 20;
    //     float angle = 360.0 / num * i;
    //     float xPos = x + radius * cos(ofDegToRad(angle));
    //     float yPos = y + radius * sin(ofDegToRad(angle));

    //     Molecule *m = new Molecule(systemPtr, this);
    //     m->reset(xPos, yPos, 0., 0.);

    //     glandMolecules.push_back(m);

    //     Spring *s = new Spring(systemPtr, STRUCTURE);
    //     s->connect(glandMolecules[i], glandMolecules[i+num]);
    //     springs.push_back(s);
    // }

    // for (int i = num; i < num*2; i++)
    // {
    //     Spring *s = new Spring(systemPtr);

    //     int j = (i < num*2 - 1) ? i + 1 : num; // add the next - if reached the end: do the circle
    //     s->connect(glandMolecules[i], glandMolecules[j]);

    //     springs.push_back(s);
    // }



        // Molecule *m = new Molecule(systemPtr, this);
        // m->reset(x, y, 0., 0.);

        // glandMolecules.push_back(m);

        // Spring *s = new Spring(systemPtr);
        // s->connect(glandMolecules[0], glandMolecules[glandMolecules.size()-1]);
        // springs.push_back(s);

        
        // int partial = round(num / 3);

        // Spring *sA = new Spring(systemPtr);
        // sA->connect(glandMolecules[partial], glandMolecules[glandMolecules.size()-1]);
        // springs.push_back(sA);

        // partial = round( (num / 3) * 2 );

        // Spring *sB = new Spring(systemPtr);
        // sB->connect(glandMolecules[partial], glandMolecules[glandMolecules.size()-1]);
        // springs.push_back(sB);





        // one Middle Spring
        // int middle = round(num / 3);
        int middle = ofRandom(3, glandMolecules.size()-3);

        Spring *s = new Spring(systemPtr);
        s->connect(glandMolecules[0], glandMolecules[middle]);
        springs.push_back(s);


}

//------------------------------------------------------------------
void Gland::update()
{

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->update();
    }

    for (unsigned int i = 0; i < glandMolecules.size(); i++)
    {
        glandMolecules[i]->update();
    }
}

//------------------------------------------------------------------
void Gland::draw()
{

    ofNoFill();
    ofSetLineWidth(3);
    
    // if(glandMolecules.size() > 14) {
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
    // ofSetHexColor(0x2bdbe6);

    ofSetHexColor(0xff447f);        // mellow magenta
    // ofSetColor(50);
    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->draw();
    }

    // ofSetColor(ofColor::limeGreen);
    // ofSetHexColor(0xff447f);        // mellow magenta
    // ofSetColor(50);

    ofBeginShape();
    for (int i = 0; i < glandMolecules.size(); i++)
    {
        glm::vec2 vertexPos = glandMolecules[i]->position;
        if (i == 0)
        {
            ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
            ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
        }
        else if (i == glandMolecules.size() - 1)
        {
            glm::vec2 firstVertexPos = glandMolecules[0]->position;
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

    // ofBeginShape();
    // for (int i = glandMolecules.size()/2; i < glandMolecules.size(); i++)
    // {
    //     glm::vec2 vertexPos = glandMolecules[i]->position;
    //     if (i == glandMolecules.size()/2)
    //     {
    //         ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
    //         ofCurveVertex(vertexPos); // we need to duplicate 0 for the curve to start at point 0
    //     }
    //     else if (i == glandMolecules.size() - 1)
    //     {
    //         glm::vec2 firstVertexPos = glandMolecules[glandMolecules.size()/2]->position;
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


    for (unsigned int i = 0; i < glandMolecules.size(); i++)
    {
        glandMolecules[i]->draw();
    }
}

//------------------------------------------------------------------
float Gland::getInflation() {

    float averageStretch = 0.0;
    for(unsigned int i = 0; i < springs.size(); i++){
        averageStretch += springs[i]->stretch;
    }
    averageStretch /= springs.size();
    averageStretch = ofMap(averageStretch, 7.0, 15.0, 0.0, 1.0, false);

    return averageStretch;
}