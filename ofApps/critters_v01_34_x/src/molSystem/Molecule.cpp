#include "Molecule.h"
#include "molecularSystem.h"
#include "ofApp.h"

//------------------------------------------------------------------
Molecule::Molecule() {
    systemPtr = NULL;
	breatherPtr = NULL;
	neuronPtr = NULL;

}

Molecule::Molecule(molecularSystem * system) {

	Molecule();
	this->systemPtr = system;
	type = moleculeType::LIQUID;
	
}

Molecule::Molecule(molecularSystem * system, Breather * myBreather) {

	Molecule();
	this->systemPtr = system;
	this->breatherPtr = myBreather;
	type = moleculeType::BREATHER;
	
}

Molecule::Molecule(molecularSystem * system, Pumper * myPumper) {

	Molecule();
	this->systemPtr = system;
	this->pumperPtr = myPumper;
	type = moleculeType::PUMPER;
	
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

	position.x = x;
	position.y = y;
	
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


	friction = guiPtr->tuneFriction;	// value is being aquired from the gui app

	if(type == moleculeType::NEURON) searchConnection();		// for molecules in a Neuron


	
	// CALCULATE THE DIFFERENT FORCES AND ACCUMULATE THEM

    force = glm::vec2(0,0);		// set the force vector to zero 

    force += repulsion();		// repulsion force between moleculse
	force += interference();	// interference with mouse or kinect data (same mechanic as repulsion)
	force += outerForce;		// forces that have been added from the outside, e.g. by a connected spring
	force += flattening();		// flatteing force - if the molecule is part of an organism this yields for a flat membrane
	force += gravity();			// gravity force



}



//------------------------------------------------------------------
void Molecule::applyForces() {

	velocity *= friction;		// apply friction (fluid resistance / drag)
    velocity += force;			// add force to velocity
    position += velocity;		// add velocity to position

    checkBounds();				// check for the bounderies of the screen and bounce of that boundaries


	outerForce = glm::vec2(0,0);	// reset outer force vector to zero


	if(type == moleculeType::LIQUID && (position.x > systemPtr->worldSize.x || isnan(position.x))) {
		removeMe = true;
		systemPtr->organismsToRemove[LIQUID] = true;
		systemPtr->thereAreCadavers = true;
	}

}


//------------------------------------------------------------------
void Molecule::draw() {


	// ONLY DRAW LIQUID MOLECULES
    
	ofFill();

	if (type == moleculeType::LIQUID) {
		ofFill();
		ofSetHexColor(0x1b9080);
    	ofDrawCircle(position.x, position.y, 2.6);
	}

	// OR WHEN IN DEBUG MODE

	if(systemPtr->debugView) {

		if (type == moleculeType::BREATHER) {

			// ofSetColor(208, 255, 63);
			ofColor col = ofColor::fromHex(0x2bdbe6);
			ofSetColor(col);
			ofDrawCircle(position.x, position.y, 1.5);

		} else if (type == moleculeType::NEURON && bondings.size() == 1) {

				ofSetColor(ofColor::hotPink);
				ofDrawLine(position.x ,position.y , debugVector2.x, debugVector2.y);

		} else if (type == moleculeType::INTESTINE) {

			ofSetColor(ofColor::mediumVioletRed);
			ofDrawCircle(position.x, position.y, 3.0);

		} 
	}

}



// CALCULATE REPULSION FORCE BETWEEN MOLECULES
//------------------------------------------------------------------
glm::vec2 Molecule::repulsion() {


	/* the following code is based loosly on algorithms fom THE NATURE OF CODE by Daniel Shiffman	https://natureofcode.com/   */


    glm::vec2 repulsionForce(0,0);

	float radius = guiPtr->tuneRepulsionThresh;
	// float radius = systemPtr->worldSize.x;
	
	vector<Molecule *> neighbors = systemPtr->getNeighbors(position.x, position.y, radius);

	// for (int i = 0; i < systemPtr->allMolecules.size(); i++) {
	for (int i = 0; i < neighbors.size(); i++) {
		
		// Molecule * other = systemPtr->allMolecules[i];
		Molecule * other = neighbors[i];
        glm::vec2 newForce = other->position - position;
        float distance = glm::length2(newForce);
		float threshold = radius;
		// float threshold = guiPtr->tuneRepulsionThresh;


		// don't (or only very slightly) apply the repulsion force, if the checked Molecule is one connected with me via a bonding 
		float bondingEnergy = 1.0;
		if(bondings.size() > 0) {	//   && breatherPtr != NULL
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
			newForce /= distance;	// is the same as  newForce = glm::normalize(newForce)
									// but glm::normalize() does another sqrt() calculation !!!!!
									// which we can spare by using the calculated distance in the step before
			newForce *= ofMap(distance, 0.0, threshold, 1.0, 0.0);	// inverse the force -> the closer the molecules are together the stronger the repulsion
			// newForce *= ofMap(distance, 0.0, threshold*threshold, 1.0, 0.0);

			repulsionForce += -newForce * powf(2, guiPtr->tuneRepulsionForce) * bondingEnergy;
		}
	}


    return repulsionForce;
}



// CALCULATE A REPULSION FORCE AGAINST INTERFERENCE POINTS (mouse position or Kinect data)
//------------------------------------------------------------------
glm::vec2 Molecule::interference() {

    glm::vec2 repulsionForce(0,0);

	if ( systemPtr->intrusionPoints.size() > 0 ) {

		for (int i = 0; i < systemPtr->intrusionPoints.size(); i++) {
		
			// glm::vec2 interferencePoint = systemPtr->intrusionPoints[0];
			glm::vec2 newForce = systemPtr->intrusionPoints[i] - position;
			float distance = glm::length2(newForce);
			float threshold = guiPtr->tuneIntrusionThresh;

			if( distance > 0 && distance < threshold*threshold ){	// check also for > 0 because if its 0 then its checking against itself (because we do not exclude itself from the moleculeSystem pointer)

				distance = sqrt(distance);
				// newForce = glm::normalize(newForce);
				newForce /= distance;
				distance = ofMap(distance, 0.0, threshold, 1.0, 0.0);	// inverse the force -> the closer the molecules are together the stronger the repulsion
				newForce *= distance;

				if(type == moleculeType::BREATHER)  breatherPtr->adaptArousal(distance);
				if(type == moleculeType::PUMPER)    pumperPtr->adaptArousal(distance);
				if(type == moleculeType::NEURON)    neuronPtr->adaptArousal(distance);
				if(type == moleculeType::INTESTINE) intestinePtr->adaptArousal(distance);

				repulsionForce += -newForce * powf(2, guiPtr->tuneIntrusionForce);
			}

		}
	
	}

    return repulsionForce;
}



// CALCULATE FLATTENING FORCE TO YIELD FOR FLATT MEMRANES
//------------------------------------------------------------------
glm::vec2 Molecule::flattening() {

	
	/* the following code is based on concepts by Alexander Miller (SPACEFILLER)  	https://notes.spacefiller.space/living-wall/  	*/


	glm::vec2 flatteningForce(0,0);
	
	if ( (type == moleculeType::NEURON || type == moleculeType::INTESTINE) && bondings.size() == 2) {			// if the middle joint of the Neurons is included (bondings >= 2) the Neurons tend to have a X shape. Without flattening of the middle joint they tend to a Y shape

		// calculate average position of all Molecules that this Molecule is connected to - hence, the flattest position possible between Molecules
		glm::vec2 targetPosition(0,0);
		for (int i = 0; i < bondings.size(); i++) {
			targetPosition += bondings[i]->position;
		}
		targetPosition /= bondings.size();


		flatteningForce = targetPosition - position;
		float distance = glm::length2(flatteningForce);
		float thresholdDistance = guiPtr->tuneFlatThresh;


		if (distance > thresholdDistance*thresholdDistance) {

			// toDo make force relative to distance to target position
			distance = sqrt(distance);
			// flatteningForce = glm::normalize(flatteningForce);
			flatteningForce /= distance;
			flatteningForce *= ofMap(distance, thresholdDistance, thresholdDistance + guiPtr->tuneFlatDistance, 0.0, 1.0, true);	
			
		} else {

			flatteningForce = glm::vec2(0,0);

		}
		

		
	}

	return flatteningForce;
}



// ADD GRAVITY FORCE
//------------------------------------------------------------------
glm::vec2 Molecule::gravity() {
    
    glm::vec2 gravityForce;

	glm::vec2 brownianMotion(0,0);


	// add some very slight horizontal movement to simulate Brownian Molecular Motion
	// float brownianMotion = ofSignedNoise(uniqueVal, position.x * 0.006, ofGetElapsedTimef()*0.2) * 0.005;
	brownianMotion.x = ofSignedNoise(uniqueVal, position.x * 0.006, ofGetElapsedTimef()*0.2) * 0.017;
	brownianMotion.y = ofSignedNoise(uniqueVal, position.y * 0.006, ofGetElapsedTimef()*0.2) * 0.017;
	float gravity = 0.0;


	if(type == moleculeType::LIQUID) brownianMotion * 0.75;		// less brownian motion for single liquid molecules


	// if(type == moleculeType::LIQUID) {

	// 	// make the liquid molecules (or suspended solid particles) to differ a bit in their weight to look less homogeneous
	// 	gravity = uniqueVal * 0.000001;
	gravity += 0.02;
	if(systemPtr->flush) gravity += 0.07;	// add a additional drag to the bottom, once the system collapsed

	// } else {

	// 	// make the Molecules of an organisms a bit lighter than the floating particles
	// 	gravity += 0.01;
	// 	if(systemPtr->flush) gravity += 0.05;  // add a additional drag to the bottom, once the system collapsed

	// }


	gravityForce.x = brownianMotion.x + gravity;
	gravityForce.y = brownianMotion.y;


	return gravityForce;
}



// ADD A FORCE FROM THE OUTSIDE (such as spring forces)
//------------------------------------------------------------------
void Molecule::addForce(glm::vec2 frc) {
	outerForce += frc;
}



// SEARCH FOR OTHER MOLECULES TO CONNECT (only Neuron Molecules)
//------------------------------------------------------------------
void Molecule::searchConnection() { 

	// if Me is part of a Neuron AND Me only has 1 bonding (-> end Molecules of a Neuron arm)	AND my Neuron is mature (full grown)
	if(type == moleculeType::NEURON  &&  bondings.size() == 1 && neuronPtr->mature) {		// && bondings[0]->bondings.size() < 3	-> to check if Me is not the end of a very virgin Dendrite

		bool found = false;
		debugVector2 = position;
		// debugVector2 = glm::vec2(0, 0);

		float searchRadius = guiPtr->tuneRepulsionThresh + 5;	// the search radius is the repulsion radius plus a little extra
		vector<Molecule *> neighbors = systemPtr->getNeighbors(position.x, position.y, searchRadius);

		// check against all Neurons in the system
		// for (int i = 0; i < systemPtr->neurons.size(); i++) {

		// check against my neighbors
		for (int i = 0; i < neighbors.size(); i++) {


			// check if the neighboring Molecule is part of a Neuron AND if its parent Neuron is not the same as mine AND check if my Neuron hasnt gone over the canvas into the abyss (is about to die)
			if (neighbors[i]->type == moleculeType::NEURON && neighbors[i]->neuronPtr != neuronPtr && neuronPtr->position.x < systemPtr->worldSize.x) {


			// excluding my own Neuron AND make sure neither my Neuron nor the other is over the edge of the screen (and therefore about to be deleted -> SEGMEMTATION FAULT!)
			// if (systemPtr->neurons[i] != neuronPtr && neuronPtr->position.x < systemPtr->worldSize.x && systemPtr->neurons[i]->position.x < systemPtr->worldSize.x) {

				// check against all Molecules in that Neuron
				// for (int j = 0; j < systemPtr->neurons[i]->neuronMolecules.size(); j++) {
				

				// check if the neighboring Molecule's parent Neuron has not gone into the abyss (is going to die) AND if the neighbor is an end Molecule of an arm (bondings = 1) AND its parent Neuron is mature AND if its not me! (has the same position)
				if (neighbors[i]->neuronPtr->position.x < systemPtr->worldSize.x && neighbors[i]->bondings.size() == 1 && neighbors[i]->neuronPtr->mature && neighbors[i]->position != position) {

					// int numBondings = systemPtr->neurons[i]->neuronMolecules[j]->bondings.size();
					
					// check if the Molecule is either an end Molecule of an arm (bondings = 1) or a middle joint (bondings = 3) and it is not Myself (!)
					// if ( (numBondings == 1) && systemPtr->neurons[i]->mature && systemPtr->neurons[i]->neuronMolecules[j]->position != position) {	// || numBondings == 3   -> to include middle joints

						// Molecule * other = systemPtr->neurons[i]->neuronMolecules.at(j);
						Molecule * other = neighbors[i];
						glm::vec2 newForce = other->position - position;
						float distance = glm::length2(newForce);					
						// float thresholdRadius = (numBondings == 3) ? 25.0 : 35.0;
						// float thresholdRadius = guiPtr->tuneRepulsionThresh + 5;
						float threshold = searchRadius;
						threshold *= threshold;

						// if this is all true and the other Molecule is within a closer reach -> connect us!
						if(distance < threshold && !found) {

							debugVector2 = other->position;
							found = true;
							neuronPtr->connect(this, other);
							break;			

						} 
					// }
				}
			}

			if (found) { break; }
		}
	}
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



/* the following code is based on code by Lena Gieseke (Shader Programming Workshop - Creative Technologies Master's Program)  */


// BOUNCE OF THE BOUNDARIES OF THE SCREEN
//------------------------------------------------------------------
void Molecule::checkBounds() {

	// get the signed distance field, describing the shape of the CRT screen
	float d = signedDistanceField(position);

	// if the distance is positive (hence the Molecule is OUTSIDE of the shape) bounce off that shape
	if(d > 0.0) { 

		// compute the reflection vector based on the incidence vector and the normal (which depends on the shape -> via derivation of the sdf function)
		// glm::vec2 incidence = glm::normalize(velocity) * -1;
		glm::vec2 incidence = -velocity;	// the incidence vector is the velocity only reversed
		glm::vec2 normal    = -estimateNormal(position);		// the negative normal, as we want to stay INSIDE the shape - so the normal turned to the inside

		float dot = glm::dot(normal, incidence);
		glm::vec2 reflection = 2 * normal * dot - incidence;	// the FORMULA to finally calculate the reflection vector

		// move the Molecule again outside of the threshold distance to not being caught in a trigger loop
		// we do so by moving the Molecule slightly along the normal (which is the most direct way away from the shape)
			// the factor is arbitrary - but greater than a certain threshold, below which the Molecule being kept pushed into the outside of the shape
			// but as small as possible, as with greater values the Molecules keep bouncing on the border
		// position = position + normal * 0.5;	// * 0.25
		position = position + normal * d;

		// then change the direction corresponding to the reflection vector but with the same velocity (same length)
		// multiplied by a value < 1.0 to slow the Molecule down - simulating a kind of energy absorption happening on impact
		// velocity = glm::normalize(reflection) * glm::length(velocity) * 0.95;
		velocity = reflection * 0.95;

	} 

}



// CALCULATE SIGNED DISTANCE FIELD
//------------------------------------------------------------------
float Molecule::signedDistanceField( glm::vec2 p)
{ 
	p.x -= systemPtr->tuneXpos;
	p.y -= systemPtr->tuneYpos;

	// the shape of the CRT screen is composed of the intersection area of two elippses
	// the parameters for the shape can be adjusted via the gui app with help of the screen mask
	glm::vec2 r1(systemPtr->tuneHorizontalBow, systemPtr->tuneCanvasHeight);	
	glm::vec2 r2(systemPtr->tuneCanvasWidth, systemPtr->tuneVerticalBow);		

	float sdf = opSmoothIntersection( sdEllipse(p, r1), sdEllipse(p, r2), systemPtr->tuneEdges );
	if(systemPtr->flush) {
		// p.y -= 750.0;
		p.x -= 1000.0;
		sdf = opSmoothUnion( sdf, sdBox(p), systemPtr->tuneEdges );
	}

	return sdf;
}



/* signed distance, union and intersection functions by Inigo Quilez
   https://iquilezles.org/articles/		*/

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