#pragma once
#include "ofMain.h"
#include "Molecule.h"

class molecularSystem{

	public:

        

		molecularSystem();
        void setup(int num);
        void update();
        void draw();

        // Molecule myMolecules;
        vector <Molecule> molecules;


    private:

};