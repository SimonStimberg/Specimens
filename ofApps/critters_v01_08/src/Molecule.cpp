#include "Molecule.h"
#include "molecularSystem.h"
#include "ofApp.h"

//------------------------------------------------------------------
Molecule::Molecule() {
    systemPtr = NULL;
}

Molecule::Molecule(molecularSystem * system) {

	Molecule();
	this->systemPtr = system;
	
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

	friction = tuneFriction;	// GUI Debug


    force = glm::vec2(0,0);

    force += repulsion();

	force += outerForce;

	force += flattening();


    velocity *= friction;

    velocity += force;

    position += velocity;

    checkBounds();


	outerForce = glm::vec2(0,0);
}


//------------------------------------------------------------------
void Molecule::draw() {

    ofSetColor(208, 255, 63);
	// ofSetColor(255);
    ofDrawCircle(position.x, position.y, 4.0);

	if (bondings.size() > 1) {
		ofSetColor(255, 0, 0);
		ofDrawLine(0.0 ,0.0 , debugVector.x, debugVector.y);
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
		float threshold = tuneRepulsionThresh;

        if( distance > 0 && distance < threshold ){	// check also for > 0 because if its 0 then its checking against itself (because we do not exclude itself from the moleculeSystem pointer)

			newForce = glm::normalize(newForce);
			newForce *= ofMap(distance, 0.0, threshold, 1.0, 0.0);	// inverse the force -> the closer the molecules are together the stronger the repulsion

			// repulsionForce += -newForce; //notice the frc is negative 
			repulsionForce += -newForce * powf(2, tuneRepulsionForce);
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
		float thresholdDistance = tuneFlatThresh;

		if (distance > thresholdDistance) {
			// toDo make force relative to distance to target position
			flatteningForce = glm::normalize(flatteningForce);
			flatteningForce *= ofMap(distance, thresholdDistance, thresholdDistance + tuneFlatDistance, 0.0, 1.0, true);	// inve

			// flatteningForce = limitVec(flatteningForce, 0.05 * powf(2.0, tuneFlatLimitForce)); // limit to a max force
			// debugVector = flatteningForce;
			
		} else {
			flatteningForce = glm::vec2(0,0);
		}
		
		// }
		
	}

	return flatteningForce;
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