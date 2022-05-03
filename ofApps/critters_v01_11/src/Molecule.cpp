#include "Molecule.h"
#include "molecularSystem.h"
#include "ofApp.h"

//------------------------------------------------------------------
Molecule::Molecule() {
    systemPtr = NULL;
	cellPtr = NULL;
}

Molecule::Molecule(molecularSystem * system) {

	Molecule();
	this->systemPtr = system;
	
}

Molecule::Molecule(molecularSystem * system, Cell * myCell) {

	Molecule();
	this->systemPtr = system;
	this->cellPtr = myCell;
	
}

//------------------------------------------------------------------
void Molecule::reset(float x, float y, float velX, float velY){

    // position.x = ofRandom(systemPtr->worldSize.x);
	// position.y = ofRandom(systemPtr->worldSize.y);
	// position.x = ofRandom(50.0, 100.0);
	// position.y = ofRandom(50.0, 100.0);
	position.x = x;
	position.y = y;
	
	// velocity.x = ofRandom(-3.9, 3.9);
	// velocity.y = ofRandom(-3.9, 3.9);
	velocity.x = velX;
	velocity.y = velY;

    force   = glm::vec2(0,0);
	outerForce  = glm::vec2(0,0);

    friction = 0.97;

}

//------------------------------------------------------------------
void Molecule::update() {

	friction = guiPtr->tuneFriction;	// GUI Debug


    force = glm::vec2(0,0);

    force += repulsion();

	force += expansion();

	force += outerForce;

	force += flattening();

	force += gravity();


    velocity *= friction;

    velocity += force;

    position += velocity;

    checkBounds();


	outerForce = glm::vec2(0,0);
}


//------------------------------------------------------------------
void Molecule::draw() {

    

	if (bondings.size() > 1) {
		ofSetColor(208, 255, 63);
    	// ofDrawCircle(position.x, position.y, 3.0);
		// ofSetColor(255, 0, 0);
		// ofDrawLine(0.0 ,0.0 , debugVector.x, debugVector.y);
	} else {
		// ofSetColor(0, 10, 130);
		ofFill();
		// ofSetColor(ofColor::deepSkyBlue);
		ofSetColor(ofColor::mediumSeaGreen);
		// ofSetColor(ofColor::darkBlue);
    	ofDrawCircle(position.x, position.y, 3.0);

	}

}




//------------------------------------------------------------------
glm::vec2 Molecule::repulsion() {

    glm::vec2 repulsionForce(0,0);

	for (int i = 0; i < systemPtr->molecules.size(); i++) {
		
		Molecule * other = systemPtr->molecules.at(i);
        glm::vec2 newForce = other->position - position;
        float distance = glm::length(newForce);
		// float threshold = 10.0;
		float threshold = guiPtr->tuneRepulsionThresh;

        if( distance > 0 && distance < threshold ){	// check also for > 0 because if its 0 then its checking against itself (because we do not exclude itself from the moleculeSystem pointer)

			newForce = glm::normalize(newForce);
			newForce *= ofMap(distance, 0.0, threshold, 1.0, 0.0);	// inverse the force -> the closer the molecules are together the stronger the repulsion

			// repulsionForce += -newForce; //notice the frc is negative 
			repulsionForce += -newForce * powf(2, guiPtr->tuneRepulsionForce);
		}
	}


    return repulsionForce;
}



//------------------------------------------------------------------
glm::vec2 Molecule::flattening() {
	glm::vec2 flatteningForce(0,0);
	// flatteningForce = glm::vec2(0,0);

	if (bondings.size() > 1) {
		// glm::vec2 targetPosition(0,0);
		// for (int i = 0; i < bondings.size(); i++) {
		// glm::vec2 targetPosition = bondings[0]->position - bondings[1]->position;
		// float distance = glm::length(targetPosition);
		// targetPosition = glm::normalize(targetPosition);
		// targetPosition *= distance * 0.5;

		glm::vec2 targetPosition = bondings[0]->position + bondings[1]->position;
		targetPosition *= 0.5;
		// debugVector = targetPosition;

		flatteningForce =  targetPosition - position;
		// flatteningForce *= -1;
		// debugVector = flatteningForce;
		// ofLogNotice(ofToString(glm::length(debugVector)));

		float distance = glm::length(flatteningForce);
		float thresholdDistance = guiPtr->tuneFlatThresh;

		if (distance > thresholdDistance) {
			// toDo make force relative to distance to target position
			flatteningForce = glm::normalize(flatteningForce);
			flatteningForce *= ofMap(distance, thresholdDistance, thresholdDistance + guiPtr->tuneFlatDistance, 0.0, 1.0, true);	// inve

			// flatteningForce = limitVec(flatteningForce, 0.05 * powf(2.0, guiPtr->tuneFlatLimitForce)); // limit to a max force
			// debugVector = flatteningForce;
			
		} else {
			flatteningForce = glm::vec2(0,0);
		}
		
		// }
		
	}

	return flatteningForce;
}



//------------------------------------------------------------------
glm::vec2 Molecule::expansion() {

	glm::vec2 expansionForce(0,0);

	// if (bondings.size() > 1 && ofGetKeyPressed('f') ) {
	if (bondings.size() > 1 && guiPtr->switchOscillation ) {

		// get the position of the center of the cell aka the average position
		glm::vec2 averagePos(0,0);
		for (int i = 0; i < cellPtr->cellMolecules.size(); i++) { 
			Molecule * other = cellPtr->cellMolecules.at(i);
			averagePos += other->position;	// sum the positions of all cells
		}
		averagePos /= cellPtr->cellMolecules.size(); 	// get the average / dividing by the total amount


		// steer towards the average position
		glm::vec2 desiredPos = averagePos - position;
		expansionForce = desiredPos - velocity;
	
	
		// expansionForce *= 0.001;
		expansionForce = limitVec(expansionForce, 0.01);	// limit the force
		expansionForce *= -1.f;		// inverse it to steer away from the center


		// float oscillate = (sin(ofGetElapsedTimef())+1) * 0.5;  // oscillate between 0. and 1.
		float oscillate = sin(ofGetElapsedTimef()) + 0.5;	// oscillate between -0.5 and 1.5
		expansionForce *= oscillate;
    	// ofLogNotice(ofToString(oscillate));

		// ofLogNotice(ofToString(expansionForce));
	}

	return expansionForce;
}


glm::vec2 Molecule::gravity() {
    
    // ADDING GRAVITY
    // ofVec3f gravity;

    // gravity.x = 0;
    // gravity.y = 0;
    // gravity.z = -1.0;
    
    // // gravity *= (1.0 - drag);

    // // gravity.normalize();
    // gravity *= maxspeed;
    // // Steering = Desired minus Velocity
    // glm::vec3 steer = gravity - velocity;
    // float maxFrc = 0.05;
    // // steer.max(maxFrc);  // Limit to maximum steering force
    // if (glm::length(steer) > maxFrc) {
	//     steer = glm::normalize(steer) * maxFrc;
    // }

    // steer *= (1.0 - drag);

    return glm::vec2(0, 0.02);
}




//------------------------------------------------------------------
void Molecule::addForce(glm::vec2 frc) {
	outerForce = frc;
}

//------------------------------------------------------------------
void Molecule::addBonding(Molecule * bondedMolecule) {
	bondings.push_back(bondedMolecule);
}


//------------------------------------------------------------------
void Molecule::checkBounds() {

    if( position.x > systemPtr->worldSize.x*0.5 ){
		position.x = systemPtr->worldSize.x*0.5;
		velocity.x *= -1.0;
	}else if( position.x < -systemPtr->worldSize.x*0.5 ){
		position.x = -systemPtr->worldSize.x*0.5;
		velocity.x *= -1.0;
	}
	if( position.y > systemPtr->worldSize.y*0.5 ){
		position.y = systemPtr->worldSize.y*0.5;
		velocity.y *= -1.0;
	}
	else if( position.y < -systemPtr->worldSize.y*0.5 ){
		position.y = -systemPtr->worldSize.y*0.5;
		velocity.y *= -1.0;
	}	

}


//------------------------------------------------------------------
glm::vec2 Molecule::limitVec(glm::vec2 inVec, float maxFrc) { 
    if (glm::length(inVec) > maxFrc) {
	    inVec = glm::normalize(inVec) * maxFrc;
    }
    return inVec;
}