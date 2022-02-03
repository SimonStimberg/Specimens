#include "Molecule.h"
#include "molecularSystem.h"

//------------------------------------------------------------------
Molecule::Molecule() {
    systemPtr = NULL;
}

Molecule::Molecule(molecularSystem * system) {

	Molecule();
	this->systemPtr = system;
	
}

//------------------------------------------------------------------
void Molecule::reset(){

    position.x = ofRandom(systemPtr->worldSize.x);
	position.y = ofRandom(systemPtr->worldSize.y);
	
	velocity.x = ofRandom(-3.9, 3.9);
	velocity.y = ofRandom(-3.9, 3.9);

    force   = glm::vec2(0,0);

    friction = 0.97;
}

//------------------------------------------------------------------
void Molecule::update() {

    force = glm::vec2(0,0);

    force += repulsion();


    velocity *= friction;

    velocity += force;

    position += velocity;

    checkBounds();
}


//------------------------------------------------------------------
void Molecule::draw() {

    ofSetColor(208, 255, 63);
    ofDrawCircle(position.x, position.y, 4.0);

}




//------------------------------------------------------------------
glm::vec2 Molecule::repulsion() {

    glm::vec2 repulsionForce(0,0);

	for (int i = 0; i < systemPtr->molecules.size(); i++) {
		
		Molecule * other = systemPtr->molecules.at(i);
        glm::vec2 newForce = other->position - position;
        float distance = glm::length(newForce);
		float threshold = 12.0;

        if( distance > 0 && distance < threshold ){	// check also for > 0 because if its 0 then its checking against itself (because we do not exclude itself from the moleculeSystem pointer)

			newForce = glm::normalize(newForce);
			newForce *= ofMap(distance, 0.0, threshold, 1.0, 0.0);	// inverse the force -> the closer the molecules are together the stronger the repulsion

			repulsionForce += -newForce; //notice the frc is negative 
		}
	}


    return repulsionForce;
}


//------------------------------------------------------------------
void Molecule::checkBounds() {

    if( position.x > systemPtr->worldSize.x ){
		position.x = systemPtr->worldSize.x;
		velocity.x *= -1.0;
	}else if( position.x < 0 ){
		position.x = 0;
		velocity.x *= -1.0;
	}
	if( position.y > systemPtr->worldSize.y ){
		position.y = systemPtr->worldSize.y;
		velocity.y *= -1.0;
	}
	else if( position.y < 0 ){
		position.y = 0;
		velocity.y *= -1.0;
	}	

}