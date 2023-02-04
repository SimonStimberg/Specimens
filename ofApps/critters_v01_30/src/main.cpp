#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.h"
#include "ofAppGLFWWindow.h"

// #define SHOW_ON_CRT

//========================================================================
int main( ){
	
	ofGLFWWindowSettings settings;

    // settings.setSize(3200, 600);		// 4 screens
	// settings.setSize(1600, 600);		// 2 screens
	// settings.setSize(800, 600);
	#ifndef SHOW_ON_CRT
		settings.setSize(600, 800);
		settings.resizable = false;
		settings.setPosition(glm::vec2(600,0));
		settings.windowMode = OF_WINDOW; 	// OF_GAME_MODE;	// OF_FULLSCREEN
	#else
		settings.windowMode = OF_FULLSCREEN;
		settings.monitor = 2;
	#endif
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

	settings.setSize(600, 800);
	settings.setPosition(glm::vec2(0,0));
	settings.resizable = false;
	settings.windowMode = OF_WINDOW;
	settings.monitor = 0;
	shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);

	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<GuiApp> guiApp(new GuiApp);
	// mainApp->guiPtr = guiApp;
	guiPtr = guiApp;

	guiApp->mainPtr = mainApp;

	
	ofRunApp(mainWindow, mainApp);
	ofRunApp(guiWindow, guiApp);
	ofRunMainLoop();


}
