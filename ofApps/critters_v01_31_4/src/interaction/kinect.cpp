//
//  kinect.cpp
//
//  Created by Simon Stimberg on 18.07.22.
//

#include "kinect.h"


// constructor
kinectHandler::kinectHandler(){

}

void kinectHandler::setup(vector<float> sizeFactors) {



    nearClip = 650;     // set the near clipping plane in mm
    clippingDepth = 1000;   // set the clipping depth in mm (near clipping + depth = far clipping)

    scanYposition = 240;    // the Y position of the scanline in the kinect image - should be the center to avoid geometric distortion -> kinectHeight = 480 / 2
    borderOffset = 100;      // the offset (in pixel) of the calibration point's position from the canvas borders

    screenRes.x = 800;
    screenRes.y = 600;

    numScreens = sizeFactors.size();

    // save the vessel sizes for obtain the right scaing of the kinect data
    for(int i = 0; i < sizeFactors.size(); i ++) { 
        vesselSizes.push_back( glm::vec2(screenRes.x*sizeFactors[i], screenRes.y*sizeFactors[i]) );
    }

    refPtA.assign(numScreens, glm::vec2(0,0));  // reference point A for calibration
    refPtB.assign(numScreens, glm::vec2(0,0));  // ref point B - we need A+B for each screen



    ofSetLogLevel(OF_LOG_VERBOSE);
    
    

        // ofSleepMillis(100);
        
        // enable depth->video image calibration
        kinect.setRegistration(false);
        
        // kinect.init();
        //kinect.init(true); // shows infrared instead of RGB video image
        kinect.init(false, false); // disable video image (faster fps)


        kinect.open();        // opens first available kinect



        ofSleepMillis(100); 

        if(kinect.isDeviceConnected(0)) { ofLogNotice("device connected!!!"); }

        ofLogNotice("Kinect Dimensions: " + ofToString(kinect.getWidth()) + "x" + ofToString(kinect.getHeight()));

        // // if (kinect.getCurrentCameraTiltAngle() != 30) { kinect.setCameraTiltAngle(30); }      
        ofLogNotice("Current Tilt Angle: " + ofToString(kinect.getCurrentCameraTiltAngle()));


        kinect.setDepthClipping(nearClip, nearClip + clippingDepth);
        
    

    ofSetLogLevel(OF_LOG_NOTICE);


    touchPoints.resize(kinect.getWidth());      // just to allocate enough memory
    mappedPoints.resize(kinect.getWidth()); 

    touchPoints.clear();
    mappedPoints.clear();

    touchPoints.resize(numScreens);     // one touch point array per screen
    mappedPoints.resize(numScreens);




    // load the calibration point data from external file
    if( calSettings.loadFile("calibration.xml") ){
		ofLogNotice("calibration settings loaded!");
	}else{
        ofLogNotice("can't find calibration settings file. default values will be loaded.");
	}

    // assign the data from the file - if the file does not exist, use default values
    for(int i = 0; i < numScreens; i ++) { 
        refPtA[i].x =  calSettings.getValue("LEFT"+ofToString(i), -300);
        refPtB[i].x =  calSettings.getValue("RIGHT"+ofToString(i), 300);
        refPtA[i].y =  calSettings.getValue("TOP"+ofToString(i), nearClip);;
        refPtB[i].y =  calSettings.getValue("BOTTOM"+ofToString(i), nearClip + clippingDepth);
    }


    calPointSwitch = 0;

}


//--------------------------------------------------------------
void kinectHandler::update(){

    kinect.update();
    
    if(kinect.isFrameNew()) {      
        computePoints();
    }

}


//--------------------------------------------------------------
void kinectHandler::computePoints() {

    for (int i = 0; i < numScreens; i ++) {
        touchPoints[i].clear();
        mappedPoints[i].clear();

    }

    // ofLogNotice("touchPoints: " + ofToString(touchPoints.size()));
    // ofLogNotice("yapp");

    vector <bool> blobFound;
    blobFound.assign(numScreens, false);


    // iterate over one entire row of the Kinect image
    for(int x = 0; x < (kinect.getWidth()); x ++) {

        // get the point on that position
        ofPoint probe = kinect.getWorldCoordinateAt(x, scanYposition);

        // do the check for every screen
        for(int v = 0; v < numScreens; v++) {

            // check if the kinect point is within the bounds of the specific screen 
            if (probe.x > refPtB[v].x-borderOffset && probe.x < refPtA[v].x+borderOffset && probe.z > refPtA[v].y-borderOffset && probe.z < refPtB[v].y+borderOffset ) {      // probe.x > refPtB[v].x-borderOffset && probe.x < refPtA[v].x+borderOffset && 

                // if so, map it to the the screen space
                float newY = ofMap(probe.x, refPtA[v].x, refPtB[v].x, vesselSizes[v].y - borderOffset, borderOffset);    // the mapping is inversed to mirror the x axis - also newX & newY are switched for upright mapping
                float newX = ofMap(probe.z, refPtA[v].y, refPtB[v].y, vesselSizes[v].x - borderOffset, borderOffset);

                mappedPoints[v].push_back(glm::vec2(newX, newY));   // the points are detected and stored for each screen seperately

                blobFound[v] = true;

            } else {

                // if there is no more detected points but there were previous points in the array
                if (blobFound[v]) {
                    
                    glm::vec2 newTouchPoint = glm::vec2(0, 0);

                    // get the average of all recent points in a row
                    for (int i = 0; i < mappedPoints[v].size(); i ++) {
                        newTouchPoint += mappedPoints[v][i];
                    }
                    newTouchPoint /= mappedPoints[v].size();

                    // shift points to make them relative to the center of the canvas
                    newTouchPoint.x -= vesselSizes[v].x * 0.5;
                    newTouchPoint.y -= vesselSizes[v].y * 0.5;  

                    // save the average as one touch point
                    touchPoints[v].push_back(glm::vec2(newTouchPoint.x, newTouchPoint.y));
                

                    // reset for the further scanning
                    blobFound[v] = false;
                    mappedPoints[v].clear();

                }

            }

        }
        
    } 

}


//--------------------------------------------------------------
void kinectHandler::drawKinect(int sNum){   // (screen number) draw only the touchoints for the specific screen


    // draw the Kinect image + the scan line
    ofBackground(100);
    ofPushMatrix();
        // ofTranslate(ofGetWidth()*0.5-kinect.getWidth()*0.5, ofGetHeight()*0.5-kinect.getHeight()*0.5);
        // ofTranslate(-kinect.getWidth()*0.5, -kinect.getHeight()*0.5);

        ofSetColor(255);
        kinect.drawDepth(0, 0);
        // kinect.drawDepth(0, 0, vesselSizes[0].x, vesselSizes[0].y);

        ofSetColor(255,0,0);
        ofDrawLine(0, scanYposition, kinect.getWidth(), scanYposition);

    ofPopMatrix();


    ofPushMatrix();
        ofTranslate(vesselSizes[sNum].x*0.5, vesselSizes[sNum].y*0.5);

        float rescaleX = screenRes.x / vesselSizes[sNum].x;
        float rescaleY = screenRes.y / vesselSizes[sNum].y;
    
        // draw the existing touch points
        for(int i = 0; i < touchPoints[sNum].size(); i ++) { 

            ofSetColor(ofColor::skyBlue);
            // ofDrawCircle(touchPoints[i].x * rescaleX, touchPoints[i].y * rescaleY, 10);
            ofDrawCircle(touchPoints[sNum][i], 10);

        }
        
    ofPopMatrix();

}


//--------------------------------------------------------------
void kinectHandler::drawCalibrationAids(int sNum){ 

    
    ofColor status = (calPointSwitch == 0) ? ofColor::indianRed : ofColor::paleGreen;
    ofSetColor(status);
    ofDrawCircle(borderOffset, borderOffset, 10);

    status = (calPointSwitch == 1) ? ofColor::indianRed : ofColor::paleGreen;
    ofSetColor(status);
    ofDrawCircle(vesselSizes[sNum].x - borderOffset, vesselSizes[sNum].y - borderOffset, 10);


}



//--------------------------------------------------------------
void kinectHandler::calibrate(int sNum) {

    kinect.update();


    // get the finger position ...
    vector <glm::vec2> tmpPoints;

    // ... by checking the scan line for depth data ...
    for(int x = 0; x < (kinect.getWidth()); x ++) {

        // get the point on that position
        ofPoint probe = kinect.getWorldCoordinateAt(x, scanYposition);

        // check if it is whithin the bounds
        if (probe.z > nearClip && probe.z < nearClip + clippingDepth ) {
            tmpPoints.push_back(glm::vec2(probe.x, probe.z));
        }

    }

    // ... and calculating the average position
    glm::vec2 averagePoint = glm::vec2(0, 0);

    for (int i = 0; i < tmpPoints.size(); i ++) {
        averagePoint += tmpPoints[i];
    }
    averagePoint /= tmpPoints.size();


    // assign the values to one of the two calibration points - switching points each call
    if (calPointSwitch == 0) {
        refPtB[sNum].x = averagePoint.x;
        refPtB[sNum].y =  averagePoint.y;

        ofLogNotice("Top Left calibration point has been set with: " + ofToString(averagePoint.x) + ", " + ofToString(averagePoint.y));
    } else {
        refPtA[sNum].x =  averagePoint.x;
        refPtA[sNum].y =  averagePoint.y;

        ofLogNotice("Bottom Right calibration point has been set with: " + ofToString(averagePoint.x) + ", " + ofToString(averagePoint.y));
    }

    calPointSwitch = !calPointSwitch;   // to make sure that the next call the other calibration points gets assigned



    // save the calibration points to a XML file to recall them the next app start
    calSettings.setValue("LEFT"+ofToString(sNum), refPtA[sNum].x);
    calSettings.setValue("RIGHT"+ofToString(sNum), refPtB[sNum].x);
    calSettings.setValue("TOP"+ofToString(sNum), refPtA[sNum].y);
    calSettings.setValue("BOTTOM"+ofToString(sNum), refPtB[sNum].y);

    calSettings.saveFile("calibration.xml");

}


void kinectHandler::exit() {
    kinect.close();	
}