#pragma once

#include "ofMain.h"
#include <string>
#include <unordered_map>
#include <variant>


class ofApp;


class GuiApp {

	public:

		using ParamValue = std::variant<int, float, bool, glm::vec2, ofColor, std::string>;

		void setup(ofApp* app);
		bool loadPreset(std::string filePath, int panel);

		const std::unordered_map<std::string, ParamValue>& getValues() const { return values; }

		template <typename T>
		bool getValue(const std::string& key, T& output) const {
			auto it = values.find(key);
			if (it == values.end()) return false;
			if (const T* value = std::get_if<T>(&it->second)) {
				output = *value;
				return true;
			}
			return false;
		}

		string defaultSimulationPreset;
		string defaultTubeCalibration;
		


		// GENERAL SIMULATION PARAMETERS
		float tuneFriction;

		float tuneRepulsionThresh;
		float tuneRepulsionForce;

		float tuneFlatThresh;
		float tuneFlatDistance;
		float tuneFlatLimitForce;

		float tuneSpringLength;
		float tuneSpringElasticity;
		float tuneSpringLimitForce;

		bool  switchOscillation;
		float tuneExpansionForce;
		float tuneExpansionRadius;
		float tuneOscillationAmount;

		float tuneIntrusionThresh;
		float tuneIntrusionForce;

		bool switchConnections;
		bool switchKinectCalibration;
		bool switchScreenMask;

		float tunePressureTest;


		// BREATHER SHAPE
		float tuneBreatherExpansionForce;
		float tuneBreatherExpansionRadius;
		float tuneBreatherOscillationAmount;
		float tuneJointLength;
		float tuneJointElasticity;
		float tuneJointLimitForce;


		// INTESTINE SHAPE
		float tuneMembraneLength;
		float tuneMembraneElasticity;
		float tuneMembraneLimitForce;
		float tuneStructureLength;
		float tuneStructureElasticity;
		float tuneStructureLimitForce;
		ofColor membraneColor;

		// ORGANISMS SHAPE
		float tuneOrganismsLineWidth;


		// TUBE CALIBRATION
		float tuneCanvasWidth;
		float tuneCanvasHeight;
		float tuneVerticalBow;
		float tuneHorizontalBow;
		float tuneEdges;
		float tuneXpos;
		float tuneYpos;
		float tuneRotation;


		// NEURON SHAPE
		float tuneDendriteLength;
		float tuneDendriteElasticity;
		float tuneDendriteLimitForce;


		// TIMINGS
		glm::vec2 cellNextGrowth;
		float cellFreqMultiplier;

		int pumperSyncDistance;
		int pumperImpulseAttack;
		int pumperImpulseHold;
		int pumperImpulseRelease;
		float pumperImpulseAmount;

		int neuronGrowSpeed;
		int neuronSignalInterval;
		int neuronSyncDistance;
		int neuronSignalSpeed;

		int intestineGrowInterval;
		int intestineDigestionInterval;
		int intestineDigestionSpeed;
		int intestineSyncDistance;



		// GUI SYNTH
		// ofxPanel            guiSynth;
		// ofParameterGroup    guiSynthFX;
		// // pdsp::ParameterGain masterGain;
		// pdsp::Parameter     lowCutFreq;
		// pdsp::Parameter     compThreshold;
		// pdsp::Parameter     compKnee;

		// ofParameterGroup    guiSynthBrthrs;
		// pdsp::Parameter		brthGain;
		// pdsp::Parameter     brthFineTune;
		// pdsp::Parameter     brthOscMix;
		// pdsp::Parameter     brthPw;
		// pdsp::Parameter     brthPwmSpeed;
		// pdsp::ParameterAmp  brthPwmAmt;  
		// pdsp::Parameter     brthCutoff;
		// pdsp::Parameter     brthReso;	

		// ofParameterGroup    guiSynthPumpers;
		// pdsp::Parameter        pmprGainControl;  
		// pdsp::Parameter        pmprPitchControl;    
		// pdsp::Parameter        pmprPEnvAmtControl;
		// pdsp::Parameter        pmprPEnvAttackControl;
		// pdsp::Parameter        pmprPEnvReleaseControl;
		// pdsp::Parameter        pmprVeloToEnv;
		// pdsp::Parameter        pmprFbControl;
		// pdsp::Parameter        pmprFbModControl;
		// pdsp::Parameter        pmprEnvAttackControl;
		// pdsp::Parameter        pmprEnvHoldControl;
		// pdsp::Parameter        pmprEnvReleaseControl;
		
		// ofParameterGroup    guiSynthPumpersImpulse;
		// pdsp::Parameter        pmprImpulseEnvAttack;
		// pdsp::Parameter        pmprImpulseEnvHold;
		// pdsp::Parameter        pmprImpulseEnvRelease;
		// ofParameter<float> 	   pmprImpulseAmt;

		// ofParameterGroup    guiSynthNeurons;
		// pdsp::Parameter		   nronGain;
		// pdsp::Parameter		   nronEnvAttack;
		// pdsp::Parameter		   nronEnvRelease; 
		// pdsp::Parameter		   nronPitch;
		// pdsp::Parameter		   nronFineTune;
		// pdsp::Parameter		   nronOscMix;
		// pdsp::Parameter		   nronCutoff;
		// pdsp::Parameter		   nronReso;
		// ofParameter<float> 	   nronFilterModAmt;
		// pdsp::Parameter		   nronFEnvAttack;
		// pdsp::Parameter		   nronFEnvHold;
		// ofParameter<int> 	   nronSignalSpeed;
		
		// ofParameterGroup    guiSynthIntestines;
		// pdsp::Parameter		   itstGain;
		// pdsp::Parameter		   itstEnvAttack;
		// pdsp::Parameter		   itstEnvDecay; 
		// pdsp::Parameter		   itstEnvSustain; 
		// pdsp::Parameter		   itstEnvRelease; 
		// pdsp::Parameter		   itstPitch;
		// pdsp::Parameter		   itstFineTune;
		// pdsp::Parameter        itstPw;
		// pdsp::Parameter        itstPwmAmt;  
		// pdsp::Parameter        itstLfoRate;
		// pdsp::Parameter		   itstOscMix;
		// pdsp::Parameter		   itstCutoff;
		// pdsp::Parameter		   itstReso;
		// pdsp::Parameter		   itstFilterModAmt;


		bool maskChanged;	
		bool debugMode;

		ofApp* mainPtr;

	private:
		std::unordered_map<std::string, ParamValue> values;

		void applyDefaults();
		void publishValues();
		bool loadSimulationPreset(const std::string& filePath);
		bool loadTubeCalibrationPreset(const std::string& filePath);
		static bool parseBool(const std::string& value, bool fallback);
		static glm::vec2 parseVec2(const std::string& value, const glm::vec2& fallback);
		static ofColor parseColor(const std::string& value, const ofColor& fallback);
		static std::string resolveFirstExistingPath(const std::vector<std::string>& candidates);
		
};

