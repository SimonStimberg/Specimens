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

        void addMolecules(int num);
        void addCells(int num);
        void reset(bool random);


        vector <Molecule *> molecules;
        vector <vector <Molecule *> > cellMolecules;
        vector <vector <Spring *> > cellSprings;

        glm::vec2 worldSize;


    private:

};