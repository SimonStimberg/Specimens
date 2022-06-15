#include "molecularSystem.h"
#include "ofApp.h"

//------------------------------------------------------------------
Cell::Cell()
{
    systemPtr = NULL;
}

Cell::Cell(molecularSystem *system)
{

    Cell();
    this->systemPtr = system;
}

void Cell::set(int num, int x, int y)
{

    for (int i = 0; i < num; i++)
    {
        float radius = 0.5 * num;
        float angle = 360.0 / num * i;
        float xPos = x + radius * cos(ofDegToRad(angle));
        float yPos = y + radius * sin(ofDegToRad(angle));

        Molecule *m = new Molecule(systemPtr, this);
        m->reset(xPos, yPos, 0., 0.);

        cellMolecules.push_back(m);
    }

    for (int i = 0; i < num; i++)
    {
        Spring *s = new Spring(systemPtr);

        int j = (i < num - 1) ? i + 1 : 0; // add the next - if reached the end: do the circle
        s->reset(cellMolecules[i], cellMolecules[j]);

        cellMolecules[i]->addBonding(cellMolecules[j]);
        cellMolecules[j]->addBonding(cellMolecules[i]);

        springs.push_back(s);
    }
    // this->systemPtr = system;
}

//------------------------------------------------------------------
void Cell::update()
{

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->update();
    }

    for (unsigned int i = 0; i < cellMolecules.size(); i++)
    {
        cellMolecules[i]->update();
    }
}

//------------------------------------------------------------------
void Cell::draw()
{

    ofNoFill();
    ofSetLineWidth(3);
    if(cellMolecules.size() > 22) {
        ofSetHexColor(0x2bdbe6);
    } else {
        // ofSetHexColor(0xf25992);
        ofSetHexColor(0xf22571);        
    }
    // ofSetHexColor(0x2bdbe6);
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
float Cell::getInflation() {

    float averageStretch = 0.0;
    for(unsigned int i = 0; i < springs.size(); i++){
        averageStretch += springs[i]->stretch;
    }
    averageStretch /= springs.size();
    averageStretch = ofMap(averageStretch, 7.0, 15.0, 0.0, 1.0, false);

    return averageStretch;
}