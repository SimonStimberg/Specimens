#pragma once
#include "ofMain.h"
#include "Molecule.h"
#include "Spring.h"


class Cell{

	public:

        

		Cell();
        Cell(molecularSystem * system);

        void set(int num, int x, int y);
        void update();
        void draw();


        molecularSystem * systemPtr;

        vector <Molecule *> cellMolecules;
        vector <Spring *> springs;


    private:

};



class molecularSystem{

	public:

        

		molecularSystem();
        void setup(int width, int height);
        void update();
        void draw();

        void addMolecules(int num);
        void addCells(int num);
        void reset(bool random);


        vector <Molecule *> molecules;
        vector <Molecule *> liquid;
        // vector <Spring *> springs;
        vector <Cell *> cells;

        vector <glm::vec2> intrusionPoints;

        glm::vec2 worldSize;

        bool flush;
        bool debugView;


    private:

};


