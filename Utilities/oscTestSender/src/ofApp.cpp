#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("OSC Intrusion Test Sender  ->  " + string(HOST) + ":" + ofToString(PORT));
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	sender.setup(HOST, PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
	// Normalize mouse to [-0.5, +0.5] — the receiver maps these to world space
	// using worldSize = screenResolution * screenSizeFactor, so screenSizeFactor
	// is absorbed here and does not need to be known by the sender.
	float normX = (ofGetMouseX() / (float)ofGetWidth())  - 0.5f;
	float normY = (ofGetMouseY() / (float)ofGetHeight()) - 0.5f;

	ofxOscMessage m;
	m.setAddress("/intrusion");
	m.addFloatArg(normX);
	m.addFloatArg(normY);
	sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(30);

	// draw crosshair at mouse position
	ofSetColor(200, 80, 80);
	ofDrawLine(ofGetMouseX(), 0, ofGetMouseX(), ofGetHeight());
	ofDrawLine(0, ofGetMouseY(), ofGetWidth(), ofGetMouseY());
	ofSetColor(255, 80, 80);
	ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 6);

	// info
	ofSetColor(220);
	string info = "Sending /intrusion  ->  " + string(HOST) + ":" + ofToString(PORT);
	info += "\nnorm x: " + ofToString((ofGetMouseX() / (float)ofGetWidth())  - 0.5f, 3);
	info += "   norm y: " + ofToString((ofGetMouseY() / (float)ofGetHeight()) - 0.5f, 3);
	info += "\n\nChange HOST in ofApp.h to the Pi's IP address.";
	ofDrawBitmapStringHighlight(info, 12, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Add your key handling logic here, or leave it empty if not needed
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
