#pragma once
#include "ofMain.h"
#include "molecularSystem.h"
#include "utils.h"



// THIS HEADER FILE CONTAINS ALL CLASSES OF ORGANISMS


enum class cellType{
	BREATHER = 0,
	PUMPER
};


class Breather{

	public:

		Breather();
        Breather(molecularSystem * system, cellType myType);

        void set(int num, int x, int y);
        // void linkAudioModule(audioModule::Breather & module);
        void update();
        void draw();

        void grow();
        void applyPressure();
        void syncFrequency();
        void die();

        void  adaptArousal(float amount);
        void  adaptValence();
        void  updatePosition();
        void  inflate();
        
        float calculateVolume();
        float getInflation();
        float getVelocity();

        float getSineOsc(float deltaTime);


        molecularSystem * systemPtr;

        vector <Molecule *> cellMolecules;
        vector <Spring *> springs;

        ofPolyline spline;
        ofMesh     ring;

        glm::vec2 position;
        float pressure;
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


        float sinePhase;
        float sineFrequency; // Hz

        


        // audioModule::Breather * audioModule;
        // pdsp::ValueControl  ampCtrl;
        // pdsp::ValueControl  filterCutoff;
        // pdsp::ValueControl  detune;


        glm::vec2 dbgCellCenter;

        


    private:

};
 

class Pumper{

	public:

		Pumper();
        Pumper(molecularSystem * system);

        void set(int num, int x, int y);
        // void linkAudioModule(audioModule::Pumper & module);
        void update();
        void draw();

        void grow();
        void contract();
        void applyPressure();
        void die();
        void sync();

        void  adaptArousal(float amount);
        void  adaptValence(); 
        void  updatePosition();
        float calculateVolume();
        float getInflation();


        molecularSystem * systemPtr;

        vector <Molecule *> cellMolecules;
        vector <Spring *> springs;

        ofPolyline spline;
        ofMesh     ring;

        glm::vec2 position;
        float pressure;

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


        // public to be accesible for other Organisms to sync
        float lfoPhase        = 0.0f;   // 0..1
        bool  lfoFired        = false;  // true for exactly one frame per cycle


    private:

        void updateImpulseLFO(float dt);
        void updateImpulseEnvelope(float dt);
        void fireImpulseEnvelope();

        bool  lfoActive       = false;
        float lfoFrequency    = 0.5f;   // Hz
        int   lfoTriggerCount = 0;

        enum EnvStage { ENV_IDLE, ENV_ATTACK, ENV_HOLD, ENV_RELEASE };
        EnvStage envStage  = ENV_IDLE;
        float    envValue  = 0.0f;      // 0..1 — this is impulseOut()
        float    envTimer  = 0.0f;

        float    envAttack  = 0.012f;   // seconds (50ms)
        float    envHold    = 0.214f;   // seconds (100ms)
        float    envRelease = 0.51f;   // seconds (350ms)


        // audioModule::Pumper * audioModule;
        // pdsp::ValueControl  ampCtrl;
        // pdsp::ValueControl  filterCutoff;
        // pdsp::TriggerControl  trig;
        // pdsp::ValueNode     impulseCtrl;
        // pdsp::ValueControl  impulseVal;

        // pdsp::TriggerControl  trigPhase;
        // pdsp::ValueControl  setPhase;
        // pdsp::ValueControl  setVelocity;


        int bugTrack;
        int bugTrackSub;

        


    

};


class Neuron{

	public:

		Neuron();
        Neuron(molecularSystem * system);

        void set(int arms, int elements, int x, int y);
        // void linkAudioModule(audioModule::Neuron & module);
        void update();
        void draw();
        void drawConnections();

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

        ofPolyline spline;
        ofMesh     ring;

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


        // audioModule::Neuron * audioModule;
        // pdsp::TriggerControl  impulse;
        // pdsp::ValueControl  signalDuration;
        


    private:

};


class Intestine{

	public:

		Intestine();
        Intestine(molecularSystem * system);

        void set(int num, int x, int y);
        // void linkAudioModule(audioModule::Intestine & module);
        void update();
        void draw();
        
        void grow();
        void keepShape();
        void digest();
        void bisect();
        void copyPositions(vector<glm::vec2> & positions);
        void die();
        void getSynced();

        void  adaptArousal(float amount);
        void  adaptValence();
        void  updatePosition();
        float getDigestionStatus();
        float minimumDistance2(glm::vec2 v, glm::vec2 w, glm::vec2 p);
        // float getInflation();


        molecularSystem * systemPtr;

        vector <Molecule *> intestineMolecules;
        vector <Spring *> springs;

        ofPolyline spline;
        ofMesh     ring;

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


        // audioModule::Intestine * audioModule;
        // pdsp::TriggerControl     soundCtrl;
        // pdsp::ValueControl       pulseRate;


    private:

};