#include "organisms.h"
#include "ofApp.h"

//------------------------------------------------------------------
Cell::Cell()
{
    systemPtr = NULL;
}

Cell::Cell(molecularSystem *system, cellType myType)
{

    Cell();
    this->systemPtr = system;
    subType = myType;
    mature = false;

}

void Cell::set(int num, int x, int y)
{

    maxGrowth = (subType == BREATHER) ? ofRandom(17, 21) : ofRandom(11, 15);
    nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(3000, 4000));
    uniqueVal = ofRandom(100);
    

    for (int i = 0; i < num; i++)
    {
        float radius = 2.5 * num;
        float angle = 360.0 / num * i;
        float xPos = x + radius * cos(ofDegToRad(angle));
        float yPos = y + radius * sin(ofDegToRad(angle));

        Molecule *m = new Molecule(systemPtr, this);
        m->reset(xPos, yPos, 0., 0.);

        cellMolecules.push_back(m);
    }

    // add one Middle Spring for the Breathers
    if (subType == BREATHER) {

        int middle = round(num / 2);    
        // int middle = round(num / 2) + ofRandom(-1, 1);
        // int middle = ofRandom(3, cellMolecules.size()-3);

        Spring *s = new Spring(systemPtr);
        s->connect(cellMolecules[0], cellMolecules[middle]);
        springs.push_back(s);
    }

    for (int i = 0; i < num; i++)
    {
        Spring *s = new Spring(systemPtr);

        int j = (i < num - 1) ? i + 1 : 0; // add the next - if reached the end: do the circle
        s->connect(cellMolecules[i], cellMolecules[j]);

        springs.push_back(s);
    }
    


}

//------------------------------------------------------------------
void Cell::update()
{
    grow();

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

    ofColor col = (subType == BREATHER) ? ofColor::fromHex(0x2bdbe6) : ofColor::fromHex(0xf22571);
    // if(cellMolecules.size() > 14) {
    //     ofSetHexColor(0x2bdbe6);        // orginal light blue
    //     // ofSetHexColor(0x1b9080);

    //     // ofSetHexColor(0x14b57f);
    //     // ofSetHexColor(0x00d9c0);
    //     // ofSetHexColor(0x0069c2);     // deep blue
    //     // ofSetHexColor(0x448fc2);        // light blue

    //     // ofSetHexColor(0xf25992);
    //     ofSetHexColor(0xf22571);            // original magenta   
    //     // ofSetHexColor(0xff447f);        // mellow magenta
    //     // ofSetHexColor(0xda0080);        // strong magenta
                
    // }
    ofSetColor(col);
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
void Cell::grow()
{
    // int maxGrowth = (subType == BREATHER) ? 19 : 13;

    if ( ofGetElapsedTimeMillis() >= nextGrowth && cellMolecules.size() < maxGrowth) {

        Molecule *first = cellMolecules[0];
        Molecule *last  = cellMolecules[cellMolecules.size()-1];
        

        Molecule *mA = new Molecule(systemPtr, this);
        glm::vec2 pos = (first->position + last->position) * 0.5;
        mA->reset(pos.x, pos.y, 0., 0.);

        // glm::vec2 averagePos(0,0);
		// for (int i = 0; i < cellMolecules.size(); i++) { 
		// 	// Molecule * other = cellPtr->cellMolecules.at(i);
		// 	averagePos += cellMolecules[i]->position;	// sum the positions of all cells
		// }
		// averagePos /= cellMolecules.size(); 	// get the average / dividing by the total amount

        // pos += (averagePos-pos) * 0.3;
        // mA->reset(averagePos.x, averagePos.y, 0., 0.);

        // disconnect and reconnect the last Spring in the circle
        springs[springs.size()-1]->disconnect();
        springs[springs.size()-1]->connect(mA, last);

        Spring *s = new Spring(systemPtr);
        s->connect(mA, first);
        springs.push_back(s);

        cellMolecules.push_back(mA);
        systemPtr->molecules.push_back(mA);


        // add one Middle Spring for the Breathers
        if (subType == BREATHER) {

            int middle = round(cellMolecules.size() / 2);    
            
            springs[0]->disconnect();
            springs[0]->connect(cellMolecules[0], cellMolecules[middle]);

            // Spring *s = new Spring(systemPtr);
            // s->connect(cellMolecules[0], cellMolecules[middle]);
            // springs.push_back(s);
        }




        // nextGrowth = 100000000000000;
        nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(1000, 2000));
        
        mature = (cellMolecules.size() >= maxGrowth) ? true : false;

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