#include "Molecule.h"
#include "molecularSystem.h"
#include "ofApp.h"

//------------------------------------------------------------------
Molecule::Molecule() {
    systemPtr = NULL;
	cellPtr = NULL;
	neuronPtr = NULL;

}

Molecule::Molecule(molecularSystem * system) {

	Molecule();
	this->systemPtr = system;
	type = moleculeType::LIQUID;
	
}

Molecule::Molecule(molecularSystem * system, Cell * myCell) {

	Molecule();
	this->systemPtr = system;
	this->cellPtr = myCell;
	type = moleculeType::CELL;
	
}

Molecule::Molecule(molecularSystem * system, Pumper * myPumper) {

	Molecule();
	this->systemPtr = system;
	this->pumperPtr = myPumper;
	type = moleculeType::CELL;
	
}

Molecule::Molecule(molecularSystem * system, Neuron * myNeuron) {

	Molecule();
	this->systemPtr = system;
	this->neuronPtr = myNeuron;
	type = moleculeType::NEURON;
	
}

Molecule::Molecule(molecularSystem * system, Intestine * myIntestine) {

	Molecule();
	this->systemPtr = system;
	this->intestinePtr = myIntestine;
	type = moleculeType::INTESTINE;
	
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
	uniqueVal = ofRandom(-10000, 10000);

	debug = false;
	debugVector  = glm::vec2(0.0, 0.0);
	debugVector2 = glm::vec2(0.0, 0.0);
	debugVector3 = glm::vec2(0.0, 0.0);

}

//------------------------------------------------------------------
void Molecule::update() {

	friction = guiPtr->tuneFriction;	// GUI Debug

	if(guiPtr->switchConnections) { searchConnection(); }


    force = glm::vec2(0,0);

    force += repulsion();

	force += interference();

	// force += expansion();

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

    

	if (bondings.size() > 1 && type == moleculeType::CELL) {
		if(systemPtr->debugView) {
			ofFill();
			ofSetColor(208, 255, 63);
			// ofDrawCircle(position.x, position.y, 3.0);
			// ofSetColor(255, 0, 0);
			// ofDrawLine(0.0 ,0.0 , debugVector.x, debugVector.y);
			// ofSetColor(ofColor::indianRed);
			// ofDrawCircle(debugVector.x, debugVector.y, 3.0);
			// ofDrawLine(position.x ,position.y , debugVector2.x, debugVector2.y);
		}
	} else if (type == moleculeType::NEURON && bondings.size() == 1) {
		if(systemPtr->debugView) {
			ofSetColor(ofColor::hotPink);
			ofDrawLine(position.x ,position.y , debugVector2.x, debugVector2.y);
		}

	} else if (type == moleculeType::INTESTINE) {
		ofSetColor(ofColor::mediumVioletRed);
		ofDrawCircle(position.x, position.y, 3.0);

	} else if (type == moleculeType::LIQUID) {
		// ofSetColor(0, 10, 130);
		ofFill();
		// ofSetColor(ofColor::deepSkyBlue);
		// ofSetColor(ofColor::mediumSeaGreen);

		ofColor col(ofColor::mediumSeaGreen);
		if(debug){
			// col = ofColor::red;
			// ofSetColor(col);
			// ofSetLineWidth(1);
			// ofDrawLine(position, (position + debugVector*50 ));
			// ofSetColor(0, 255, 0);
			// ofDrawLine(position, (position + debugVector2*50 ));
			// ofSetColor(255, 255, 0);
			// ofDrawLine(position, (position + debugVector3*50 ));
		} else {
			col = ofColor::mediumSeaGreen;
		}
		// // ofColor col = (debug) ? ofColor::red : ofColor::mediumSeaGreen;
		// ofSetColor(col);
		// ofSetHexColor(0x481f82);
		ofSetHexColor(0x1b9080);

		// ofSetColor(ofColor::darkBlue);
    	ofDrawCircle(position.x, position.y, 2.6);

	}

}




//------------------------------------------------------------------
glm::vec2 Molecule::repulsion() {

    glm::vec2 repulsionForce(0,0);

	for (int i = 0; i < systemPtr->allMolecules.size(); i++) {
		
		Molecule * other = systemPtr->allMolecules[i];
        glm::vec2 newForce = other->position - position;
        float distance = glm::length2(newForce);
		// float threshold = 10.0;
		float threshold = guiPtr->tuneRepulsionThresh;


		// don't (or only very slightly) apply the repulsion force, if the checked Molecule is one connected with me via a bonding 
		float bondingEnergy = 1.0;
		if(bondings.size() > 0) {	//   && cellPtr != NULL
			for (int j = 0; j < bondings.size(); j++) { 
				if(other->position == bondings[j]->position) {
					// threshold *= 0.5;
					bondingEnergy = 0.1;
					break;
				}
			}
		}


		threshold *= bondingEnergy;

        if( distance > 0 && distance < threshold*threshold ){	// check also for > 0 because if its 0 then its checking against itself (because we do not exclude itself from the moleculeSystem pointer)
			
			distance = sqrt(distance);
			newForce = glm::normalize(newForce);
			newForce *= ofMap(distance, 0.0, threshold, 1.0, 0.0);	// inverse the force -> the closer the molecules are together the stronger the repulsion

			// repulsionForce += -newForce; //notice the frc is negative 
			repulsionForce += -newForce * powf(2, guiPtr->tuneRepulsionForce) * bondingEnergy;
		}
	}


    return repulsionForce;
}


//------------------------------------------------------------------
glm::vec2 Molecule::interference() {

    glm::vec2 repulsionForce(0,0);

	if ( systemPtr->intrusionPoints.size() > 0 ) {

		for (int i = 0; i < systemPtr->intrusionPoints.size(); i++) {
		
			// glm::vec2 interferencePoint = systemPtr->intrusionPoints[0];
			glm::vec2 newForce = systemPtr->intrusionPoints[i] - position;
			float distance = glm::length2(newForce);
			// float threshold = 10.0;
			float threshold = guiPtr->tuneIntrusionThresh;

			if( distance > 0 && distance < threshold*threshold ){	// check also for > 0 because if its 0 then its checking against itself (because we do not exclude itself from the moleculeSystem pointer)

				distance = sqrt(distance);
				newForce = glm::normalize(newForce);
				newForce *= ofMap(distance, 0.0, threshold, 1.0, 0.0);	// inverse the force -> the closer the molecules are together the stronger the repulsion

				// repulsionForce += -newForce; //notice the frc is negative 
				repulsionForce += -newForce * powf(2, guiPtr->tuneIntrusionForce);
			}

		}
	
	}

    return repulsionForce;
}


//------------------------------------------------------------------
glm::vec2 Molecule::flattening() {
	glm::vec2 flatteningForce(0,0);
	// flatteningForce = glm::vec2(0,0);

	// if (bondings.size() > 1 && cellPtr != NULL) {
	
	if (bondings.size() == 2) {			// if the middle joint of the Neurons is included (bondings >= 2) the Neurons tend to have a X shape. Without flattening of the middle joint they tend to a Y shape

		glm::vec2 targetPosition(0,0);
		for (int i = 0; i < bondings.size(); i++) {
			targetPosition += bondings[i]->position;
		}
		targetPosition /= bondings.size();

		// float distance = glm::length(targetPosition);
		// targetPosition = glm::normalize(targetPosition);
		// targetPosition *= distance * 0.5;

		// glm::vec2 targetPosition = bondings[0]->position + bondings[1]->position;
		// targetPosition *= 0.5;
		// debugVector = targetPosition;

		flatteningForce =  targetPosition - position;
		// flatteningForce *= -1;
		// debugVector = flatteningForce;
		// ofLogNotice(ofToString(glm::length(debugVector)));

		float distance = glm::length2(flatteningForce);
		float thresholdDistance = guiPtr->tuneFlatThresh;

		if (distance > thresholdDistance*thresholdDistance) {
			// toDo make force relative to distance to target position
			distance = sqrt(distance);
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
void Molecule::searchConnection() { 

	// if Me is part of a Neuron AND Me only has 1 bonding (-> end Molecules of a Neuron arm)	AND my Neuron is mature (full grown)
	if(type == moleculeType::NEURON  &&  bondings.size() == 1 && neuronPtr->mature) {		// && bondings[0]->bondings.size() < 3	-> to check if Me is not the end of a very virgin Dendrite

		bool found = false;
		debugVector2 = position;
		// debugVector2 = glm::vec2(0, 0);

		// check against all Neurons in the system
		for (int i = 0; i < systemPtr->neurons.size(); i++) {

			// excluding my own Neuron
			if (systemPtr->neurons[i] != neuronPtr) {

				// check against all Molecules in that Neuron
				for (int j = 0; j < systemPtr->neurons[i]->neuronMolecules.size(); j++) {

					int numBondings = systemPtr->neurons[i]->neuronMolecules[j]->bondings.size();
					
					// check if the Molecule is either an end Molecule of an arm (bondings = 1) or a middle joint (bondings = 3) and it is not Myself (!)
					if ( (numBondings == 1) && systemPtr->neurons[i]->mature && systemPtr->neurons[i]->neuronMolecules[j]->position != position) {	// || numBondings == 3   -> to include middle joints

						Molecule * other = systemPtr->neurons[i]->neuronMolecules.at(j);
						glm::vec2 newForce = other->position - position;
						float distance = glm::length2(newForce);					
						// float thresholdRadius = (numBondings == 3) ? 25.0 : 35.0;
						float thresholdRadius = guiPtr->tuneRepulsionThresh + 5;
						thresholdRadius *= thresholdRadius;

						// if this is all true and the other Molecule is within a closer reach -> connect us!
						if(distance < thresholdRadius && !found) {

							debugVector2 = other->position;
							found = true;
							neuronPtr->connect(this, other);
							break;			

						} 
					}
				}
			}

			if (found) { break; }
		}
	}
}


glm::vec2 Molecule::gravity() {
    
    // ADDING GRAVITY
    glm::vec2 gravityForce;


	// add some very slight horizontal movement to simulate Brownian Molecular Motion
	float brownianMotion = ofSignedNoise(uniqueVal, position.x * 0.006, ofGetElapsedTimef()*0.2) * 0.005;
	float gravity = 0.0;

	if(type == moleculeType::LIQUID) {
		// make the liquid molecules (or suspended solid particles) to differ a bit in their weight to look less homogeneous
		gravity = uniqueVal * 0.000001;
		gravity += 0.02;
		if(systemPtr->flush) gravity += 0.05;
	} else {
		// make the organisms a bit lighter than the floating particles
		gravity += 0.01;
		// gravity = glm::vec2(0.02, 0);
		if(systemPtr->flush) gravity += 0.05;
	}

	gravityForce.x = gravity;
	gravityForce.y = brownianMotion;

	// return glm::vec2(0.02, 0.02);
	// return glm::vec2(0.02, 0);
    // return glm::vec2(0, 0.02);
	return gravityForce;
}
 



//------------------------------------------------------------------
void Molecule::addForce(glm::vec2 frc) {
	outerForce += frc;
}

//------------------------------------------------------------------
void Molecule::addBonding(Molecule * bondedMolecule) {
	bondings.push_back(bondedMolecule);
}

//------------------------------------------------------------------
void Molecule::removeBonding(Molecule * bondedMolecule) {

	for (int i = 0; i < bondings.size(); i++) {
		if (bondings[i] == bondedMolecule) {
			// delete bondings[i];
			bondings.erase( bondings.begin() + i);
			break;
		}
	}

}


//------------------------------------------------------------------
void Molecule::checkBounds() {

    // if( position.x > systemPtr->worldSize.x*0.5 ){
	// 	position.x = systemPtr->worldSize.x*0.5;
	// 	velocity.x *= -1.0;
	// }else if( position.x < -systemPtr->worldSize.x*0.5 ){
	// 	position.x = -systemPtr->worldSize.x*0.5;
	// 	velocity.x *= -1.0;
	// }
	// if( position.y > systemPtr->worldSize.y*0.5 ){
	// 	position.y = systemPtr->worldSize.y*0.5;
	// 	velocity.y *= -1.0;
	// 	// debug = true;
	// }
	// else if( position.y < -systemPtr->worldSize.y*0.5 ){
	// 	position.y = -systemPtr->worldSize.y*0.5;
	// 	velocity.y *= -1.0;
	// }	
	// else {
	// 	// debug = false;
	// }


	// get the signed distance field from the desired shape
	float d = signedDistanceField(position);

	// if(d > -0.1 && d <= 0.0) { 
	// if(d > 10.0) {
	// 	velocity = -estimateNormal(position) * 5.0;
	// }
	if(d > 0.0) { 
		// debug = true;

		// compute the reflection vector based on the incidence vector and the normal (which depends on the shape -> via derivation of the sdf function)
		glm::vec2 incidence = glm::normalize(velocity) * -1;	// the incidence vector is the velocity only reversed
		glm::vec2 normal    = -estimateNormal(position);		// the negative normal, as we want to stay INSIDE the shape - so the normal turned to the inside
		// incidence = glm::normalize(incidence);
		float dot = glm::dot(normal, incidence);
		glm::vec2 reflection = 2 * normal * dot - incidence;	// the FORMULA to finally calculate the reflection vector

		// move the Molecule again outside of the threshold distance to not being caught in a trigger loop
		// we do so by moving the Molecule slightly along the normal (which is the most direct way away from the shape)
			// the factor is arbitrary - but greater than a certain threshold, below which the Molecule being kept pushed into the outside of the shape
			// but as small as possible, as with greater values the Molecules keep bouncing on the border
		position = position + normal * 0.5;	// * 0.25

		// then change the direction corresponding to the reflection vector but with the same velocity (same length)
		// multiplied by a value < 1.0 to slow the Molecule down - simulating a kind of energy absorption happening on impact
		velocity = glm::normalize(reflection) * glm::length(velocity) * 0.95;



		// debugVector = reflection;
		// debugVector2 = incidence;
		// debugVector3 = normal;

		// debugVector = estimateNormal(position) * (d/abs(d));
		// debugVector  = glm::normalize(velocity) - glm::normalize(2*estimateNormal(position));  // * (d/abs(d))
		// debugVector  = glm::normalize(debugVector);
		// debugVector  = glm::normalize(velocity - 2*estimateNormal(position));  // * (d/abs(d))
	} else{
		// debug = false;
		// debugVector  = glm::vec2(0.0, 0.0);
		// debugVector2 = glm::vec2(0.0, 0.0);
		// debugVector3 = glm::vec2(0.0, 0.0);
	}

}


float Molecule::signedDistanceField( glm::vec2 p)
{ 
	p.x -= systemPtr->tuneXpos;
	p.y -= systemPtr->tuneYpos;

	// float opIntersection( float d1, float d2 ) { return max(d1,d2); }
	// return sdCircle(p);
	glm::vec2 r1(systemPtr->tuneHorizontalBow, systemPtr->tuneCanvasHeight);
	glm::vec2 r2(systemPtr->tuneCanvasWidth, systemPtr->tuneVerticalBow);
	// return sdEllipse(p, r);
	// return max(sdEllipse(p, r1), sdEllipse(p, r2));
	// return max( max(sdEllipse(p, r1), sdEllipse(p, r2)), sdCircle( p) );
	float sdf = opSmoothIntersection( sdEllipse(p, r1), sdEllipse(p, r2), systemPtr->tuneEdges );
	if(systemPtr->flush) {
		// p.y -= 750.0;
		p.x -= 1250.0;
		sdf = opSmoothUnion( sdf, sdBox(p), systemPtr->tuneEdges );
	}
	// return opSmoothIntersection( sdEllipse(p, r1), sdEllipse(p, r2), guiPtr->tuneEdges );
	return sdf;
}

float Molecule::sdCircle( glm::vec2 p)
{
	float r = 230.0;
    return glm::length(p) - r;
}

float Molecule::sdEllipse( glm::vec2 p, glm::vec2 r)
{
	// glm::vec2 r(400.0, 200.0);

    p = abs(p);
    // p = max(p,(p-r).yx); // idea by oneshade
	glm::vec2 q = p-r;
	q = glm::vec2(q.y, q.x);
	p = max(p, q);
    
    float m = dot(r,r);
    float d = p.y-p.x;
    return p.x - (r.y*sqrt(m-d*d)-r.x*d) * r.x/m;
}

float Molecule::sdBox( glm::vec2 p)
{
	// glm::vec2 rad(175.0, 1000.0);
	glm::vec2 rad(1000.0, 125.0);
    p = abs(p)-rad;
    return max(p.x,p.y);
}

float Molecule::opSmoothIntersection( float d1, float d2, float k ) {
    float h = glm::clamp( 0.5 - 0.5*(d2-d1)/k, 0.0, 1.0 );
    return glm::mix( d2, d1, h ) + k*h*(1.0-h); 
}

float Molecule::opSmoothUnion( float d1, float d2, float k ) {
    float h = glm::clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return glm::mix( d2, d1, h ) - k*h*(1.0-h); 
}


glm::vec2 Molecule::estimateNormal(glm::vec2 p) // p is point_on_surface
{
	const float EPSILON = 0.0001;
    glm::vec2 n = glm::vec2(
        signedDistanceField(glm::vec2(p.x + EPSILON, p.y		   )) - signedDistanceField(glm::vec2(p.x - EPSILON, p.y		   )),
        signedDistanceField(glm::vec2(p.x, 		  p.y + EPSILON)) - signedDistanceField(glm::vec2(p.x, 		  p.y - EPSILON))
    );

    return glm::normalize(n);
}


//------------------------------------------------------------------
glm::vec2 Molecule::limitVec(glm::vec2 inVec, float maxFrc) { 
    if (glm::length2(inVec) > maxFrc*maxFrc) {
	    inVec = glm::normalize(inVec) * maxFrc;
    }
    return inVec;
}