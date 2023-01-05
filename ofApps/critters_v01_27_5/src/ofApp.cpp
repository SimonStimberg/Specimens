#include "ofApp.h"


shared_ptr<GuiApp> guiPtr;


//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetVerticalSync(true);
    ofSetFrameRate(50);


    // synth.setup();

    initSynth();


    screenResolution.x = 800;
    screenResolution.y = 600;


    vector<float> screenSizeFactor;
    screenSizeFactor.assign(numScreens,1.0f);

    // set the desired vessel sizes by shrinking the native resolution by a custom factor
    screenSizeFactor[0] = 0.8;          // make this screen 80% the size of the native resolution 800px -> 640px
    // screenSizeFactor[1] = 0.8 * 0.85;   // make the smaller screen even only 85% of the bigger screen to match the perceived size


    for (int i = 0; i < numScreens; i++) {

        // anti-aliasing FBOs by multisampling! watch out: needs more performance
        ofFbo::Settings settings;
            settings.numSamples = 4;    // also try 8, if your GPU supports it
            settings.useDepth = false;  // don't know what this is for
            settings.width = screenResolution.x * screenSizeFactor[i];
            settings.height = screenResolution.y * screenSizeFactor[i];

            vessel[i].allocate(settings);


        mask[i].allocate(vessel[i].getWidth(), vessel[i].getHeight(), GL_LUMINANCE);

        molSystem[i].setup(vessel[i].getWidth(), vessel[i].getHeight());    

        molSystem[i].masterBus.ch(0) >> fxBus.ch(0);      // channel1: breathers
        molSystem[i].masterBus.ch(1) >> gain.ch(0);       // channel2: pumpers
        // molSystem[i].masterBus >> gain.ch(1);
        molSystem[i].blackhole >> engine.blackhole();



        molSystem[i].addOrganisms(LIQUID,    100);
        // molSystem[i].addOrganisms(BREATHER,  2);
        molSystem[i].addOrganisms(PUMPER,    1);
        // molSystem.addOrganisms(NEURON,    10);
        // molSystem[i].addOrganisms(INTESTINE, 2);
        // molSystem.addLiquid(300);

        ofLogNotice("Screen No. " + ofToString(i) + " with size: " + ofToString(vessel[i].getWidth()) + "x" + ofToString(vessel[i].getHeight()));
    }

    // ofLogNotice("fric: " + ofToString(guiPtr->tuneFriction));
    // ofLogNotice("Canvas Size: " + ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight()));

    
    setTVmask();
    showMask = false;

    
    // synth.setUI();
    // synth.gui.setPosition(300, 10);
    // synth.guiVoices.setPosition(300, 675);
    // guiPtr->loadPreset("default", 2);

    kinectToPoints.setup();

    


    // ofLogNotice("0 dB is: " + ofToString(dB(0.0)));

}

//--------------------------------------------------------------
void ofApp::update() {

    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    // USE THIS FOR KINECT INTERACTION
    // kinectToPoints.update();
    // molSystem.setIntrusionPoints(kinectToPoints.getTouchPoints());

    // USE THIS FOR MOUSE INTERACTION   
    vector <glm::vec2> mousePos;
    mousePos.assign(1, glm::vec2(ofGetMouseX() - ofGetWidth() * 0.5, ofGetMouseY() - ofGetHeight() * 0.5) );

    
    for (int i = 0; i < numScreens; i++) {
        
        float scalingFactor = vessel[i].getWidth() / screenResolution.x;
        // float scalingFactor = 1.0;
        mousePos[0].x = (ofGetMouseX() - screenResolution.x * 0.5 ) * scalingFactor - vessel[i].getWidth() * i;     // vessel[i].getWidth()
        mousePos[0].y = (ofGetMouseY() - screenResolution.y * 0.5 ) * scalingFactor;

        // ofLogNotice("mouseX: " + ofToString(ofGetMouseX()));

        molSystem[i].setIntrusionPoints(mousePos);
        molSystem[i].update();

        if(guiPtr->maskChanged) { setTVmask(); }


        // vector<float> inflations = molSystem.getCellInflation();

        // ofLogNotice("inflations: " +  ofToString(molSystem.getCellInflation()));
        // synth.setVoiceLevels(inflations);   
        // synth.setVoiceCutoffs(inflations);     


        vessel[i].begin();
        
            // ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
            // ofBackground(208, 255, 63);
            ofBackground(0);

            ofPushMatrix();
                // ofTranslate(ofGetWidth()*0.5 + guiPtr->tuneXpos, ofGetHeight()*0.5 + guiPtr->tuneYpos);
                // ofTranslate(vessel[i].getWidth()*0.5 + guiPtr->tuneXpos, vessel[i].getHeight()*0.5 + guiPtr->tuneYpos);
                ofTranslate(vessel[i].getWidth()*0.5, vessel[i].getHeight()*0.5);
                // ofRotateDeg(guiPtr->tuneRotation);
        
                molSystem[i].draw();
            ofPopMatrix();

                if(guiPtr->switchScreenMask) {
                    // ofTranslate(-ofGetWidth()*0.5, -ofGetHeight()*0.5);
                    // mask[i].draw(-vessel[i].getWidth()*0.5, -vessel[i].getHeight()*0.5);
                    mask[i].draw(0, 0);
                }

                // vector <glm::vec2> tPts = kinectToPoints.getTouchPoints();
                // for(int i = 0; i < tPts.size(); i++) {
                //     ofSetColor(ofColor::yellowGreen);
                //     ofDrawCircle(tPts[i], 10);
                // }


            

            if(guiPtr->switchKinectCalibration) {
                kinectToPoints.drawKinect();
                kinectToPoints.drawCalibrationAids();
            }
        
        
        vessel[i].end();

    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    for (int i = 0; i < numScreens; i++) {
        int xShift = i * screenResolution.x;
        vessel[i].draw(xShift, 0, screenResolution.x, screenResolution.y);
        // vessel[i].draw(xShift, 0);
    }   

    ofSetColor(ofColor::aliceBlue);
    ofFill();
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 4.);

}

//--------------------------------------------------------------
void ofApp::setTVmask() {

    

    for (int s = 0; s < numScreens; s++) {
        ofPixels pixels;
        mask[s].readToPixels(pixels);
        // pixels.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_MONO);

        Molecule * m = new Molecule(&molSystem[s]);
        m->reset(0,0,0,0);

        for(int i = 0; i < pixels.size(); i++) {
            
            float y = floor(i/vessel[s].getWidth());
            float x = i - y * vessel[s].getWidth();
            y -= vessel[s].getHeight() * 0.5;
            x -= vessel[s].getWidth()  * 0.5;
            // int val = floor(ofMap(x, 0, ofGetWidth(), 0, 255));
            // int val = floor(ofMap(y, 0, ofGetHeight(), 0, 255));
            // pixels[i] = val;

            // pixels[i] = (m->signedDistanceField(glm::vec2(x, y)) > 0.0) ? 0 : 255;

            float sdf = m->signedDistanceField(glm::vec2(x, y));

            if(sdf <= 0.0 ) {
                pixels[i] = 255;
            } else if(sdf > 0.0 && sdf <= 10.0) {
                pixels[i] = 150;
            } else {
                pixels[i] = 0;
            }

        }
        mask[s].loadData(pixels);

    }

    guiPtr->maskChanged = false;
}



//--------------------------------------------------------------
void ofApp::initSynth() {

    // gain.set(-12);
    fxBus.set(1.0);
    guiPtr->masterGain >> gain;

    fxBus.ch(0) >> chorus.ch(0) >> delay.ch(0) >> gain.ch(0);     // one FX bus per Screen
    fxBus.ch(1) >> chorus.ch(1) >> delay.ch(1) >> gain.ch(1);

    // delay.ch(0) >> gain.ch(0);      // for temporary stereo monitoring 
    // delay.ch(0) >> gain.ch(1);


    gain.ch(0) >> engine.audio_out(0);
    gain.ch(0) >> engine.audio_out(1);

    // gain.ch(0) >> absolute >> envFollowerPeak >> engine.blackhole();

        

     //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(2); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 

    

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == ' ' ){
		// molSystem.reset(true);
        // initSynth();
        molSystem[0].addOrganisms(PUMPER, 1);
        // int allPitches[7] = {60, 63, 67, 70, 74, 80, 84};
        // synth.addVoice( allPitches[ (int)floor(ofRandom(7)) ] );
        // synth.addVoice(floor(ofRandom(60.0, 72.0)));
	}
    if( key == 'r' ){
		molSystem[0].reset(false);
	}
    if( key == 'f' ){
        ofToggleFullscreen();
	}
    if( key == 'x' ){
        molSystem[0].flush = !molSystem[0].flush;
	}
    if( key == 'm' ){
        showMask = !showMask;        
    }
    if( key == 'd' ){
        molSystem[0].debugView = !molSystem[0].debugView;        
    }
    if( key == OF_KEY_RETURN && guiPtr->switchKinectCalibration ){
        kinectToPoints.calibrate();        
    }
    
  
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // molSystem.addIntestine(x - ofGetWidth()*0.5, y - ofGetHeight()*0.5);

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
