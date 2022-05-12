#include "ofApp.h"


shared_ptr<GuiApp> guiPtr;


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetFrameRate(50);

    mask.allocate(ofGetWidth(), ofGetHeight(), GL_LUMINANCE);
    showMask = false;

    molSystem.setup(ofGetWidth(), ofGetHeight());
    molSystem.addCells(7);
    molSystem.addMolecules(300);

    // ofLogNotice("fric: " + ofToString(guiPtr->tuneFriction));
    ofLogNotice("Canvas Size: " + ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight()));

    setTVmask();

}

//--------------------------------------------------------------
void ofApp::update(){

    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    molSystem.update();

    if(guiPtr->maskChanged) { setTVmask(); }
    
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
        if(showMask) {
            // ofTranslate(-ofGetWidth()*0.5, -ofGetHeight()*0.5);
            mask.draw(-ofGetWidth()*0.5, -ofGetHeight()*0.5);
        }
    ofPopMatrix();


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
    if( key == 'x' ){
        molSystem.flush = !molSystem.flush;
	}
    if( key == 'm' ){
        showMask = !showMask;        
    }
    if( key == 'd' ){
        molSystem.debugView = !molSystem.debugView;        
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
