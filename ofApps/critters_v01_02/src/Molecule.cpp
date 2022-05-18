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
void Molecule::reset(molecularSystem * system){

    position.x = ofRandomWidth();
	position.y = ofRandomHeight();
	
	velocity.x = ofRandom(-3.9, 3.9);
	velocity.y = ofRandom(-3.9, 3.9);

    force   = glm::vec2(0,0);

    friction = 0.97;

	this->systemPtr = system;
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
		Molecule other = systemPtr->molecules.at(i);

        glm::vec2 newForce = other.position - position;

        float distance = glm::length(newForce);

        if( distance > 0 && distance < 25 ){

			// if(i == 0) { ofLogNotice("newForce is: " + ofToString(newForce));}
			newForce = glm::normalize(newForce);
			
			repulsionForce += -newForce * 0.003; //notice the frc is negative 
		}
	}


    return repulsionForce;
}


//------------------------------------------------------------------
void Molecule::checkBounds() {

    if( position.x > ofGetWidth() ){
		position.x = ofGetWidth();
		velocity.x *= -1.0;
	}else if( position.x < 0 ){
		position.x = 0;
		velocity.x *= -1.0;
	}
	if( position.y > ofGetHeight() ){
		position.y = ofGetHeight();
		velocity.y *= -1.0;
	}
	else if( position.y < 0 ){
		position.y = 0;
		velocity.y *= -1.0;
	}	

}