#pragma once

#include "ofMain.h"
#include "molecularSystem.h"
#include "ofxGui.h"



extern ofParameter<float> tuneFriction;

extern ofParameter<float> tuneRepulsionThresh;
extern ofParameter<float> tuneRepulsionForce;

extern ofParameter<float> tuneFlatThresh;
extern ofParameter<float> tuneFlatDistance;
extern ofParameter<float> tuneFlatLimitForce;

extern ofParameter<float> tuneSpringLength;
extern ofParameter<float> tuneSpringElasticity;
extern ofParameter<float> tuneSpringLimitForce;

extern ofParameter<bool>  switchOscillation;



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void setupGUI();

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

		ofFbo fbo;

		// vector <molecule> molecules;
		molecularSystem 	molSystem;
		int offset;


		// gui stuff
		ofxPanel gui;
		ofParameterGroup guiTune;

		// ofParameter<int> addFriction;
		
};
