#pragma once
#include "ofMain.h"
#include "molecularSystem.h"


enum class cellType{
	BREATHER = 0,
	PUMPER
};

class Cell{

	public:

		Cell();
        Cell(molecularSystem * system, cellType myType);

        void set(int num, int x, int y);
        void grow();
        void inflate();
        void update();
        void draw();
        float getInflation();


        molecularSystem * systemPtr;

        vector <Molecule *> cellMolecules;
        vector <Spring *> springs;

        int nextGrowth;
        int maxGrowth;
        int uniqueVal;

        cellType subType;
        bool mature;
        


    private:

};


class Neuron{

	public:

		Neuron();
        Neuron(molecularSystem * system);

        void set(int arms, int elements, int x, int y);
        void connect(Molecule * myMolecule, Molecule * otherMolecule);
        void grow();
        void signal();
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

        bool isSignaling;
        int nextSignal;
        int startTime;
        glm::vec2 signalPos;
        vector <Molecule *> signalFlow;

        bool mature;
        


    private:

};


class Intestine{

	public:

		Intestine();
        Intestine(molecularSystem * system);

        void set(int num, int x, int y);
        void grow();
        void digest();
        void update();
        void draw();
        // float getInflation();


        molecularSystem * systemPtr;

        vector <Molecule *> intestineMolecules;
        vector <Spring *> springs;
        // vector <Spring *> hiddenSprings;
        int nextGrowth;

        
        bool isDigesting;
        int nextDigestion;
        int startTime;
        glm::vec2 digestionPos;
        // vector <Molecule *> signalFlow;

        bool mature;


    private:

};