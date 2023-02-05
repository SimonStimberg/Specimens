#pragma once
#include "ofMain.h"
#include "molecularSystem.h"

// #include "ofxPDSP.h"
#include "../../synth/audioModules.h"



enum class cellType{
	BREATHER = 0,
	PUMPER
};

class Breather{

	public:

		Breather();
        Breather(molecularSystem * system, cellType myType);

        void set(int num, int x, int y);
        void update();
        void draw();

        void grow();
        void inflate();
        void syncFrequency();
        void die();

        void  adaptArousal(float amount);
        void  adaptValence();
        void  updatePosition();
        float getInflation();
        float getVelocity();


        molecularSystem * systemPtr;

        vector <Molecule *> cellMolecules;
        vector <Spring *> springs;

        glm::vec2 position;
        float frequency;
        float initFrequency;

        int nextGrowth;
        int maxGrowth;
        int uniqueVal;

        cellType subType;
        bool mature;
        float timeOfMaturity;
        bool isDead;

        float arousal;
        float valence;

        float fmStored;
        float phaseCompensation;
        bool  cycleCheck;
        int   cycleCount;
        int   maxNumCycles;

        


        audioModule::Breather audioModule;
        pdsp::ValueControl  ampCtrl;
        pdsp::ValueControl  filterCutoff;
        pdsp::ValueControl  detune;


        glm::vec2 dbgCellCenter;

        


    private:

};
 

class Pumper{

	public:

		Pumper();
        Pumper(molecularSystem * system);

        void set(int num, int x, int y);
        void update();
        void draw();

        void grow();
        void contract();
        void die();
        void sync();

        void  adaptArousal(float amount);
        void  adaptValence(); 
        void  updatePosition();
        float getInflation();


        molecularSystem * systemPtr;

        vector <Molecule *> cellMolecules;
        vector <Spring *> springs;

        glm::vec2 position;

        int nextGrowth;
        int maxGrowth;
        int uniqueVal;

        // cellType subType;
        bool mature;
        float timeOfMaturity;
        bool isDead;

        float arousal;
        float valence;

        float fmStored;
        float phaseCompensation;
        bool  cycleCheck;
        int   cycleCount;
        int   maxNumCycles;


        audioModule::Pumper audioModule;
        pdsp::ValueControl  ampCtrl;
        pdsp::ValueControl  filterCutoff;
        pdsp::TriggerControl  trig;
        pdsp::ValueNode     impulseCtrl;
        pdsp::ValueControl  impulseVal;

        pdsp::TriggerControl  trigPhase;
        pdsp::ValueControl  setPhase;
        pdsp::ValueControl  setVelocity;

        


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
        void getSynced();
        void sync();
        void die();

        void  adaptArousal(float amount);
        void  adaptValence();
        void  updatePosition();
        float getSignalStatusSquared();
        void addConnection(Spring * c);
        void removeDeadConnections();
        

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
        int maxNumSignals;

        glm::vec2 position;

        int debugVal;

        bool mature;
        bool isDead;

        float arousal;
        float valence;

        float arousalThreshold;
        float freqDivergence;


        audioModule::Neuron audioModule;
        pdsp::TriggerControl  impulse;
        // pdsp::ValueControl  signalDuration;
        


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
        void getSynced();

        void  adaptArousal(float amount);
        void  adaptValence();
        void  updatePosition();
        float getDigestionStatus();
        // float getInflation();


        molecularSystem * systemPtr;

        vector <Molecule *> intestineMolecules;
        vector <Spring *> springs;
        // vector <Spring *> hiddenSprings;
        int nextGrowth;
        int maxElements;

        glm::vec2 position;

        
        bool isDigesting;
        int nextDigestion;
        int startTime;
        glm::vec2 digestionPos;
        // vector <Molecule *> signalFlow;

        bool mature;
        bool isDead;

        float arousal;
        float valence;
        float freqDivergence;


        audioModule::Intestine   audioModule;
        pdsp::TriggerControl     soundCtrl;
        pdsp::ValueControl       pulseRate;


    private:

};