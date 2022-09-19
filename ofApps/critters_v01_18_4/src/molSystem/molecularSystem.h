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
        float getInflation();


        molecularSystem * systemPtr;

        vector <Molecule *> cellMolecules;
        vector <Spring *> springs;


    private:

};


class Neuron{

	public:

		Neuron();
        Neuron(molecularSystem * system);

        void set(int arms, int elements, int x, int y);
        void connect(Molecule * myMolecule, Molecule * otherMolecule);
        void grow();
        void update();
        void draw();

        molecularSystem * systemPtr;

        vector <Molecule *> neuronMolecules;
        vector <vector <Molecule *>> dendrites;
        vector <Spring *> springs;
        vector <Spring *> connections;
        // int numDendrites;
        int maxArmElements;
        int nextGrowth;


    private:

};


class Intestine{

	public:

		Intestine();
        Intestine(molecularSystem * system);

        void set(int num, int x, int y);
        void grow();
        void update();
        void draw();
        // float getInflation();


        molecularSystem * systemPtr;

        vector <Molecule *> intestineMolecules;
        vector <Spring *> springs;
        vector <Spring *> membraneSprings;
        int nextGrowth;


    private:

};



class molecularSystem{

	public:

        

		molecularSystem();
        void setup(int width, int height);
        void update();
        void draw();

        vector <float> getCellInflation();
        vector <int> getCellSizes();

        void addMolecules(int num);
        void addCells(int num);
        void addNeurons(int num);
        void addIntestines(int num);
        void reset(bool random);

        void setIntrusionPoints(vector <glm::vec2> pts) { intrusionPoints = pts; }


        vector <Molecule *> molecules;
        vector <Molecule *> liquid;
        // vector <Spring *> springs;
        vector <Cell *> cells;
        vector <Neuron *> neurons;
        vector <Intestine *> intestines;

        vector <glm::vec2> intrusionPoints;

        glm::vec2 worldSize;

        bool flush;
        bool debugView;


    private:

};


