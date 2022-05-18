#include "Spring.h"
#include "molecularSystem.h"

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
}

//------------------------------------------------------------------
void Spring::update()
{

    glm::vec2 force = moleculeA->position - moleculeB->position;

    float stretch = glm::length(force) - length;    // Calculate the displacement between distance and rest length.

    force = glm::normalize(force);
    force *= -1 * k * stretch;

    force = limitVec(force, 0.05); // limit to a max force

    moleculeA->addForce(force);

    force *= -1;
    moleculeB->addForce(force);

}

//------------------------------------------------------------------
void Spring::draw()
{

    ofSetColor(63, 255, 208);
    // ofSetColor(255);
    ofDrawLine(moleculeA->position, moleculeB->position);
}

//------------------------------------------------------------------
glm::vec2 Spring::limitVec(glm::vec2 inVec, float maxFrc) { 
    if (glm::length(inVec) > maxFrc) {
	    inVec = glm::normalize(inVec) * maxFrc;
    }
    return inVec;
}