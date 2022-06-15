#pragma once
#include "ofMain.h"
// #include "molecularSystem.h" // -> if applied makes error in molecularSystem.h "unknown type name 'Molecule'"

class molecularSystem;  // if omitted -> makes error "unknown type name 'molecularSystem'" ???
class Cell;
class Neuron;

class Molecule {

	public:

		Molecule();
        Molecule(molecularSystem * system);
        Molecule(molecularSystem * system, Cell * myCell);
        Molecule(molecularSystem * system, Neuron * myNeuron);

        void reset(float x, float y, float velX, float velY);
        void update();
        void draw();
        void addForce(glm::vec2 frc);
        void addBonding(Molecule * bondedMolecule);

        molecularSystem * systemPtr;
        Cell * cellPtr;
        Neuron * neuronPtr;
        

    // private:

        glm::vec2 repulsion();
        glm::vec2 interference();
        glm::vec2 expansion();
        glm::vec2 flattening();
        glm::vec2 gravity();
        void checkBounds();
        float signedDistanceField( glm::vec2 p);
        float sdCircle( glm::vec2 p);
        float sdEllipse( glm::vec2 p, glm::vec2 r);
        float sdBox( glm::vec2 p);
        float opSmoothIntersection( float d1, float d2, float k );
        float opSmoothUnion( float d1, float d2, float k );
        glm::vec2 estimateNormal(glm::vec2 p);
        glm::vec2 limitVec(glm::vec2 inVec, float maxFrc);

        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec2 force;
        glm::vec2 outerForce;

        glm::vec2 debugVector;
        glm::vec2 debugVector2;
        glm::vec2 debugVector3;
        bool debug;

        vector <Molecule *> bondings;


        float friction;


};