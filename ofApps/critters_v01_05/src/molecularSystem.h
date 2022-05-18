#pragma once
#include "ofMain.h"
#include "Molecule.h"
#include "Spring.h"

class molecularSystem{

	public:

        

		molecularSystem();
        void setup(int num, int width, int height);
        void update();
        void draw();

        void reset();

        vector <Molecule *> molecules;
        vector <Spring *> springs;

        glm::vec2 worldSize;


    private:

};