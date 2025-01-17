#include "ofApp.h"

// uncomment this if the simulation is to be shown on CRT screens
#define SHOW_ON_CRT


shared_ptr<GuiApp> guiPtr;  // pointer to the gui app for the exchange of control parameters


//--------------------------------------------------------------
void ofApp::setup() {
    
    // ofSetVerticalSync(true);
    ofSetFrameRate(50);

    singleSpeciesMode = true;


    // initSynth();
    audioMaster.setup(numScreens);



    // sets the native resolution of the screens that are rendered to
    screenResolution.x = 800; 
    screenResolution.y = 600; 


    // a resizing factor to account for different CRT screen sizes 
    // as the resolution stays the same for all screens, the depicted graphics would be perceived with different sizes
    vector<float> screenSizeFactor;
    screenSizeFactor.assign(4 ,1.0f);

    // define the desired vessel size for each screen (the installation consists of four screens)
    // shrinking the native resolution by a custom factor
    screenSizeFactor[0] = 0.8 * 0.85;    // Sanyo Ultraschall      // make this screen 80% the size of the native resolution 800px -> 640px
    screenSizeFactor[1] = 0.8 * 0.85;   // Hitachi            // make the smaller screen even only 85% of the bigger screen to match the perceived size
    screenSizeFactor[2] = 0.8 * 0.8;   // sanyo small 
    screenSizeFactor[3] = 0.8 * 0.9;   // Philips      0.8 * 0.95


    // INITIALIZE THE SCREEN BUFFERS AND THE MOLECULAR SYSTEM FOR EACH SCREEN
    for (int i = 0; i < numScreens; i++) {

        // anti-aliasing FBOs by multisampling! watch out: needs more performance
        ofFbo::Settings settings;
            settings.numSamples = 4;    // also try 8, if your GPU supports it
            settings.useDepth = false; 
            settings.width = screenResolution.x * screenSizeFactor[i];
            settings.height = screenResolution.y * screenSizeFactor[i];

        vessel[i].allocate(settings);   // the frame buffer is called vessel as it contains the rendered Molecular System


        // mask for mapping the screen shape
        mask[i].allocate(vessel[i].getWidth(), vessel[i].getHeight(), GL_LUMINANCE);


        // initialize the Molecular System  
        // specify which organism types to spawn exclusively type 1-4 // 5 for all ("none")
        int setSpecies = (singleSpeciesMode) ? i+1 : 5;
        molSystem[i].setup(vessel[i].getWidth(), vessel[i].getHeight(), setSpecies);  

        molSystem[i].linkAudio( audioMaster.getSubMasterModule(i) );  

        // ROUTE THE AUDIO from the each Molecular System to a FX/Master bus for each screen
        // the Molecular System creates stems for each organism type
        // molSystem[i].masterBus.ch(0) >> fxBus.ch(i);      // channel1: breathers
        // molSystem[i].masterBus.ch(1) >> cleanBus.ch(i);     // channel2: pumpers
        // molSystem[i].masterBus.ch(2) >> fxBus.ch(i);      // channel3: neurons
        // molSystem[i].masterBus.ch(3) >> cleanBus.ch(i);     // channel4: intestines

        // compressor[i].ch(0) >> sumBus.ch(i);
        // compressor[i].ch(1) >> fxBus.ch(i);
    
        // molSystem[i].blackhole >> engine.blackhole();  
        // molSystem[i].blackhole >> gain.ch(3);


        // add organisms or liquid (free floating particles) to the Molecular System
        molSystem[i].addInitialDrop(2);
        // molSystem[i].addOrganisms(LIQUID,    floor(250 * screenSizeFactor[i]) );
        // molSystem[i].addOrganisms(BREATHER,  2);
        // molSystem[i].addOrganisms(PUMPER,    1);
        // molSystem[i].addOrganisms(NEURON,    3);
        // molSystem[i].addOrganisms(INTESTINE, 3);

        ofLogNotice("Screen No. " + ofToString(i) + " with size: " + ofToString(vessel[i].getWidth()) + "x" + ofToString(vessel[i].getHeight()));
    }

    
    setTVmask();
    showMask = false;

    kinectToPoints.setup(screenSizeFactor);


    ofLogNotice("Gain -47dB: " + ofToString(dB(-47)));

    #ifdef SHOW_ON_CRT
        ofHideCursor();     // hide mouse cursor
    #endif

}



//--------------------------------------------------------------
void ofApp::update() {


    if (mouseDown) {
        // ofLogNotice("still pressed");
        if (ofGetElapsedTimeMillis() > mouseDownTime + 5000 ) {

            if(mouseDownButton == 2) {
                ofLogNotice("shutdown please");
                // ofSystem("sudo -S shutdown -h now < /Users/pablo/masterpw.txt");
                // ofSystem("sudo -S shutdown -h now < ./masterpw.txt");
                ofSystem("echo \"samsara\" | sudo -S shutdown -h now");
            }
            ofExit();
        }
    }


    #ifdef SHOW_ON_CRT

        // USE THIS FOR KINECT INTERACTION
        kinectToPoints.update();

        // ofLogNotice("Touchpoints Scrn 0: " + ofToString(kinectToPoints.getTouchPoints(0)));
        // ofLogNotice("Touchpoints Scrn 1: " + ofToString(kinectToPoints.getTouchPoints(1)));
        // ofLogNotice("Touchpoints Scrn 2: " + ofToString(kinectToPoints.getTouchPoints(2)));
        // ofLogNotice("Touchpoints Scrn 3: " + ofToString(kinectToPoints.getTouchPoints(3)));

    #else

        // USE THIS FOR MOUSE INTERACTION   
        vector <glm::vec2> mousePos;
        mousePos.assign(1, glm::vec2(ofGetMouseX() - ofGetWidth() * 0.5, ofGetMouseY() - ofGetHeight() * 0.5) );

    #endif


    // UPDATE THE MOLECULAR SYSTEM OF EACH SCREEN
    for (int i = 0; i < numScreens; i++) {


        #ifdef SHOW_ON_CRT

            // set kinect output as intrusion points for interaction
            molSystem[i].setIntrusionPoints(kinectToPoints.getTouchPoints(i));

        #else

            // set mouse position as intrusion points for interaction
            float scalingFactor = vessel[i].getWidth() / screenResolution.x;
            mousePos[0].x = (ofGetMouseX() - screenResolution.x * 0.5 ) * scalingFactor - vessel[i].getWidth() * i;
            mousePos[0].y = (ofGetMouseY() - screenResolution.y * 0.5 ) * scalingFactor;
            
            float angle = glm::radians(-90.0);
            // mousePos[0] = glm::rotate(mousePos[0], angle);
            
            molSystem[i].setIntrusionPoints(mousePos);

        #endif

        if(singleSpeciesMode) {
            glm::vec2 check = kinectToPoints.getTriggerPoint(i);
            if(check != glm::vec2(0, 0) && !molSystem[i].flush) molSystem[i].addControlledRandom(check.x, check.y);
        }
        
        molSystem[i].update();


        // draw the Molecular System to the frame buffer
        vessel[i].begin();
        
            ofBackground(0);
            if(molSystem[i].flush && molSystem[i].flushTimestamp + 50 > ofGetElapsedTimeMillis() ) ofBackground(200); 

            // draw the Molecular System
            ofPushMatrix();
                ofTranslate(vessel[i].getWidth()*0.5, vessel[i].getHeight()*0.5);   // translate to the center of the screen
                molSystem[i].draw();
            ofPopMatrix();


            // draw mask for screen shape mapping when activated
            if(guiPtr->switchScreenMask) {
                mask[i].draw(0, 0);
            }

            
            // draw kinect callibration guides
            if(guiPtr->switchKinectCalibration) {
                int iScreen = floor(ofGetMouseX()/ screenResolution.x);
                if(iScreen == i) {
                    kinectToPoints.drawKinect(i);
                    kinectToPoints.drawCalibrationAids(i);
                }
            }      
        
        vessel[i].end();



        // if(molSystem[i].flush) {
        //     1.0f >> disasterFX.distortion_ch(i);
        // } else {
        //     float pressure = molSystem[i].getSystemPressure();
        //     pressure * pressure * 0.65 >> disasterFX.reverb_ch(i);
        //     0.0f >> disasterFX.distortion_ch(i);
        // }

        
        audioMaster.switchDistortion(molSystem[i].flush, i);

        if(molSystem[i].flush) {
            audioMaster.switchDistortion(true, i);
        } else {
            float pressure = molSystem[i].getSystemPressure();
            pressure = pressure * pressure * 0.65;
            audioMaster.setReverbAmount(pressure, i);
            
            audioMaster.switchDistortion(false, i);
        }





        if(guiPtr->maskChanged) { setTVmask(); }



    }


    // if(molSystem[0].flush) 1.0f >> disasterFX.distortion_ch(0);
    // float pressure = molSystem[0].getSystemPressure();
    // // pressure * pressure * 0.65 >> disasterFX.reverb_ch0();
    // pressure * pressure * 0.65 >> disasterFX.reverb_ch(0);
    // // 1.0 >> disasterFX.reverb_ch0();

    // ofLogNotice("molecules: " + ofToString(molSystem[3].allMolecules.size()));
    // ofLogNotice("springs: " + ofToString(molSystem[3].allSprings.size()));
    // ofLogNotice("liquid: " + ofToString(molSystem[3].liquid.size()));
    // ofLogNotice("breathers: " + ofToString(molSystem[3].breathers.size()));
    // ofLogNotice("pumpers: " + ofToString(molSystem[3].pumpers.size()));
    // ofLogNotice("neurons: " + ofToString(molSystem[3].neurons.size()));
    // ofLogNotice("neuroconnect: " + ofToString(molSystem[3].neuronConnections.size()));
    // ofLogNotice("intestines: " + ofToString(molSystem[3].intestines.size()));
    
}



//--------------------------------------------------------------
void ofApp::draw(){


    ofPushMatrix();

        #ifndef SHOW_ON_CRT
            // simulate upright orientation of the CRT screens when in window mode
            // ofTranslate(600, 0);
            // ofRotateDeg(90);
        #endif

        // draw the screen buffers side by side to the canvas and scale it up to the native screen resolution
        for (int i = 0; i < numScreens; i++) {
            int xShift = i * screenResolution.x;
            vessel[i].draw(xShift, 0, screenResolution.x, screenResolution.y);
            // vessel[i].draw(xShift, 0);
        }   

        

        #ifndef SHOW_ON_CRT
            int numMolecules = 0;
            int itrPts = 0;
            for (int i = 0; i < numScreens; i++) {
                numMolecules += molSystem[i].allMolecules.size();
                itrPts += kinectToPoints.getTouchPoints(i).size();

                maxBreathers = (molSystem[i].breathers.size() > maxBreathers) ? molSystem[i].breathers.size() : maxBreathers;
                maxPumpers = (molSystem[i].pumpers.size() > maxPumpers) ? molSystem[i].pumpers.size() : maxPumpers;
                maxNeurons = (molSystem[i].neurons.size() > maxNeurons) ? molSystem[i].neurons.size() : maxNeurons;
                maxIntestines = (molSystem[i].intestines.size() > maxIntestines) ? molSystem[i].intestines.size() : maxIntestines;
            }
            ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
            ofPushMatrix();
                string infoTxt = "fps: " + ofToString(ofGetFrameRate()) + "\nnum Molecules: " + ofToString(numMolecules) + "\nIntrusion Points: " + ofToString(itrPts) + "\n\nmax num Breathers: " + ofToString(maxBreathers) + "\nmax num Pumpers: " + ofToString(maxPumpers) + "\nmax num Neurons: " + ofToString(maxNeurons) + "\nmax num Intestines: " + ofToString(maxIntestines);
                ofRotateDeg(-90);
                ofDrawBitmapString(infoTxt, -500, ofGetWidth()-700);
                // ofDrawBitmapString(infoTxt, 50, 50);
            ofPopMatrix();
        #endif


        // ofSetColor(ofColor::aliceBlue);
        // ofFill();
        // ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 4.);

    ofPopMatrix();

}



//--------------------------------------------------------------
void ofApp::setTVmask() {

    
    // set the screen mask for mapping the CRT screen shape
    for (int s = 0; s < numScreens; s++) {
        ofPixels pixels;
        mask[s].readToPixels(pixels);

        Molecule * m = new Molecule(&molSystem[s]);
        m->reset(0,0,0,0);

        for(int i = 0; i < pixels.size(); i++) {
            
            float y = floor(i/vessel[s].getWidth());
            float x = i - y * vessel[s].getWidth();
            y -= vessel[s].getHeight() * 0.5;
            x -= vessel[s].getWidth()  * 0.5;
  
            // get the shape from the signed distance field in the Molecule class
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
void ofApp::keyPressed(int key){  
    
    if( key == OF_KEY_RETURN && guiPtr->switchKinectCalibration ){
        int iScreen = floor(ofGetMouseX() / screenResolution.x);
        
        if (iScreen % 2 == 0) {
            kinectToPoints.calibrateKinect(iScreen);       
        } else {
            kinectToPoints.calibrateLeap(iScreen);         
        }
    }
    if( key == ' ' ) {
        ofToggleFullscreen();
    }

    if( key == 'c' && guiPtr->switchKinectCalibration ){
        // kinectToPoints.calibrateLeap();        
    }


    if (key == '1')   molSystem[0].addOrganisms(BREATHER,  5);
    if (key == '2')   molSystem[0].addOrganisms(PUMPER,    5);
    if (key == '3')   molSystem[0].addOrganisms(NEURON,    5);
    if (key == '4')   molSystem[0].addOrganisms(INTESTINE,  5);
    if (key == '5')   molSystem[0].addOrganisms(LIQUID,  20);


    // if( key == 'x' ) { 
    //     for(int i = 0; i < molSystem[3].breathers.size(); i++) {
    //         0.0 >> molSystem[3].breathers[i]->audioModule.in_bypass();
    //     }

    // }

    if( key == 'm' ) { 
        ofLogNotice("system shutdown please per keypress");
        // ofSystem("sudo -S shutdown -h now < /Users/pablo/masterpw.txt");

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

    if (!mouseDown) {
        mouseDown = true;
        mouseDownButton = button;
        mouseDownTime = ofGetElapsedTimeMillis();
        // ofLogNotice("first press");
    } 

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){


    if(guiPtr->switchKinectCalibration) {
        int iScreen = floor(ofGetMouseX()/ screenResolution.x);
        if (iScreen % 2 == 0) {
            kinectToPoints.calibrateKinect(iScreen);       
        } else {
            kinectToPoints.calibrateLeap(iScreen);         
        }
        ofLogNotice("screenNumber: " + ofToString(iScreen));
        ofLogNotice("mousePosX: " + ofToString(ofGetMouseX()));      
    } else {

        // INTERACTION
        // SPAWN A RANDOM ORGANISM ON MOUSE CLICK
        // the same will happen if a person's finger will rest at one position for a longer time

        #ifndef SHOW_ON_CRT 

            int screenIdx = floor(x / screenResolution.x);
            float scalingFactor = vessel[screenIdx].getWidth() / screenResolution.x;
            float xScaled = (x - screenResolution.x * 0.5 ) * scalingFactor - vessel[screenIdx].getWidth() * screenIdx;     // vessel[i].getWidth()
            float yScaled = (y - screenResolution.y * 0.5 ) * scalingFactor;

            int screenID = floor(ofGetMouseX()/ screenResolution.x);

            // molSystem[screenID].addRandom(xScaled, yScaled);
            molSystem[screenID].addControlledRandom(xScaled, yScaled);
            // molSystem[screenID].addLiquid(xScaled, yScaled);

            // float probability[4] = {0.2, 0.4, 0.95, 1.0};   // the probability for the different organism types
            // float dice = ofRandom(1.);

            // if (dice < probability[0]) {
            //     molSystem[screenIdx].addBreather(xScaled, yScaled);
            // } else if (dice < probability[1]) {
            //     molSystem[screenIdx].addPumper(xScaled, yScaled);
            // } else if (dice < probability[2]) {
            //     molSystem[screenIdx].addNeuron(xScaled, yScaled);
            // } else {
            //     molSystem[screenIdx].addIntestine(xScaled, yScaled);
            // }

        #endif

    }

    mouseDown = false;
    // ofLogNotice("mouse press released");

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
