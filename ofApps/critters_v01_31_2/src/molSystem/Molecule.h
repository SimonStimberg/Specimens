#pragma once
#include "ofMain.h"
// #include "molecularSystem.h" // -> if applied makes error in molecularSystem.h "unknown type name 'Molecule'"

class molecularSystem;  // if omitted -> makes error "unknown type name 'molecularSystem'" ???
class Breather;
class Pumper;
class Neuron;
class Intestine;


enum class moleculeType{
	LIQUID = 0,
	BREATHER,
    PUMPER,
	NEURON,
    INTESTINE
};


class Molecule {

	public:

		Molecule();
        Molecule(molecularSystem * system);
        Molecule(molecularSystem * system, Breather * myBreather);
        Molecule(molecularSystem * system, Pumper * myPumper);
        Molecule(molecularSystem * system, Neuron * myNeuron);
        Molecule(molecularSystem * system, Intestine * myIntestine);


        void reset(float x, float y, float velX, float velY);
        void update();
        void draw();

        // force calculation functions
        glm::vec2 repulsion();
        glm::vec2 interference();
        glm::vec2 flattening();
        glm::vec2 gravity();
        void addForce(glm::vec2 frc);
        void checkBounds();

        // functions handling the connection to other Molecules (via springs)
        void addBonding(Molecule * bondedMolecule);
        void removeBonding(Molecule * bondedMolecule);
        void searchConnection();

        // helper functions for calculating a signed distance field (to describe the screen shape)
        float signedDistanceField( glm::vec2 p);
        float sdCircle( glm::vec2 p);
        float sdEllipse( glm::vec2 p, glm::vec2 r);
        float sdBox( glm::vec2 p);
        float opSmoothIntersection( float d1, float d2, float k );
        float opSmoothUnion( float d1, float d2, float k );
        glm::vec2 estimateNormal(glm::vec2 p);
        glm::vec2 limitVec(glm::vec2 inVec, float maxFrc);


        // pointers to the parent Molecular System or Organism class
        molecularSystem * systemPtr;
        Breather * breatherPtr;
        Pumper * pumperPtr;
        Neuron * neuronPtr;
        Intestine * intestinePtr;


        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec2 force;
        glm::vec2 outerForce;


        vector <Molecule *> bondings;   // pointers to the Molecules to that this Molecule is connected to via Springs

        moleculeType type;

        float friction;
        float uniqueVal;
        bool removeMe = false;


        glm::vec2 debugVector;
        glm::vec2 debugVector2;
        glm::vec2 debugVector3;
        bool debug;

};