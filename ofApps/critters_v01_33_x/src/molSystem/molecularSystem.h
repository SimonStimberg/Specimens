#pragma once
#include "ofMain.h"
#include "Molecule.h"
#include "Spring.h"
#include "organisms.h"
#include "ofxPDSP.h"



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
        float getSystemPressure();
        void cleanUp();

        void addLiquid(float x, float y);
        void addBreather(float x, float y);
        void addPumper(float x, float y);
        void addNeuron(float x, float y);
        void addIntestine(float x, float y);
        void addOrganisms(organismType type, int num);
        void addRandom(float x, float y);
        
        void setGui();
        void setIntrusionPoints(vector <glm::vec2> pts) { intrusionPoints.clear(); intrusionPoints = pts; }
        void reset(bool random);


        // vectors of pointers containing all the organism that live in this Molecular System
        vector <Molecule *> liquid;
        vector <Breather *> breathers;
        vector <Pumper *> pumpers;
        vector <Neuron *> neurons;
        vector <Intestine *> intestines;

        // a copy of every Molecule (also those within the Organisms) and Spring in the System is stored here to manage deletion
        vector <Molecule *> allMolecules;       
        vector <Spring *>   allSprings;
        vector <Spring *>   neuronConnections;


        vector <glm::vec2> intrusionPoints;     // interaction points by mouse position or Kinect data


        glm::vec2 worldSize;

        bool flush;
        int  flushTimestamp;
        bool debugView;
        bool thereAreCadavers;
        bool organismsToRemove[5]; 

        bool doNotDigest;
        int collapseThreshold;

        bool freshlySpawned;



        // audio master bus containing the stems for each organism type
        pdsp::ParameterAmp      masterBus;
        pdsp::ParameterAmp      blackhole;



        // gui parameters for the canvas shape
        ofParameter<float> tuneCanvasWidth;		
		ofParameter<float> tuneCanvasHeight;
		ofParameter<float> tuneVerticalBow;
		ofParameter<float> tuneHorizontalBow;
		ofParameter<float> tuneEdges;
		ofParameter<float> tuneXpos;
		ofParameter<float> tuneYpos;
		ofParameter<float> tuneRotation;
		ofParameterGroup gui;


        // ofApp * masterPtr;


    private:

};


