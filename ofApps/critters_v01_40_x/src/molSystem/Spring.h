#pragma once
#include "ofMain.h"
#include "Molecule.h"

class molecularSystem;  // if omitted -> makes error "unknown type name 'molecularSystem'" ???

enum springType{
	DFLT = 0,
    JOINT,
	MEMBRANE,
	STRUCTURE,
    DENDRITE
};

class Spring {

	public:

		Spring();
        Spring(molecularSystem * system);
        Spring(molecularSystem * system, springType myType);

        void setup();
        void connect(Molecule * molA, Molecule * molB);
        void disconnect();
        void update();
        void draw(float resMultiplier);

        molecularSystem * systemPtr;
        

    // private:

        glm::vec2 limitVec(glm::vec2 inVec, float maxFrc);

        Molecule * moleculeA;
        Molecule * moleculeB;
        float restLength;
        float currentLength;
        float k;    // stiffness constant
        float stretch;
        glm::vec2 normal;

        springType type;
        bool removeMe = false;
   
};