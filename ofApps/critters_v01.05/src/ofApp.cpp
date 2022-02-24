#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // fbo.allocate(360, 288, GL_RGB);
    offset = 60;
    fbo.allocate(ofGetWidth()-offset, ofGetHeight()-offset, GL_RGB);

    int amount = 19;

    molSystem.setup(amount, fbo.getWidth(), fbo.getHeight());


}

//--------------------------------------------------------------
void ofApp::update(){

    molSystem.update();
    
    fbo.begin();
    
    ofClear(0);
    // ofSetColor(255,255,255);
    // ofDrawCircle(180, 144, 20);
    molSystem.draw();
    fbo.end();

}

//--------------------------------------------------------------
void ofApp::draw(){

    // ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    ofBackground(0);

    // molSystem.draw();

    // fbo.draw(0, 0, 720, 576);
    fbo.draw(offset/2, offset/2);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == ' ' ){
		molSystem.reset();
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
