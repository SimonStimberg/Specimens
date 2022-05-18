#pragma once
#include "ofMain.h"
#include "Molecule.h"

class molecularSystem{

	public:

        

		molecularSystem();
        void setup(int num, int width, int height);
        void update();
        void draw();

        void reset();

        vector <Molecule *> molecules;

        glm::vec2 worldSize;


    private:

};