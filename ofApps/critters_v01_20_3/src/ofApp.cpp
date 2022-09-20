#include "ofApp.h"


shared_ptr<GuiApp> guiPtr;


//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetVerticalSync(true);
    ofSetFrameRate(50);

    mask.allocate(ofGetWidth(), ofGetHeight(), GL_LUMINANCE);
    showMask = false;

    molSystem.setup(ofGetWidth(), ofGetHeight());    
    molSystem.addLiquid(300);
    molSystem.addBreathers(5);
    molSystem.addPumpers(6);
    molSystem.addNeurons(20);
    molSystem.addIntestines(3);

    // ofLogNotice("fric: " + ofToString(guiPtr->tuneFriction));
    ofLogNotice("Canvas Size: " + ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight()));

    
    setTVmask();

    initSynth();
    synth.setUI();
    synth.gui.setPosition(300, 10);
    guiPtr->loadPreset("default", 2);

    kinectToPoints.setup();

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
    molSystem.setIntrusionPoints(mousePos);


    molSystem.update();

    if(guiPtr->maskChanged) { setTVmask(); }


    vector<float> inflations = molSystem.getCellInflation();

    // ofLogNotice("inflations: " +  ofToString(molSystem.getCellInflation()));
    // synth.setVoiceLevels(inflations);   
    synth.setVoiceCutoffs(inflations);     
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    // ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    // ofBackground(208, 255, 63);
    ofBackground(0);

    ofPushMatrix();
        ofTranslate(ofGetWidth()*0.5 + guiPtr->tuneXpos, ofGetHeight()*0.5 + guiPtr->tuneYpos);
        ofRotateDeg(guiPtr->tuneRotation);
  
        molSystem.draw();
        if(guiPtr->switchScreenMask) {
            // ofTranslate(-ofGetWidth()*0.5, -ofGetHeight()*0.5);
            mask.draw(-ofGetWidth()*0.5, -ofGetHeight()*0.5);
        }

        // vector <glm::vec2> tPts = kinectToPoints.getTouchPoints();
        // for(int i = 0; i < tPts.size(); i++) {
        //     ofSetColor(ofColor::yellowGreen);
        //     ofDrawCircle(tPts[i], 10);
        // }


    ofPopMatrix();

    if(guiPtr->switchKinectCalibration) {
        kinectToPoints.drawKinect();
        kinectToPoints.drawCalibrationAids();
    }


}

//--------------------------------------------------------------
void ofApp::setTVmask() {

    ofPixels pixels;
    mask.readToPixels(pixels);
    // pixels.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_MONO);

    Molecule * m = new Molecule(&molSystem);
    m->reset(0,0,0,0);

    for(int i = 0; i < pixels.size(); i++) {
        
        float y = floor(i/ofGetWidth());
        float x = i - y * ofGetWidth();
        y -= ofGetHeight() * 0.5;
        x -= ofGetWidth()  * 0.5;
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
    mask.loadData(pixels);

    guiPtr->maskChanged = false;
}



//--------------------------------------------------------------
void ofApp::initSynth() {

    vector <int> pitches;
    // vector <float> panPositions;

    // int allPitches[7] = {60, 63, 67, 70, 74, 80, 84};
    int allPitches[7] = {36, 48, 55, 60, 63, 70, 74};


    vector<int> cellSizes = molSystem.getCellSizes();

    // int numCells = molSystem.cells.size();
    
    
    
    for(int i = 0; i < cellSizes.size(); i++){
                
        // set panPosition of the note according to the x-position of the bulb
        // float pan = ofMap(b.x, -worldSize.x*0.5, worldSize.x*0.5, -1.f, 1.f);
        // panPositions.push_back( pan );

        // int pick = floor(ofRandom(7));
        int newPitch = 0;

        // newPitch = (int)round(ofMap(cellSizes[i], 13, 31, 6., 0., true));

        if (cellSizes[i] > 14) {
            // newPitch = ( (int)ofRandom(0,2) == 0) ? 48 : 36;
            // newPitch = allPitches[i % 7] - 12;
            newPitch = (int)round(ofMap(cellSizes[i], 23, 31, 3., 0., true));
        } else {
            // newPitch = ( (int)ofRandom(0,2) == 0) ? 64 : 70;
            // newPitch = allPitches[i % 7];
            newPitch = (int)round(ofMap(cellSizes[i], 13, 22, 6., 4., true));
        }
        pitches.push_back(allPitches[newPitch]);

        // pitches.push_back(newPitch);

    }


    synth.setup( pitches );
    // synth.setVoicePans( panPositions );

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == ' ' ){
		molSystem.reset(true);
        initSynth();
	}
    if( key == 'r' ){
		molSystem.reset(false);
	}
    if( key == 'f' ){
        ofToggleFullscreen();
	}
    if( key == 'x' ){
        molSystem.flush = !molSystem.flush;
	}
    if( key == 'm' ){
        showMask = !showMask;        
    }
    if( key == 'd' ){
        molSystem.debugView = !molSystem.debugView;        
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
