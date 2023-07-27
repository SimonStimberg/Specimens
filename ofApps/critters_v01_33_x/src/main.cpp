#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.h"
#include "ofAppGLFWWindow.h"

// uncomment this if the simulation is to be shown on CRT screens
// #define SHOW_ON_CRT


//========================================================================
int main( ){
	
	ofGLFWWindowSettings settings;


	// setup the main simulation (ofApp)
	#ifdef SHOW_ON_CRT

		// this will run the app in fullscreen mode spanning over all connected monitors
		settings.multiMonitorFullScreen = true;
		settings.windowMode = OF_FULLSCREEN;
		// settings.monitor = 1;
	#else
	
		// run the app in a window on the computer screen
		// settings.setSize(800, 600);		// the standard resolution for one screen buffer is 800x600 px
		settings.setSize(600, 800);
		// settings.setSize(1600, 600);		// 2 screens
		// settings.setSize(3200, 600);		// 4 screens
		settings.windowMode = OF_WINDOW;
		settings.resizable = false;
		settings.setPosition(glm::vec2(600,0));	
	#endif

	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);



	// setup the GuiApp, a second window containting the control panels
	settings.setSize(600, 800);
	settings.setPosition(glm::vec2(0,0));
	settings.windowMode = OF_WINDOW;
	settings.resizable = false;
	settings.monitor = 0;
	shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);


	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<GuiApp> guiApp(new GuiApp);
	// mainApp->guiPtr = guiApp;
	guiPtr = guiApp;
	guiApp->mainPtr = mainApp;	// create a pointer for intercommunication between the apps


	// run the apps	
	ofRunApp(mainWindow, mainApp);
	ofRunApp(guiWindow, guiApp);
	ofRunMainLoop();

}
