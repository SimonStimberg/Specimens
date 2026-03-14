#pragma once

#include "ofMain.h"
// #include "ofxGui.h"
#include "GuiApp.h"
#include "ofxOsc.h"

#include "molSystem/molecularSystem.h"
// #include "synth/synthFX.h"
// #include "synth/audioMaster.h"
// #include "interaction/kinect.h"


extern shared_ptr<GuiApp> guiPtr;	// a pointer to the gui app for the exchange of control parameters


class ofApp : public ofBaseApp{


	public:

		void setup();
		void update();
		void draw();
		// void initSynth();
		void setTVmask();
		// void reconnectAudio();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		// static const int 	numScreens = 1;			// IMPORTANT: defines the number of screens/windows
		molecularSystem 	molSystem;	// there is one molecular system for each screen/window - the size amount of instances is defined by numScreens
		// ofFbo 				vessel[numScreens];		// frame buffer object - the size amount of instances is defined by numScreens
		glm::vec2 			screenResolution;
		float 				screenSizeFactor;		// a resizing factor to account for different CRT screen sizes as the resolution stays the same for all screens, the depicted graphics would be perceived with different sizes

		
		// AUDIO UNITS		

		// pdsp::Engine            engine;		// the audio engine

		// // busses, routing, parameters
		// pdsp::ParameterGain     gain;
		// pdsp::ParameterAmp      fxBus;
		// pdsp::ParameterAmp      cleanBus;
		// pdsp::ParameterAmp      sumBus;


		// // effect units
		// effect::QuadDelay				delay;
		// effect::QuadChorus 				chorus;
		// effect::DisasterFX				disasterFX;
		// pdsp::LowCut					loCut;
		// vector <pdsp::Compressor>		compressor;
		// // pdsp::Compressor		compressorA;
		// // pdsp::Compressor		compressorB;
		
		// audioModule::Master audioMaster;




		// mask for calibrating the screen shape 
		ofTexture mask;
		bool showMask;
		bool showOverlay = false;

		bool singleSpeciesMode = false;


		bool testBool = true;


		// kinect class that processes the incoming data from the Kinect for interaction
		// kinectHandler kinectToPoints;



		bool mouseDown = false;
		int mouseDownTime = 0;
		int mouseDownButton;


		// OSC receiver for intrusion points from master computer
		static constexpr int OSC_PORT = 9000;
		static constexpr int OSC_TIMEOUT_MS = 500;  // fall back to mouse if no data for this long
		ofxOscReceiver      oscReceiver;
		vector<glm::vec2>   oscIntrusionPoints;
		uint64_t            lastOscTime = 0;


		int maxBreathers = 0;
		int maxPumpers = 0;
		int maxNeurons = 0;
		int maxIntestines = 0;
		
};
