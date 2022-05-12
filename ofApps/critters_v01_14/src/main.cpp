#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	
	ofGLFWWindowSettings settings;

    settings.setSize(720, 576);
	settings.setPosition(glm::vec2(300,0));
	settings.resizable = false;
	settings.windowMode = OF_WINDOW; 	// OF_GAME_MODE;	// OF_FULLSCREEN
	settings.monitor = 1;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

	settings.setSize(300, 600);
	settings.setPosition(glm::vec2(0,0));
	settings.resizable = false;
	settings.windowMode = OF_WINDOW;
	settings.monitor = 0;
	shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);

	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<GuiApp> guiApp(new GuiApp);
	// mainApp->guiPtr = guiApp;
	guiPtr = guiApp;

	ofRunApp(guiWindow, guiApp);
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();


}
