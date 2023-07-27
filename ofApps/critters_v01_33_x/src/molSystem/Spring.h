#pragma once
#include "ofMain.h"
#include "Molecule.h"

class molecularSystem;  // if omitted -> makes error "unknown type name 'molecularSystem'" ???

enum springType{
	DFLT = 0,
    JOINT,
	MEMBRANE,
	STRUCTURE,
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
        void draw();

        molecularSystem * systemPtr;
        

    // private:

        glm::vec2 limitVec(glm::vec2 inVec, float maxFrc);

        Molecule * moleculeA;
        Molecule * moleculeB;
        float length;
        float k;    // stiffness constant
        float stretch;

        springType type;
        bool removeMe = false;
   
};