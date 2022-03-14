#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class GuiApp: public ofBaseApp {
public:
	void setup();
	void update();
	void draw();


	ofxPanel gui;
	ofParameterGroup guiTune;


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


		
};

