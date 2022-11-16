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
        void update();
        void draw();

        void grow();
        void inflate();
        void die();

        float getInflation();


        molecularSystem * systemPtr;

        vector <Molecule *> cellMolecules;
        vector <Spring *> springs;

        int nextGrowth;
        int maxGrowth;
        int uniqueVal;

        cellType subType;
        bool mature;
        float timeOfMaturity;
        bool isDead;
        


    private:

};


class Neuron{

	public:

		Neuron();
        Neuron(molecularSystem * system);

        void set(int arms, int elements, int x, int y);
        void update();
        void draw();

        void grow();
        void connect(Molecule * myMolecule, Molecule * otherMolecule);
        void signal();
        void die();
        

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
        int signalCounter;

        int debugVal;

        bool mature;
        bool isDead;
        


    private:

};


class Intestine{

	public:

		Intestine();
        Intestine(molecularSystem * system);

        void set(int num, int x, int y);
        void update();
        void draw();
        
        void grow();
        void digest();
        void die();

        float getDigestionStatus();
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
        bool isDead;


    private:

};