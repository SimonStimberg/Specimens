#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPDSP.h"
#include "GuiApp.h"

#include "molSystem/molecularSystem.h"
#include "synth/synthFX.h"
#include "interaction/kinect.h"


extern shared_ptr<GuiApp> guiPtr;	// a pointer to the gui app for the exchange of control parameters


class ofApp : public ofBaseApp{


	public:

		void setup();
		void update();
		void draw();
		void initSynth();
		void setTVmask();
		void reconnectAudio();

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


		static const int 	numScreens = 1;			// IMPORTANT: defines the number of screens/windows
		molecularSystem 	molSystem[numScreens];	// there is one molecular system for each screen/window - the size amount of instances is defined by numScreens
		ofFbo 				vessel[numScreens];		// frame buffer object - the size amount of instances is defined by numScreens
		glm::vec2 			screenResolution;

		
		// AUDIO UNITS		

		pdsp::Engine            engine;		// the audio engine

		// busses, routing, parameters
		pdsp::ParameterGain     gain;
		pdsp::ParameterAmp      fxBus;
		pdsp::ParameterAmp      cleanBus;
		pdsp::ParameterAmp      sumBus;


		// effect units
		effect::QuadDelay				delay;
		effect::QuadChorus 				chorus;
		effect::DisasterFX				disasterFX;
		pdsp::LowCut					loCut;
		vector <pdsp::Compressor>		compressor;
		// pdsp::Compressor		compressorA;
		// pdsp::Compressor		compressorB;



		// mask for calibrating the screen shape 
		ofTexture mask[numScreens];
		bool showMask;


		// kinect class that processes the incoming data from the Kinect for interaction
		kinectHandler kinectToPoints;
		
};
