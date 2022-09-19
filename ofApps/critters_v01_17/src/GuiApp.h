#pragma once

#include "ofMain.h"
#include "ofxGui.h"
// #include "ofApp.h"


class ofApp;



class GuiApp: public ofBaseApp {
public:
	void setup();
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


	ofParameter<float> tuneCanvasWidth;
	ofParameter<float> tuneCanvasHeight;
	ofParameter<float> tuneVerticalBow;
	ofParameter<float> tuneHorizontalBow;
	ofParameter<float> tuneEdges;
	ofParameter<float> tuneXpos;
	ofParameter<float> tuneYpos;
	ofParameter<float> tuneRotation;

	bool maskChanged;	
	string defaultSimulationPreset;
	string defaultSynthPreset;

	shared_ptr<ofApp> mainPtr;



		
};
