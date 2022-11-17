#include "organisms.h"
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

    nextGrowth = ofGetElapsedTimeMillis() + (guiPtr->intestineGrowInterval) + (int)(ofRandom(guiPtr->intestineGrowInterval*0.5));
    mature = false;
    isDead = false;

    isDigesting = false;
    nextDigestion = ofGetElapsedTimeMillis() + (guiPtr->intestineDigestionInterval) + (int)(ofRandom(guiPtr->intestineDigestionInterval*0.5));
    digestionPos = glm::vec2(0, 0);
    maxElements = 140;


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
        systemPtr->allMolecules.push_back(mA);
        systemPtr->allMolecules.push_back(mB);


        Spring *s = new Spring(systemPtr, STRUCTURE);
        s->connect(mA, mB);
        springs.push_back(s);
        systemPtr->allSprings.push_back(s);

        if (i > 0) {

            Spring *sA = new Spring(systemPtr, MEMBRANE);
            Molecule *mA0 = intestineMolecules[ intestineMolecules.size() - 4 ];
            sA->connect(mA, mA0);
            springs.push_back(sA);
            systemPtr->allSprings.push_back(sA);

            Spring *sB = new Spring(systemPtr, MEMBRANE);
            Molecule *mB0 = intestineMolecules[ intestineMolecules.size() - 3 ];
            sB->connect(mB, mB0);
            springs.push_back(sB);
            systemPtr->allSprings.push_back(sB);

        }

    }

}


//------------------------------------------------------------------
void Intestine::update()
{
    grow();
    digest();

    for (unsigned int i = 0; i < springs.size(); i++)
    {
        springs[i]->update();
    }

    for (unsigned int i = 0; i < intestineMolecules.size(); i++)
    {
        intestineMolecules[i]->update();
    }

    if (intestineMolecules.size() >= maxElements) die();

}


//------------------------------------------------------------------
void Intestine::draw()
{

    if (isDigesting) {
        // ofLogNotice("digestion status: " + ofToString(getDigestionStatus()) );

        // map the digestion status to color and size -> transformation from liquid to an egg spawning new cells
        ofColor col;
        col.setHex(0x1b9080); // Liquid color

        float brtnss = col.getBrightness();
        brtnss = ofMap( getDigestionStatus(), 0.0, 1.0, brtnss, 255.0 );
        col.setBrightness(brtnss);

        float sat = col.getSaturation();
        sat = ofMap( getDigestionStatus(), 0.0, 1.0, sat, 15.0 );
        col.setSaturation(sat);

        ofSetColor(col);
        ofFill();

        float radius = ofMap( getDigestionStatus(), 0.0, 1.0, 3.0, 5.0 );

        // ofSetColor(ofColor::darkSlateGrey);
        // ofSetColor(ofColor::white);          
        ofDrawCircle(digestionPos, radius);
    }

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
    // int maxElements = 70;

    if ( ofGetElapsedTimeMillis() >= nextGrowth && intestineMolecules.size() < maxElements * 2 && !isDigesting) {


        
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
        systemPtr->allMolecules.push_back(mA);      // and adding a copy to the vector containing totally-all-molecules in the whole System (to be considered in the force calculations like repulsion)
        intestineMolecules.push_back(mB);
        systemPtr->allMolecules.push_back(mB);


        Spring *s = new Spring(systemPtr, STRUCTURE);
        s->connect(mA, mB);
        springs.push_back(s);
        systemPtr->allSprings.push_back(s);


            Spring *sA = new Spring(systemPtr, MEMBRANE);
            sA->connect(mA, mA0);
            springs.push_back(sA);
            systemPtr->allSprings.push_back(sA);

            Spring *sB = new Spring(systemPtr, MEMBRANE);
            sB->connect(mB, mB0);
            springs.push_back(sB);
            systemPtr->allSprings.push_back(sB);


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



        nextGrowth = ofGetElapsedTimeMillis() + (int)(guiPtr->intestineGrowInterval) + (int)(ofRandom(guiPtr->intestineGrowInterval*0.5));

    }

    if (intestineMolecules.size() >= 20) mature = true;     // from a certain length its mature to start digestion

}


//------------------------------------------------------------------
void Intestine::digest()
{
    
    // only digest if the time has come to
    if ( ofGetElapsedTimeMillis() >= nextDigestion && isDigesting == false && mature) {

        isDigesting = true;
        startTime = ofGetElapsedTimeMillis();
  
    }



    // if the Intestine is in the process of digesting do the following
    if (isDigesting) {

        // ofLogNotice("fire signal!");
  

        int signalSpeed = (guiPtr->intestineDigestionSpeed); // in milliseconds the signal needs for crossing one segment
        int timeIdx = ofGetElapsedTimeMillis()-startTime;   // the time used as basis for the animation
        int segmentIdx = floor(timeIdx / signalSpeed) * 2;      // the index for the Molecules in the signal path is incremented in steps defined by the signal speed

        // ofLogNotice("segIdx: " + ofToString(segmentIdx));


        // as long as we have not reached the end of the Intestine: shift the point's position along the shape
        if (segmentIdx < intestineMolecules.size() - 2) {

            // mapping the time to the proportion of each segment for the animation
            float prop = ofMap( (timeIdx) % signalSpeed, 0, signalSpeed, 0.0f, 1.0f);   // resulting in a value between 0.0 and 1.0 for the position to interpolate between the two Molecules position
            // ofLogNotice("prop: " + ofToString(prop));


            // calculate the new position of the digested point by interpolating between the position of the two consecutive segment. each segment is the middlepoint between the to opposite Molecules that form the Intestine tunnel
            // this is used for the visualisation only
            digestionPos = (intestineMolecules[segmentIdx]->position + intestineMolecules[segmentIdx+1]->position) * 0.5 * (1-prop)  +  (intestineMolecules[segmentIdx+2]->position + intestineMolecules[segmentIdx+3]->position) * 0.5 * prop;



            // do the actual deformation of the Intestine
            float stretchFactor = 0.75;     // the length of the vector aka the force the Molecules will be pushed away from their idle position


            // displacing the first segment
            glm::vec2 stretchFrc = intestineMolecules[segmentIdx]->position - intestineMolecules[segmentIdx+1]->position;   // get the orientation of the displacement force by getting the vector that one Molecule points to the other
            stretchFrc = glm::normalize(stretchFrc) * stretchFactor;    // normalize it to get a force of 1 in that direction - then multiply it by the stretchFactor for the desired force

            intestineMolecules[segmentIdx]  ->addForce( stretchFrc * (1-prop));        // applying the displacement force proportionaly to the position
            intestineMolecules[segmentIdx+1]->addForce(-stretchFrc * (1-prop));        // do the same with the second Molecule in the opposite direction


            // displacment of the secend segment - in the same way
            stretchFrc = intestineMolecules[segmentIdx+2]->position - intestineMolecules[segmentIdx+3]->position;
            stretchFrc = glm::normalize(stretchFrc) * stretchFactor;

            intestineMolecules[segmentIdx+2]->addForce( stretchFrc * (prop));          // using the inverted proportion
            intestineMolecules[segmentIdx+3]->addForce(-stretchFrc * (prop));



        } else {    // if we have reached the end of the Intestine, stop the digestion

            isDigesting = false;
            nextDigestion = ofGetElapsedTimeMillis() + (int)(guiPtr->intestineDigestionInterval) + (int)(ofRandom(guiPtr->intestineDigestionInterval*0.5));    // choose the next signal timestamp 

            if( floor( ofRandom(2)) == 0) {
                systemPtr->addBreather(digestionPos.x, digestionPos.y);
            } else {
                systemPtr->addPumper(digestionPos.x, digestionPos.y);
            }

        }

    }

}


//------------------------------------------------------------------
void Intestine::die()
{
    
    // get the position of the center of the cell aka the average position
    // glm::vec2 center(0,0);
    for (int i = 0; i < intestineMolecules.size(); i++) { 
        intestineMolecules[i]->removeMe = true;      // mark the Molecules to be removed
        // center += intestineMolecules[i]->position;	// sum the positions of all cells
        if(i % 2 == 0) {
            glm::vec2 spawnPos = (intestineMolecules[i]->position + intestineMolecules[i+1]->position) * 0.5;
            spawnPos.x += ofRandom(-10.0, 10.0);
            spawnPos.y += ofRandom(-10.0, 10.0);
            systemPtr->addLiquid(spawnPos.x, spawnPos.y);
        }
    }
    // center /= intestineMolecules.size(); 	// get the average / dividing by the total amount

    for (int i = 0; i < springs.size(); i++) { 
        springs[i]->removeMe = true;    // mark the Springs to be removed
    }


    // int numSpawnNeurons = (subType == cellType::BREATHER) ? 5 : 3;
    // for (int i = 0; i < numSpawnNeurons; i++) { 
    //     float x = center.x + ofRandom(-10.0, 10.0);
    //     float y = center.y + ofRandom(-10.0, 10.0);
    //     systemPtr->addNeuron(x, y);
    // }
    

    isDead = true;      // mark itself to be removed
    // systemPtr->cleanUp = true;
    systemPtr->organismsToRemove = INTESTINE;

}


//------------------------------------------------------------------
float Intestine::getDigestionStatus() 
{
    int signalSpeed = (guiPtr->intestineDigestionSpeed); // in milliseconds the signal needs for crossing one segment
    int timeIdx = ofGetElapsedTimeMillis()-startTime;   // the time used as basis for the animation
    // int segmentIdx = floor(timeIdx / signalSpeed) * 2; 

    int totalTime = (intestineMolecules.size()-2) * 0.5 * signalSpeed;

    return ofMap( timeIdx, 0, totalTime, 0.0f, 1.0f); 
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