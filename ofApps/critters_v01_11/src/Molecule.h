#pragma once
#include "ofMain.h"
// #include "molecularSystem.h" // -> if applied makes error in molecularSystem.h "unknown type name 'Molecule'"

class molecularSystem;  // if omitted -> makes error "unknown type name 'molecularSystem'" ???
class Cell;

class Molecule {

	public:

		Molecule();
        Molecule(molecularSystem * system);
        Molecule(molecularSystem * system, Cell * myCell);

        void reset(float x, float y, float velX, float velY);
        void update();
        void draw();
        void addForce(glm::vec2 frc);
        void addBonding(Molecule * bondedMolecule);

        molecularSystem * systemPtr;
        Cell * cellPtr;
        

    // private:

        glm::vec2 repulsion();
        glm::vec2 expansion();
        glm::vec2 flattening();
        glm::vec2 gravity();
        void checkBounds();
        glm::vec2 limitVec(glm::vec2 inVec, float maxFrc);

        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec2 force;
        glm::vec2 outerForce;

        glm::vec2 debugVector;

        vector <Molecule *> bondings;


        float friction;
        

};