#include "ofApp.h"


shared_ptr<GuiApp> guiPtr;


//--------------------------------------------------------------
void ofApp::setup(){
    
    offset = 60;

    // fbo.allocate(360, 288, GL_RGB);
    fbo.allocate(ofGetWidth()-offset, ofGetHeight()-offset, GL_RGB);

    molSystem.setup(fbo.getWidth(), fbo.getHeight());
    molSystem.addCells(5);
    molSystem.addMolecules(500);

    ofLogNotice("fric: " + ofToString(guiPtr->tuneFriction));


    // INITIALIZE SERIAL COMMUNICATION
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    int baud = 115200;
    serial.setup(0, baud); //open the first device   
    timeCheck = ofGetElapsedTimeMillis();
    refreshRate = 40; // interval sending to Arduino in Milliseconds (40ms = "25fps" -> 1000 / 25)

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
        // ofTranslate(fbo.getWidth()*0.5, fbo.getHeight()*0.5);
        // molSystem.draw();
        // ofPopMatrix();

    fbo.end();


    // float oscillate = (sin(ofGetElapsedTimef())+1) * 0.5;
    // ofLogNotice(ofToString(oscillate));
    // sendToESP(molSystem.molecules);
}

//--------------------------------------------------------------
void ofApp::draw(){

    // ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    ofBackground(0);

    ofPushMatrix();
        ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
        molSystem.draw();
        

        // fbo.draw(0, 0, 720, 576);
        // fbo.draw(0, 0);
        // fbo.draw(offset/2, offset/2);

    ofPopMatrix();

}


//--------------------------------------------------------------
void ofApp::sendToESP(vector <Molecule *> allMolecules) {
    
    
    // only send new command at a certain time interval (refreshRate)
    
    if (timeCheck + refreshRate < ofGetElapsedTimeMillis()){
        
        // uint8_t commandMsg[247];
        string commandMsg = "";
        
        
        // set a byte with value 127
        // this serves as "starting flag" - means the starting of a new command string
        
        // commandMsg += (uint8_t)255;
        // commandMsg[0] = (uint8_t)255;
        commandMsg += (char)127;
        
        
        for(int i = 0; i < 3; i++) {
            
            // map the intensity so that it fits into one byte
            // (minus the two values serving as starting and ending flags (127 + 126)

            glm::vec2 position = allMolecules[i]->position;
            
            int mappedX = ofMap(position.x, -molSystem.worldSize.x * 0.5, molSystem.worldSize.x * 0.5, 0, 125, true);   // map to 253 for full uint8_t 8 bit
            int mappedY = ofMap(position.y, -molSystem.worldSize.y * 0.5, molSystem.worldSize.y * 0.5, 0, 125, true);


            // mapped = ofClamp(mapped, 0, 125);  // for safety
            
            
            // convert the integer into a byte (char) and add it to the string
            
            // uint8_t byte = (uint8_t)mapped;
            // commandMsg[i] = (uint8_t)mappedX;
            // commandMsg[i+1] = (uint8_t)mappedY;

            // char byte = (char)mapped;
            commandMsg += (char)mappedX;
            commandMsg += (char)mappedY;
            
        }
        
        
        // set a byte with value 126 -> means ending of a command string
        
        // commandMsg[246] = (uint8_t)254;
        commandMsg += (char)126;
        //        ofLog(OF_LOG_NOTICE, ofToString(commandMsg));
        
        // ofLogNotice(ofToString(commandMsg));
        // write the command string into the serial buffer
        
        const char *buffer = commandMsg.c_str();
        serial.writeBytes(buffer, commandMsg.size());
        
        
        timeCheck = ofGetElapsedTimeMillis();
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == ' ' ){
		molSystem.reset(true);
	}
    if( key == 'r' ){
		molSystem.reset(false);
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
