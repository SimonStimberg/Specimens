#pragma once
#include "ofMain.h"
// #include "molecularSystem.h" // -> if applied makes error in molecularSystem.h "unknown type name 'Molecule'"

class molecularSystem;  // if omitted -> makes error "unknown type name 'molecularSystem'" ???

class Molecule {

	public:

		Molecule();
        Molecule(molecularSystem * system);

        void reset();
        void update();
        void draw();
        void addForce(glm::vec2 frc);

        molecularSystem * systemPtr;
        

    // private:

        glm::vec2 repulsion();
        void checkBounds();

        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec2 force;
        glm::vec2 outerForce;


        float friction;
        

};