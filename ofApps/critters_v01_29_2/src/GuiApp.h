#pragma once

#include "ofMain.h"
#include "ofxGui.h"
// #include "ofApp.h"
#include "ofxPDSP.h"


class ofApp;



class GuiApp: public ofBaseApp {
public:
	void setup();
	void setSimulationGUI();
	void setSynthGUI();
	void update();
	void draw();
	void savePreset(int panel);
	void loadPreset(string filePath, int panel);
	void keyPressed(int key);
	void dragEvent(ofDragInfo dragInfo);


	void maskListener(float & val);


	ofxPanel gui;
	ofParameterGroup guiTune;
	
	ofParameterGroup guiCanvas;
	ofParameterGroup guiGeneral;
	


	ofParameter<float> tuneFriction;

	ofParameter<float> tuneRepulsionThresh;
	ofParameter<float> tuneRepulsionForce;

	ofParameter<float> tuneFlatThresh;
	ofParameter<float> tuneFlatDistance;
	ofParameter<float> tuneFlatLimitForce;

	ofParameter<float> tuneSpringLength;
	ofParameter<float> tuneSpringElasticity;
	ofParameter<float> tuneSpringLimitForce;

	ofParameter<bool>  switchOscillation;
	ofParameter<float> tuneExpansionForce;
	ofParameter<float> tuneExpansionRadius;
	ofParameter<float> tuneOscillationAmount;

	ofParameter<float> tuneIntrusionThresh;
	ofParameter<float> tuneIntrusionForce;

	ofParameter<bool> switchConnections;

	// BREATHER SHAPE
	ofParameterGroup guiBreather;
	ofParameter<float> tuneBreatherExpansionForce;
	ofParameter<float> tuneBreatherExpansionRadius;
	ofParameter<float> tuneBreatherOscillationAmount;
	ofParameter<float> tuneJointLength;
	ofParameter<float> tuneJointElasticity;
	ofParameter<float> tuneJointLimitForce;

	// INTESTINE SHAPE
	ofParameterGroup guiIntestine;
	ofParameter<float> tuneMembraneLength;
	ofParameter<float> tuneMembraneElasticity;
	ofParameter<float> tuneMembraneLimitForce;
	ofParameter<float> tuneStructureLength;
	ofParameter<float> tuneStructureElasticity;
	ofParameter<float> tuneStructureLimitForce;
	ofParameter<ofColor> membraneColor;
	


	ofParameter<bool> switchKinectCalibration;
	ofParameter<bool> switchScreenMask;


	// TIMINGS
	ofParameterGroup guiTimings;
	ofParameterGroup guiTimingsCells;
	ofParameter<glm::vec2> cellNextGrowth;
	ofParameter<float> 	   cellFreqMultiplier;

	ofParameterGroup guiTimingsNeurons;
	ofParameter<int> neuronGrowSpeed;
	ofParameter<int> neuronSignalInterval;
	ofParameter<int> neuronSyncDistance;

	ofParameterGroup guiTimingsIntestines;
	ofParameter<int> intestineGrowInterval;
	ofParameter<int> intestineDigestionInterval;
	ofParameter<int> intestineDigestionSpeed;


	bool maskChanged;	
	// string defaultSimulationPreset;
	// string defaultSynthPreset;



	// GUI SYNTH
	ofxPanel            guiSynth;
	ofParameterGroup    guiSynthFX;
	pdsp::ParameterGain masterGain;

	ofParameterGroup    guiSynthBrthrs;
	pdsp::Parameter		brthGain;
	pdsp::Parameter     brthFineTune;
	pdsp::Parameter     brthOscMix;
	pdsp::Parameter     brthPw;
	pdsp::Parameter     brthPwmSpeed;
	pdsp::ParameterAmp  brthPwmAmt;  
	pdsp::Parameter     brthCutoff;
	pdsp::Parameter     brthReso;	

	ofParameterGroup    guiSynthPumpers;
	pdsp::Parameter        pmprGainControl;  
	pdsp::Parameter        pmprPitchControl;    
	pdsp::Parameter        pmprPEnvAmtControl;
	pdsp::Parameter        pmprPEnvAttackControl;
	pdsp::Parameter        pmprPEnvReleaseControl;
	pdsp::Parameter        pmprVeloToEnv;
	pdsp::Parameter        pmprFbControl;
	pdsp::Parameter        pmprFbModControl;
	pdsp::Parameter        pmprEnvAttackControl;
	pdsp::Parameter        pmprEnvHoldControl;
	pdsp::Parameter        pmprEnvReleaseControl;
	
	ofParameterGroup    guiSynthPumpersImpulse;
	pdsp::Parameter        pmprImpulseEnvAttack;
	pdsp::Parameter        pmprImpulseEnvHold;
	pdsp::Parameter        pmprImpulseEnvRelease;
	ofParameter<float> 	   pmprImpulseAmt;

	ofParameterGroup    guiSynthNeurons;
    pdsp::Parameter		   nronGain;
	pdsp::Parameter		   nronEnvAttack;
    pdsp::Parameter		   nronEnvRelease; 
    pdsp::Parameter		   nronPitch;
    pdsp::Parameter		   nronFineTune;
	pdsp::Parameter		   nronOscMix;
    pdsp::Parameter		   nronCutoff;
	pdsp::Parameter		   nronReso;
	ofParameter<float> 	   nronFilterModAmt;
	pdsp::Parameter		   nronFEnvAttack;
    pdsp::Parameter		   nronFEnvHold;
	ofParameter<int> 	   nronSignalSpeed;
    
	ofParameterGroup    guiSynthIntestines;
    pdsp::Parameter		   itstGain;
	pdsp::Parameter		   itstEnvAttack;
	pdsp::Parameter		   itstEnvDecay; 
	pdsp::Parameter		   itstEnvSustain; 
    pdsp::Parameter		   itstEnvRelease; 
    pdsp::Parameter		   itstPitch;
    pdsp::Parameter		   itstFineTune;
	pdsp::Parameter        itstPw;
	pdsp::Parameter        itstPwmAmt;  
	pdsp::Parameter        itstLfoRate;
	pdsp::Parameter		   itstOscMix;
    pdsp::Parameter		   itstCutoff;
	pdsp::Parameter		   itstReso;
	pdsp::Parameter		   itstFilterModAmt;
	// ofParameter<float> 	   itstFilterModAmt;
	// pdsp::Parameter		   itstFEnvAttack;
    // pdsp::Parameter		   itstFEnvHold;
	// ofParameter<int> 	   itstSignalSpeed;

	// pdsp::Parameter		   reverbTime;
	// pdsp::Parameter		   reverbDensity;
	// pdsp::Parameter		   reverbDamping;
	


	bool debugMode;







	shared_ptr<ofApp> mainPtr;



		
};

