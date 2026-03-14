#pragma once
#include "ofMain.h"
#include "Molecule.h"
#include "Spring.h"
#include "organisms.h"
// #include "ofxPDSP.h"
// #include "../synth/audioMaster.h"



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
        void setup(int width, int height, float scalingFactor, organismType species);
        // void linkAudio(audioModule::SubMaster & subMasterModule) { audioLink = &subMasterModule; }
        void update();
        void draw();

        // bin system
        void updateBins();
        vector<Molecule *> getNeighbors(float x, float y, float radius);

        float getScalingFactor() const { return scalingFactor; }
        float scaledLineWidth(float lineWidth) const { return lineWidth * scalingFactor; }
        vector <float> getCellInflation();
        vector <int> getCellSizes();
        float getSystemPressure();
        void cleanUp();
        void clampArousal(float min, float max) { arousalMin = min; arousalMax = max; }
        void evolveOrganisms(bool bEvolve) { evolve = bEvolve; }

        void addLiquid(float x, float y);
        void addBreather(float x, float y);
        void addPumper(float x, float y);
        void addNeuron(float x, float y);
        void addIntestine(float x, float y);
        void addOrganisms(organismType type, int num);
        void addRandom(float x, float y);
        void addControlledRandom(float x, float y);
        void addInitialDrop(int vesselType);
        
        void addOnNextFrame(organismType type, float x, float y);
        void addBisectedIntestine(vector<glm::vec2> positions);
        void addFromStack();
        
        void setVesselShape();
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

        // bin structure
        vector< vector<Molecule *> > bins;
        int k, xBins, yBins, binSize;


        vector <glm::vec2> intrusionPoints;     // interaction points by mouse position or Kinect data


        vector <organismType> organismsToAdd;
        vector <glm::vec2>    positionsToAdd;

        vector <vector<glm::vec2>> bisectedIntestines;


        glm::vec2 worldSize;
        organismType mySpecies;
        float scalingFactor;

        bool flush;
        bool drop;
        int  flushTimestamp;
        int  dropTimestamp;
        bool debugView;
        bool thereAreCadavers;
        bool organismsToRemove[5]; 

        bool doNotDigest;
        int collapseThreshold;

        bool freshlySpawned;
        bool evolve;
        float arousalMin;
        float arousalMax;


        // audio master bus containing the stems for each organism type
        // pdsp::ParameterAmp      masterBus;
        // pdsp::ParameterAmp      blackhole;
        // audioModule::SubMaster * audioLink;


        // vessel shape parameters (world-space pixels, computed by setVesselShape())
        float vesselCanvasWidth   = 120.0f;
		float vesselCanvasHeight  = 95.0f;
		float vesselVerticalBow   = 790.0f;
		float vesselHorizontalBow = 600.0f;
		float vesselEdges         = 25.0f;
		float vesselXpos          = 0.0f;
		float vesselYpos          = 0.0f;
		float vesselRotation      = 0.0f;


        // ofApp * masterPtr;


    private:

};


