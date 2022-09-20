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
    setup();
    this->systemPtr = system;
    type = DFLT;
}

Spring::Spring(molecularSystem *system, springType myType)
{

    Spring();
    setup();
    this->systemPtr = system;
    type = myType;
}


//------------------------------------------------------------------
void Spring::setup()
{
    length = 30.0;
    k = 0.005;
    stretch = 0.0;  
}


//------------------------------------------------------------------
void Spring::connect(Molecule * molA, Molecule * molB)
{

    moleculeA = molA;
    moleculeB = molB;

    molA->addBonding(molB);
    molB->addBonding(molA);

}

//------------------------------------------------------------------
void Spring::disconnect()
{

    moleculeA->removeBonding(moleculeB);
    moleculeB->removeBonding(moleculeA);

    moleculeA = NULL;
    moleculeB = NULL;

    ofLogNotice("Disconnected!");
    if (moleculeA != NULL && moleculeB != NULL) {
        ofLogNotice("molA is not Null!");
    }

}

//------------------------------------------------------------------
void Spring::update()
{
    if (moleculeA != NULL && moleculeB != NULL) {

        float tuneElasticity = 0;
        float tuneFrcLimit = 0;
        
        if (type == DFLT) {

            length = guiPtr->tuneSpringLength;
            tuneElasticity = guiPtr->tuneSpringElasticity;
            tuneFrcLimit = guiPtr->tuneSpringLimitForce;

        } else if (type == MEMBRANE) {

            length = guiPtr->tuneMembraneLength;
            tuneElasticity = guiPtr->tuneMembraneElasticity;
            tuneFrcLimit = guiPtr->tuneMembraneLimitForce;

        } else if (type == STRUCTURE) {

            length = guiPtr->tuneStructureLength;
            tuneElasticity = guiPtr->tuneStructureElasticity;
            tuneFrcLimit = guiPtr->tuneStructureLimitForce;

        }


        
        k = 0.9 * powf(2, tuneElasticity);        // 0.01

        glm::vec2 force = moleculeA->position - moleculeB->position;

        stretch = glm::length(force) - length;    // Calculate the displacement between distance and rest length.

        force = glm::normalize(force);
        force *= -1 * k * stretch;

        force = limitVec(force, 0.1 * powf(2, tuneFrcLimit)); // limit to a max force

        moleculeA->addForce(force);

        force *= -1;
        moleculeB->addForce(force);

    }

}

//------------------------------------------------------------------
void Spring::draw()
{
    if (!(moleculeA == NULL || moleculeB == NULL)) {

        // ofSetColor(63, 255, 208);
        // ofSetColor(ofColor::indianRed);
        // ofSetColor(ofColor::darkSlateGrey);
        // ofSetColor(50);
        // ofSetColor(0);
        ofSetLineWidth(3);
        ofDrawLine(moleculeA->position, moleculeB->position);
    }
}

//------------------------------------------------------------------
glm::vec2 Spring::limitVec(glm::vec2 inVec, float maxFrc) { 
    if (glm::length(inVec) > maxFrc) {
	    inVec = glm::normalize(inVec) * maxFrc;
    }
    return inVec;
}