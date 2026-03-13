#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.h"
#include "ofAppGLFWWindow.h"

// uncomment this if the simulation is to be shown on CRT screens
#define SHOW_ON_CRT


//========================================================================
int main( ){
	
	
	ofGLWindowSettings settings;
	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	// ofGLFWWindowSettings settings;


	// setup the main simulation (ofApp)
	#ifdef SHOW_ON_CRT

		// this will run the app in fullscreen mode spanning over all connected monitors
		// settings.multiMonitorFullScreen = true;
		// settings.windowMode = OF_FULLSCREEN;
		settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN	
		settings.setSize(720, 576);

		// settings.monitor = 0;
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

	// shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
	auto window = ofCreateWindow(settings);


	// shared_ptr<ofApp> mainApp(new ofApp);
	guiPtr = make_shared<GuiApp>();


	// run the apps	
	// ofRunApp(mainWindow, mainApp);
	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();





	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	// ofGLWindowSettings settings;
	// settings.setSize(1024, 768);
	// settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	// auto window = ofCreateWindow(settings);

	// ofRunApp(window, make_shared<ofApp>());
	// ofRunMainLoop();

}
