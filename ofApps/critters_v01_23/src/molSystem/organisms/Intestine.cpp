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

    nextGrowth = ofGetElapsedTimeMillis() + (int)(ofRandom(1000, 2000));
    mature = false;

    isDigesting = false;
    nextDigestion = ofGetElapsedTimeMillis() + (int)(ofRandom(1000, 2000));
    pusher = glm::vec2(0, 0);

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
    digest();

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

    if (isDigesting) {
        ofSetColor(ofColor::darkSlateGrey);
        ofFill();
        ofDrawCircle(pusher, 5.);
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
    int maxElements = 70;

    if ( ofGetElapsedTimeMillis() >= nextGrowth && !isDigesting) {


        
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

    if (intestineMolecules.size() >= 20) mature = true;

}


//------------------------------------------------------------------
void Intestine::digest()
{
    
    // only digest if the time has come to
    if ( ofGetElapsedTimeMillis() >= nextDigestion && isDigesting == false && mature) {

        isDigesting = true;
        startTime = ofGetElapsedTimeMillis();
  
    }



    // if the Neuron is in the process of signaling do the following
    if (isDigesting) {

        // ofLogNotice("fire signal!");
  

        int signalSpeed = 500; // in milliseconds the signal needs for crossing one segment
        int timeIdx = ofGetElapsedTimeMillis()-startTime;   // the time used as basis for the animation
        int segmentIdx = floor(timeIdx / signalSpeed) * 2;      // the index for the Molecules in the signal path is incremented in steps defined by the signal speed

        // ofLogNotice("segIdx: " + ofToString(segmentIdx));


        // as long as we have not reached the end of the signal path: shift the point's position along the signal path
        if (segmentIdx < intestineMolecules.size() - 2) {

            // mapping the time for each segment animation
            float prop = ofMap( (timeIdx) % signalSpeed, 0, signalSpeed, 0.0f, 1.0f);   // resulting in a value between 0.0 and 1.0 for the position to interpolate between the two Molecules position
            // ofLogNotice("prop: " + ofToString(prop));

            // calculate the new position on the signal path by interpolating between the position of the two Molecules defining the actual segment of the signal path
            // signalPos = signalFlow[segmentIdx]->position * (1-prop) + signalFlow[segmentIdx + 1]->position * prop;

            pusher = (intestineMolecules[segmentIdx]->position + intestineMolecules[segmentIdx+1]->position) * 0.5 * (1-prop)  +  (intestineMolecules[segmentIdx+2]->position + intestineMolecules[segmentIdx+3]->position) * 0.5 * prop;



            float stretchFactor = 0.75;


            glm::vec2 newForce = intestineMolecules[segmentIdx]->position - intestineMolecules[segmentIdx+1]->position;
            newForce = glm::normalize(newForce) * stretchFactor;

            intestineMolecules[segmentIdx]->addForce(newForce * (1-prop));
            intestineMolecules[segmentIdx+1]->addForce(-newForce * (1-prop));


            glm::vec2 stretchFrc = intestineMolecules[segmentIdx+2]->position - intestineMolecules[segmentIdx+3]->position;
            stretchFrc = glm::normalize(stretchFrc) * stretchFactor;

            intestineMolecules[segmentIdx+2]->addForce(stretchFrc * (prop));
            intestineMolecules[segmentIdx+3]->addForce(-stretchFrc * (prop));




            // for (int i = 0; i < intestineMolecules.size(); i++) {

            //     // Molecule * other = intestineMolecules.at(i);
            //     glm::vec2 newForce = pusher - intestineMolecules[i]->position;
            //     float distance = glm::length2(newForce);    // using length2 for better performance (-> squared -> avoiding the square root operation) 

            //     float repulsionRadius = 20.0;
            //     repulsionRadius *= repulsionRadius;     // as we use length2 we have to square the radius as well

            //     if( distance > 0 && distance < repulsionRadius ){	// check also for > 0 because if its 0 then its checking against itself (because we do not exclude itself from the moleculeSystem pointer)

            //         newForce = glm::normalize(newForce);
            //         newForce *= ofMap(distance, 0.0, repulsionRadius, 1.0, 0.0);	// inverse the force -> the closer the molecules are together the stronger the repulsion

            //         newForce *= 2.;
            //         // repulsionForce += -newForce; //notice the frc is negative 
            //         // repulsionForce += -newForce * powf(2, guiPtr->tuneRepulsionForce) * bondingEnergy;

            //         intestineMolecules[i]->addForce(-newForce);
            //     }

                

            // }

        } else {    // if we have reached the end of the signal path, stop the signaling
            isDigesting = false;
            nextDigestion = ofGetElapsedTimeMillis() + (int)(ofRandom(5000, 7000));    // choose the next signal timestamp 
            // signalFlow.clear();
        }

    }




    // pusher = (intestineMolecules[2]->position + intestineMolecules[3]->position) * 0.5;

    // for (int i = 0; i < intestineMolecules.size(); i++) {

    //     // Molecule * other = intestineMolecules.at(i);
    //     glm::vec2 newForce = pusher - intestineMolecules[i]->position;
    //     float distance = glm::length2(newForce);    // using length2 for better performance (-> squared -> avoiding the square root operation) 

    //     float repulsionRadius = 20.0;
    //     repulsionRadius *= repulsionRadius;     // as we use length2 we have to square the radius as well

    //     if( distance > 0 && distance < repulsionRadius ){	// check also for > 0 because if its 0 then its checking against itself (because we do not exclude itself from the moleculeSystem pointer)

	// 		newForce = glm::normalize(newForce);
	// 		newForce *= ofMap(distance, 0.0, repulsionRadius, 1.0, 0.0);	// inverse the force -> the closer the molecules are together the stronger the repulsion

    //         newForce *= 3.;
	// 		// repulsionForce += -newForce; //notice the frc is negative 
	// 		// repulsionForce += -newForce * powf(2, guiPtr->tuneRepulsionForce) * bondingEnergy;

    //         intestineMolecules[i]->addForce(-newForce);
	// 	}

        

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