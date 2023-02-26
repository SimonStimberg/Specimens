#include "ofApp.h"

// uncomment this if the simulation is to be shown on CRT screens
#define SHOW_ON_CRT


shared_ptr<GuiApp> guiPtr;  // pointer to the gui app for the exchange of control parameters


//--------------------------------------------------------------
void ofApp::setup() {
    
    // ofSetVerticalSync(true);
    ofSetFrameRate(50);


    initSynth();

    // sets the native resolution of the screens that are rendered to
    screenResolution.x = 800;
    screenResolution.y = 600;


    // a resizing factor to account for different CRT screen sizes 
    // as the resolution stays the same for all screens, the depicted graphics would be perceived with different sizes
    vector<float> screenSizeFactor;
    screenSizeFactor.assign(4 ,1.0f);

    // define the desired vessel size for each screen (the installation consists of four screens)
    // shrinking the native resolution by a custom factor
    screenSizeFactor[0] = 0.8 * 0.8;    // sanyo small      // make this screen 80% the size of the native resolution 800px -> 640px
    screenSizeFactor[1] = 0.8 * 0.85;   // Hitachi          // make the smaller screen even only 85% of the bigger screen to match the perceived size
    screenSizeFactor[2] = 0.8 * 0.85;   // Sanyo Ultraschall
    screenSizeFactor[3] = 0.8 * 0.95;   // Philips 


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
        molSystem[i].setup(vessel[i].getWidth(), vessel[i].getHeight());    

        // ROUTE THE AUDIO from the each Molecular System to a FX/Master bus for each screen
        // the Molecular System creates stems for each organism type
        molSystem[i].masterBus.ch(0) >> fxBus.ch(i);      // channel1: breathers
        molSystem[i].masterBus.ch(1) >> cleanBus.ch(i);     // channel2: pumpers
        molSystem[i].masterBus.ch(2) >> fxBus.ch(i);      // channel3: neurons
        molSystem[i].masterBus.ch(3) >> cleanBus.ch(i);     // channel4: intestines

        // compressor[i].ch(0) >> sumBus.ch(i);
        // compressor[i].ch(1) >> fxBus.ch(i);
    
        molSystem[i].blackhole >> engine.blackhole();  


        // add organisms or liquid (free floating particles) to the Molecular System
        molSystem[i].addOrganisms(LIQUID,    floor(250 * screenSizeFactor[i]) );
        // molSystem[i].addOrganisms(BREATHER,  2);
        // molSystem[i].addOrganisms(PUMPER,    1);
        // molSystem[i].addOrganisms(NEURON,    3);
        // molSystem[i].addOrganisms(INTESTINE, 3);

        ofLogNotice("Screen No. " + ofToString(i) + " with size: " + ofToString(vessel[i].getWidth()) + "x" + ofToString(vessel[i].getHeight()));
    }

    
    setTVmask();
    showMask = false;

    kinectToPoints.setup(screenSizeFactor);


    ofLogNotice("Gain 12dB: " + ofToString(dB(12)));

}



//--------------------------------------------------------------
void ofApp::update() {


    // USE THIS FOR KINECT INTERACTION
    kinectToPoints.update();
    // molSystem[0].setIntrusionPoints(kinectToPoints.getTouchPoints());

    ofLogNotice("Touchpoints Scrn 0: " + ofToString(kinectToPoints.getTouchPoints(0)));
    ofLogNotice("Touchpoints Scrn 1: " + ofToString(kinectToPoints.getTouchPoints(1)));
    ofLogNotice("Touchpoints Scrn 2: " + ofToString(kinectToPoints.getTouchPoints(2)));
    ofLogNotice("Touchpoints Scrn 3: " + ofToString(kinectToPoints.getTouchPoints(3)));
    // USE THIS FOR MOUSE INTERACTION   
    // vector <glm::vec2> mousePos;
    // mousePos.assign(1, glm::vec2(ofGetMouseX() - ofGetWidth() * 0.5, ofGetMouseY() - ofGetHeight() * 0.5) );


    // UPDATE THE MOLECULAR SYSTEM OF EACH SCREEN
    for (int i = 0; i < numScreens; i++) {
        
        // float scalingFactor = vessel[i].getWidth() / screenResolution.x;
        // mousePos[0].x = (ofGetMouseX() - screenResolution.x * 0.5 ) * scalingFactor - vessel[i].getWidth() * i;     // vessel[i].getWidth()
        // mousePos[0].y = (ofGetMouseY() - screenResolution.y * 0.5 ) * scalingFactor;

        // set mouse position (or kinect output) as intrusion points for interaction
        // molSystem[i].setIntrusionPoints(mousePos);

        

        molSystem[i].setIntrusionPoints(kinectToPoints.getTouchPoints(i));
        glm::vec2 check = kinectToPoints.getTriggerPoint(i);
        if(check != glm::vec2(0, 0) && !molSystem[i].flush) molSystem[i].addRandom(check.x, check.y);
        
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



        if(molSystem[i].flush) {
            1.0f >> disasterFX.distortion_ch(i);
        } else {
            float pressure = molSystem[i].getSystemPressure();
            pressure * pressure * 0.65 >> disasterFX.reverb_ch(i);
            0.0f >> disasterFX.distortion_ch(i);
        }





        if(guiPtr->maskChanged) { setTVmask(); }



    }


    // if(molSystem[0].flush) 1.0f >> disasterFX.distortion_ch(0);
    // float pressure = molSystem[0].getSystemPressure();
    // // pressure * pressure * 0.65 >> disasterFX.reverb_ch0();
    // pressure * pressure * 0.65 >> disasterFX.reverb_ch(0);
    // // 1.0 >> disasterFX.reverb_ch0();

    ofLogNotice("molecules: " + ofToString(molSystem[3].allMolecules.size()));
    ofLogNotice("springs: " + ofToString(molSystem[3].allSprings.size()));
    ofLogNotice("liquid: " + ofToString(molSystem[3].liquid.size()));
    ofLogNotice("breathers: " + ofToString(molSystem[3].breathers.size()));
    ofLogNotice("pumpers: " + ofToString(molSystem[3].pumpers.size()));
    ofLogNotice("neurons: " + ofToString(molSystem[3].neurons.size()));
    ofLogNotice("neuroconnect: " + ofToString(molSystem[3].neuronConnections.size()));
    ofLogNotice("intestines: " + ofToString(molSystem[3].intestines.size()));
    
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

        

        #ifdef SHOW_ON_CRT
            int numMolecules = 0;
            int itrPts = 0;
            for (int i = 0; i < numScreens; i++) {
                numMolecules += molSystem[i].allMolecules.size();
                itrPts += kinectToPoints.getTouchPoints(i).size();
            }
            ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
            ofPushMatrix();
                ofRotateDeg(-90);
                string infoTxt = "fps: " + ofToString(ofGetFrameRate()) + "\nnum Molecules: " + ofToString(numMolecules) + "\nIntrusion Points: " + ofToString(itrPts);
                ofDrawBitmapString(infoTxt, -500, ofGetWidth()-700);
            ofPopMatrix();
        #endif


        ofSetColor(ofColor::aliceBlue);
        ofFill();
        ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 4.);

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
void ofApp::initSynth() {

    // AUDIO SETUP



    // set the gain for each bus
    fxBus.set(1.0);
    cleanBus.set(1.0);
    sumBus.set(1.0);
    // guiPtr->masterGain >> gain;

    guiPtr->lowCutFreq >> loCut.in_freq();  // low cut frequency
    compressor.resize(4);
    for(int i = 0; i < compressor.size(); i++) {
        compressor[i].stereoLink(false);
        guiPtr->compThreshold >> compressor[i].in_threshold();
        guiPtr->compKnee      >> compressor[i].in_knee();
    }



    // FX CHAIN ROUTING

    cleanBus.ch(0) >> compressor[0].ch(0) >> sumBus.ch(0);
    cleanBus.ch(1) >> compressor[1].ch(0) >> sumBus.ch(1);
    cleanBus.ch(2) >> compressor[2].ch(0) >> sumBus.ch(2);
    cleanBus.ch(3) >> compressor[3].ch(0) >> sumBus.ch(3);

    fxBus.ch(0)    >> compressor[0].ch(1) >> chorus.ch(0) >> delay.ch(0) >> sumBus.ch(0);     // one FX bus per Screen
    fxBus.ch(1)    >> compressor[1].ch(1) >> chorus.ch(1) >> delay.ch(1) >> sumBus.ch(1);
    fxBus.ch(2)    >> compressor[2].ch(1) >> chorus.ch(2) >> delay.ch(2) >> sumBus.ch(2);
    fxBus.ch(3)    >> compressor[3].ch(1) >> chorus.ch(3) >> delay.ch(3) >> sumBus.ch(3); 





    // ALL FX BYPASS ROUTING

    // fxBus.ch(0) >> sumBus.ch(0);
    // fxBus.ch(1) >> sumBus.ch(1);
    // fxBus.ch(2) >> sumBus.ch(2);
    // fxBus.ch(3) >> sumBus.ch(3);

    sumBus.ch(0) >> disasterFX.ch(0) >> loCut.ch(0) >> gain.ch(0);
    sumBus.ch(1) >> disasterFX.ch(1) >> loCut.ch(1) >> gain.ch(1); 
    sumBus.ch(2) >> disasterFX.ch(2) >> loCut.ch(2) >> gain.ch(2);
    sumBus.ch(3) >> disasterFX.ch(3) >> loCut.ch(3) >> gain.ch(3); 



    // FX SYSTEM COLLAPSE ROUTING

    // mix the reverb according to the system pressure level
    // sumBus.ch(0) >> mix.in_A();
    // sumBus.ch(0) >> reverb >> mix.in_B(); 
    // mixFader >> mix.in_fade();

    // switch routing to the distotion chain once the system collapses
    // switcher.resize(2);
    // mix >> switcher.input(0);
    // mix >> bitCrush * 4.0f >> saturator * dB(-3) >> switcher.input(1);

    // switcher >> gain.ch(0); 
    // 0.0f >> switcher.in_select();


    // gain.ch(0) >> gain.ch(1);      // for temporary stereo monitoring if the simulation runs with one screen only



    // MAIN OUTPUT
    // one mono channel per screen
    gain.ch(0) >> engine.audio_out(0);
    gain.ch(1) >> engine.audio_out(1);
    gain.ch(2) >> engine.audio_out(2);
    gain.ch(3) >> engine.audio_out(3);


        

     //------------SETUPS AND START AUDIO-------------
    engine.listDevices();

    // defines the audio device used for output
    #ifdef SHOW_ON_CRT
        engine.setDeviceID(4);
    #else
        engine.setDeviceID(5);  // THIS HAS TO BE SET THIS AT THE RIGHT INDEX!!!!   the ID for the audio devices can will be shown in the console on program start
    #endif
    // engine.setChannels (0, 2);  // two channel setup
    engine.setChannels (0, 4);  // four channel setup
    engine.setup( 44100, 512, 3); 
  

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){  
    
    if( key == OF_KEY_RETURN && guiPtr->switchKinectCalibration ){
        int iScreen = floor(ofGetMouseX()/ screenResolution.x);
        kinectToPoints.calibrate(iScreen);        
    }
    if( key == ' ' ) {
        ofToggleFullscreen();
    }


    if (key == '1')   molSystem[0].addOrganisms(BREATHER,  5);
    if (key == '2')   molSystem[0].addOrganisms(PUMPER,    5);
    if (key == '3')   molSystem[0].addOrganisms(NEURON,    5);
    if (key == '4')   molSystem[0].addOrganisms(INTESTINE,  5);
    if (key == '5')   molSystem[0].addOrganisms(LIQUID,  20);


  
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


    // INTERACTION
    // SPAWN A RANDOM ORGANISM ON MOUSE CLICK
    // the same will happen if a person's finger will rest at one position for a longer time

    int screenIdx = floor(x / screenResolution.x);
    float scalingFactor = vessel[screenIdx].getWidth() / screenResolution.x;
    float xScaled = (x - screenResolution.x * 0.5 ) * scalingFactor - vessel[screenIdx].getWidth() * screenIdx;     // vessel[i].getWidth()
    float yScaled = (y - screenResolution.y * 0.5 ) * scalingFactor;

    int screenID = floor(ofGetMouseX()/ screenResolution.x);

    molSystem[screenID].addRandom(xScaled, yScaled);

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
