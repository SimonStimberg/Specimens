#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

// IP address of the Pi (change to Pi's static IP, e.g. "192.168.1.101")
// #define HOST "localhost"
#define HOST "192.168.0.19"

// must match OSC_PORT in the critters ofApp
#define PORT 9000

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxOscSender sender;
};
