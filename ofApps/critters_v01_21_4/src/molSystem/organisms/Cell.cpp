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
    inflate();

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

    if(subType == BREATHER) {
        float brtnss = col.getBrightness();
        brtnss = ofMap( getInflation(), 0.0, 1.0, brtnss, 255.0 );
        col.setBrightness(brtnss);

        float sat = col.getSaturation();
        sat = ofMap( getInflation(), 0.0, 1.0, sat, 15.0 );
        col.setSaturation(sat);
        // col = ofColor(255, 25, 0);
    }

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
        nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(100, 200));
        
        mature = (cellMolecules.size() >= maxGrowth) ? true : false;

    }

    
    

}


//------------------------------------------------------------------
void Cell::inflate() {

	
	if (mature == true && guiPtr->switchOscillation ) {

		// get the position of the center of the cell aka the average position
		glm::vec2 cellCenter(0,0);
		for (int i = 0; i < cellMolecules.size(); i++) { 
			// Molecule * other = cellMolecules.at(i);
			cellCenter += cellMolecules[i]->position;	// sum the positions of all cells
		}
		cellCenter /= cellMolecules.size(); 	// get the average / dividing by the total amount


		float freqMultiplier = ofMap(cellMolecules.size(), 13, 31, 2.0, 0.5) + uniqueVal * 0.01;
		// float phaseShift = ofRandom(3.14159);

		float oscillate = 0.0;
		// float oscillate = (sin(ofGetElapsedTimef())+1) * 0.5;  // oscillate between 0. and 1.
		if (subType == BREATHER) {
			oscillate = sin(ofGetElapsedTimef() * freqMultiplier) * guiPtr->tuneOscillationAmount + guiPtr->tuneOscillationAmount*0.5;	// oscillate between -0.5 and 1.5
		} else {
			oscillate = ( (ofGetElapsedTimeMillis() + uniqueVal * 100) % (int)(2000*freqMultiplier) <= 300) ? -0.05 : 0.01;	// oscillate between 0.04 : -0.005
		}


        for (int i = 0; i < cellMolecules.size(); i++) {
        
            // glm::vec2 expansionForce(0,0);

            // steer towards the average position
            glm::vec2 desiredPos = cellCenter - cellMolecules[i]->position;
            // expansionForce = desiredPos - velocity;		// ????? velocity???
        
            float length = glm::length(desiredPos);		// can be optimized with length2 -> no squareroot calculation!

            float expansionRadius = cellMolecules.size() * guiPtr->tuneExpansionRadius;

            length = ofMap(length, 0.0, expansionRadius, 1.0, 0.0, true);

            // expansionForce *= 0.001;
            // expansionForce = limitVec(expansionForce, 0.2);	// limit the force
            glm::vec2 expansionForce = glm::normalize(desiredPos) * -1.0 * length;
            expansionForce *= powf(2, guiPtr->tuneExpansionForce);
            // expansionForce *= -1.f;		// inverse it to steer away from the center


            expansionForce *= oscillate;
            // ofLogNotice(ofToString(oscillate));

            cellMolecules[i]->addForce(expansionForce);

            // ofLogNotice(ofToString(expansionForce));
        }


	}

	
}


//------------------------------------------------------------------
float Cell::getInflation() {

    float averageStretch = 0.0;
    for(unsigned int i = 1; i < springs.size(); i++){   // i = 1 -> for the breathers exclude the middle spring
        averageStretch += springs[i]->stretch;
    }
    averageStretch /= springs.size() - 1;    // without middle spring

    // float idleLength = guiPtr->tuneSpringLength;

    // the value scaling/mapping is based on observing the actual values in the simulation
    // averageStretch = ofMap(averageStretch, -1.2, 4.0, 0.0, 1.0, false);
    // averageStretch = ofMap(averageStretch, -0.5, 3.0, 0.0, 1.0, false);
    averageStretch = ofMap(averageStretch, -1.0, 3.5, 0.0, 1.0, false);

    return averageStretch;
}