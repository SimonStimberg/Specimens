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

	debug = false;
	debugVector  = glm::vec2(0.0, 0.0);
	debugVector2 = glm::vec2(0.0, 0.0);
	debugVector3 = glm::vec2(0.0, 0.0);

}

//------------------------------------------------------------------
void Molecule::update() {

	friction = guiPtr->tuneFriction;	// GUI Debug


    force = glm::vec2(0,0);

    force += repulsion();

	force += interference();

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
		ofSetColor(col);
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
glm::vec2 Molecule::interference() {

    glm::vec2 repulsionForce(0,0);
		
	glm::vec2 interferencePoint = systemPtr->intrusionPoints[0];
	glm::vec2 newForce = interferencePoint - position;
	float distance = glm::length(newForce);
	// float threshold = 10.0;
	float threshold = guiPtr->tuneIntrusionThresh;

	if( distance > 0 && distance < threshold ){	// check also for > 0 because if its 0 then its checking against itself (because we do not exclude itself from the moleculeSystem pointer)

		newForce = glm::normalize(newForce);
		newForce *= ofMap(distance, 0.0, threshold, 1.0, 0.0);	// inverse the force -> the closer the molecules are together the stronger the repulsion

		// repulsionForce += -newForce; //notice the frc is negative 
		repulsionForce += -newForce * powf(2, guiPtr->tuneIntrusionForce);
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
		expansionForce = limitVec(expansionForce, 0.2);	// limit the force
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

	// return glm::vec2(0.02, 0.02);
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
		debug = true;

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



		debugVector = reflection;
		debugVector2 = incidence;
		debugVector3 = normal;

		// debugVector = estimateNormal(position) * (d/abs(d));
		// debugVector  = glm::normalize(velocity) - glm::normalize(2*estimateNormal(position));  // * (d/abs(d))
		// debugVector  = glm::normalize(debugVector);
		// debugVector  = glm::normalize(velocity - 2*estimateNormal(position));  // * (d/abs(d))
	} else{
		debug = false;
		debugVector  = glm::vec2(0.0, 0.0);
		debugVector2 = glm::vec2(0.0, 0.0);
		debugVector3 = glm::vec2(0.0, 0.0);
	}

}


float Molecule::signedDistanceField( glm::vec2 p)
{ 

	// float opIntersection( float d1, float d2 ) { return max(d1,d2); }
	// return sdCircle(p);
	glm::vec2 r1(guiPtr->tuneHorizontalBow, guiPtr->tuneCanvasHeight);
	glm::vec2 r2(guiPtr->tuneCanvasWidth, guiPtr->tuneVerticalBow);
	// return sdEllipse(p, r);
	// return max(sdEllipse(p, r1), sdEllipse(p, r2));
	// return max( max(sdEllipse(p, r1), sdEllipse(p, r2)), sdCircle( p) );
	float sdf = opSmoothIntersection( sdEllipse(p, r1), sdEllipse(p, r2), guiPtr->tuneEdges );
	if(systemPtr->flush) {
		p.y -= 750.0;
		sdf = opSmoothUnion( sdf, sdBox(p), guiPtr->tuneEdges );
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
	glm::vec2 rad(75.0, 1000.0);
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
    if (glm::length(inVec) > maxFrc) {
	    inVec = glm::normalize(inVec) * maxFrc;
    }
    return inVec;
}