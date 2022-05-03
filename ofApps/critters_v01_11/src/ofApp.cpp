#include "ofApp.h"


shared_ptr<GuiApp> guiPtr;


//--------------------------------------------------------------
void ofApp::setup(){
    
    offset = 60;

    ofSetVerticalSync(true);
    ofSetFrameRate(50);

    // fbo.allocate(360, 288, GL_RGB);
    fbo.allocate(ofGetWidth()-offset, ofGetHeight()-offset, GL_RGB);

    molSystem.setup(fbo.getWidth(), fbo.getHeight());
    molSystem.addCells(7);
    molSystem.addMolecules(500);

    // ofLogNotice("fric: " + ofToString(guiPtr->tuneFriction));
    ofLogNotice("Canvas Size: " + ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight()));

}

//--------------------------------------------------------------
void ofApp::update(){

    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    molSystem.update();
    

    fbo.begin();
    
        ofClear(0);
    // ofSetColor(255,255,255);
    // ofDrawCircle(180, 144, 20);
        // ofPushMatrix();
        // ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
        // molSystem.draw();
        // ofPopMatrix();

    fbo.end();


    // float oscillate = (sin(ofGetElapsedTimef())+1) * 0.5;
    // ofLogNotice(ofToString(oscillate));
}

//--------------------------------------------------------------
void ofApp::draw(){

    // ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    // ofBackground(208, 255, 63);
    ofBackground(0);

    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    molSystem.draw();
    ofPopMatrix();

    // fbo.draw(0, 0, 720, 576);
    // fbo.draw(offset/2, offset/2);

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == ' ' ){
		molSystem.reset(true);
	}
    if( key == 'r' ){
		molSystem.reset(false);
	}
    if( key == 'f' ){
        ofToggleFullscreen();
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
