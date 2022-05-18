#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	// ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	ofGLFWWindowSettings settings;

    settings.setSize(720, 576);
	// settings.setPosition(glm::vec2(300,0));
	// settings.resizable = false;

	settings.windowMode = OF_FULLSCREEN;
	settings.monitor = 1;

    ofCreateWindow(settings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
