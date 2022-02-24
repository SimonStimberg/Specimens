#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofGLFWWindowSettings settings;

    settings.setSize(720, 576);
	// settings.setPosition(glm::vec2(300,0));
	// settings.resizable = false;
	settings.windowMode = OF_FULLSCREEN;
	settings.monitor = 1;
    ofCreateWindow(settings);

    // ofSetupOpenGL(300,300, OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
