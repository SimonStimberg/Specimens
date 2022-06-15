#pragma once

#include "ofMain.h"
#include "molecularSystem.h"
#include "ofxGui.h"
#include "ofxPDSP.h"

#include "GuiApp.h"
#include "synth/synth.h"


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

		molecularSystem 	molSystem;
		PolySynth	synth;

		ofTexture mask;
		bool showMask;
		
};
