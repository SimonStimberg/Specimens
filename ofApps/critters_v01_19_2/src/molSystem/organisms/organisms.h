#pragma once
#include "ofMain.h"
#include "molecularSystem.h"


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
        // vector <Spring *> hiddenSprings;
        int nextGrowth;


    private:

};