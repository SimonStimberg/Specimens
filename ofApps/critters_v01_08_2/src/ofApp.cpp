#include "ofApp.h"


ofParameter<float> tuneFriction;

ofParameter<float> tuneRepulsionThresh;
ofParameter<float> tuneRepulsionForce;

ofParameter<float> tuneFlatThresh;
ofParameter<float> tuneFlatDistance;
ofParameter<float> tuneFlatLimitForce;

ofParameter<float> tuneSpringLength;
ofParameter<float> tuneSpringElasticity;
ofParameter<float> tuneSpringLimitForce;



//--------------------------------------------------------------
void ofApp::setup(){
    // fbo.allocate(360, 288, GL_RGB);
    offset = 60;
    fbo.allocate(ofGetWidth()-offset, ofGetHeight()-offset, GL_RGB);

    // int amount = 19;

    molSystem.setup(fbo.getWidth(), fbo.getHeight());

    molSystem.addCells(5);

    molSystem.addMolecules(50);

    setupGUI();

}

//--------------------------------------------------------------
void ofApp::update(){

    molSystem.update();
    
    fbo.begin();
    
    ofClear(0);
    // ofSetColor(255,255,255);
    // ofDrawCircle(180, 144, 20);
    // molSystem.draw();
    fbo.end();

}

//--------------------------------------------------------------
void ofApp::draw(){

    // ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    ofBackground(0);

    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);

    molSystem.draw();

    // fbo.draw(0, 0, 720, 576);
    // fbo.draw(offset/2, offset/2);
    ofPopMatrix();

    gui.draw();

}

//--------------------------------------------------------------
void ofApp::setupGUI(){
    gui.setup();
    gui.setPosition(10, 10);
    // gui.setWi

    guiTune.setName("Tune Forces");
    guiTune.add(tuneFriction.set("Friction", 0.97, 0.9, 0.98));
    
    guiTune.add(tuneRepulsionThresh.set("Repulsion Radius", 14.0, 1., 30.));
    guiTune.add(tuneRepulsionForce.set("Tune Repulsion Force", 0, -10, 10));

    guiTune.add(tuneFlatThresh.set("Flattening min Thresh", 7.0, 1., 30.));
    guiTune.add(tuneFlatDistance.set("Flattening Distance", 20.0, 1., 30.));
    guiTune.add(tuneFlatLimitForce.set("Tune Flattening Force Limit", -3., -10, 10));

    guiTune.add(tuneSpringLength.set("Spring Length", 16.0, 1., 30.));
    guiTune.add(tuneSpringElasticity.set("Tune Spring Elasticity (K factor)", -5., -10, 10));
    guiTune.add(tuneSpringLimitForce.set("Tune Spring Force limit", 0, -10, 10));
    

    gui.add(guiTune);


}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == ' ' ){
		molSystem.reset(false);
	}
    if( key == 'r' ){
		molSystem.reset(true);
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
