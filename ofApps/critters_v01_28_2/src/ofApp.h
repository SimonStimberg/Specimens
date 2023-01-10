#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPDSP.h"
#include "GuiApp.h"

#include "molSystem/molecularSystem.h"
#include "synth/synthFX.h"
#include "interaction/kinect.h"


extern shared_ptr<GuiApp> guiPtr;


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void initSynth();
		void setTVmask();

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


		static const int numScreens = 1;
		glm::vec2 screenResolution;
		ofFbo vessel[numScreens];

		molecularSystem 	molSystem[numScreens];

		pdsp::Engine            engine;
		pdsp::ParameterGain     gain;
		pdsp::ParameterAmp      fxBus;
			pdsp::ParameterGain     sendChorusBreathers;
			pdsp::ParameterGain     sendChorusPumpers;
			pdsp::ParameterGain     sendChorusNeurons;
			pdsp::ParameterGain     sendChorusIntestines;
			pdsp::ParameterGain     sendDelayBreathers;
			pdsp::ParameterGain     sendDelayPumpers;
			pdsp::ParameterGain     sendDelayNeurons;
			pdsp::ParameterGain     sendDelayIntestines;
		StereoDelay 			delay;
    	StereoChorus 			chorus;

		ofTexture mask[numScreens];
		bool showMask;

		kinectHandler kinectToPoints;
		
};