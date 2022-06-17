#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);

    ofLogNotice("Canvas Resolution: " + ofToString(ofGetWidth()) + " x " + ofToString(ofGetHeight()));
    ofLogNotice("Screen Resolution: " + ofToString(ofGetScreenWidth()) + " x " + ofToString(ofGetScreenHeight()));

    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    pix.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_RGB);

    ofLogNotice("Pixel array: " + ofToString(pix.size()));

    blueStep = 0;
    changeBlueBy = 1;

}

//--------------------------------------------------------------
void ofApp::update(){
    // ofLogNotice("Framerate: " +  ofToString(ofGetFrameRate()) );

    fbo.begin();
    
    ofClear(0);
    ofSetColor(255,255,255);
    ofDrawCircle(180, 144, 20);
    // molSystem.draw();
    fbo.end();

    // ofPixels & pixels = fbo.getPixels();

}

//--------------------------------------------------------------
void ofApp::draw(){

    // for (int i = 0; i < ofGetHeight(); i++) {
    //     if (i % 50 == 0 || i == 1) {
    //         ofSetColor(255, 0, 0);    
    //     } else if (i % 2 == 0) {
    //         ofSetColor(0);
    //     } else {
    //         ofSetColor(255);
    //     }
    //     ofDrawLine(0, i, ofGetWidth(), i);
    // }
    // for (int i = 0; i < ofGetHeight()/2; i++) {
    //     if (i % 50 == 0 || i == 1) {
    //         ofSetColor(255, 0, 0);    
    //     } else if (i % 2 == 0) {
    //         ofSetColor(0);
    //     } else {
    //         ofSetColor(255);
    //     }
    //     ofDrawLine(0, i*2, ofGetWidth(), i*2);
    //     ofDrawLine(0, i*2+1, ofGetWidth(), i*2+1);
    // }

    int blue = (256 / 4) * blueStep;

    int yStep = ofGetHeight() / 8;
    int xStep = ofGetWidth() / 8;

    for (int y = 0; y < ofGetHeight(); y += yStep) {
        for (int x = 0; x < ofGetWidth(); x += xStep) {
            int red   = (256 / 7) * (y / yStep);
            int green = (256 / 7) * (x / xStep);
            // ofSetColor(red, green, blueStep);
            ofSetColor(red, green, blue);
            ofDrawRectangle(x, y, xStep, yStep);
        }
    }

    // if (blueStep >= 255) {
    //     changeBlueBy = -1;
    // } else if (blueStep < 1) {
    //     changeBlueBy = 1;
    // }

    // blueStep += changeBlueBy;
    
    blueStep++;
    blueStep = blueStep > 3 ? 0 : blueStep;

    ofSleepMillis(1000);

    // ofSetColor(255);
    // ofDrawCircle(50, 50, 20);

    // fbo.draw(0, 0);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
