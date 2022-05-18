#include "Spring.h"
#include "molecularSystem.h"
#include "ofApp.h"

//------------------------------------------------------------------
Spring::Spring()
{
    systemPtr = NULL;
}

Spring::Spring(molecularSystem *system)
{

    Spring();
    this->systemPtr = system;
}

//------------------------------------------------------------------
void Spring::reset(Molecule * molA, Molecule * molB)
{

    moleculeA = molA;
    moleculeB = molB;

    length = 30.0;
    k = 0.005;
    stretch = 0.0;
}

//------------------------------------------------------------------
void Spring::update()
{
    length = guiPtr->tuneSpringLength;
    k = 0.01 * powf(2, guiPtr->tuneSpringElasticity);

    glm::vec2 force = moleculeA->position - moleculeB->position;

    stretch = glm::length(force) - length;    // Calculate the displacement between distance and rest length.

    force = glm::normalize(force);
    force *= -1 * k * stretch;

    force = limitVec(force, 0.1 * powf(2, guiPtr->tuneSpringLimitForce)); // limit to a max force

    moleculeA->addForce(force);

    force *= -1;
    moleculeB->addForce(force);

}

//------------------------------------------------------------------
void Spring::draw()
{

    ofSetColor(63, 255, 208);
    // ofSetColor(0);
    ofSetLineWidth(6);
    ofDrawLine(moleculeA->position, moleculeB->position);
}

//------------------------------------------------------------------
glm::vec2 Spring::limitVec(glm::vec2 inVec, float maxFrc) { 
    if (glm::length(inVec) > maxFrc) {
	    inVec = glm::normalize(inVec) * maxFrc;
    }
    return inVec;
}