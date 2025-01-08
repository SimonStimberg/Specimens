#pragma once
#include "ofMain.h"
#include "Molecule.h"
#include "Spring.h"
#include "organisms.h"

// enum class cellType;

enum organismType{
	LIQUID = 0,
	BREATHER,
    PUMPER,
	NEURON,
    INTESTINE,
    NONE
};

class molecularSystem{

	public:

        

		molecularSystem();
        void setup(int width, int height);
        void update();
        void draw();

        vector <float> getCellInflation();
        vector <int> getCellSizes();
        void cleanUp();

        void addLiquid(float x, float y);
        void addBreather(float x, float y);
        void addPumper(float x, float y);
        void addNeuron(float x, float y);
        void addIntestine(float x, float y);
        void addOrganisms(organismType type, int num);
        void reset(bool random);
        void setGui();

        void setIntrusionPoints(vector <glm::vec2> pts) { intrusionPoints = pts; }


        vector <Molecule *> allMolecules;
        vector <Spring *>   allSprings;
        vector <Spring *>   neuronConnections;

        vector <Molecule *> liquid;
        vector <Cell *> breathers;
        vector <Cell *> pumpers;
        vector <Neuron *> neurons;
        vector <Intestine *> intestines;

        vector <glm::vec2> intrusionPoints;

        glm::vec2 worldSize;

        bool flush;
        bool debugView;
        organismType organismsToRemove;




        ofParameter<float> tuneCanvasWidth;			// [mainPtr->numScreens]
		ofParameter<float> tuneCanvasHeight;
		ofParameter<float> tuneVerticalBow;
		ofParameter<float> tuneHorizontalBow;
		ofParameter<float> tuneEdges;
		ofParameter<float> tuneXpos;
		ofParameter<float> tuneYpos;
		ofParameter<float> tuneRotation;
		ofParameterGroup gui;


    private:

};

