//
//  kinect.cpp
//
//  Created by Simon Stimberg on 18.07.22.
//

#include "kinect.h"


// constructor
kinectHandler::kinectHandler(){

}

void kinectHandler::setup() {



    nearClip = 550;     // set the near clipping plane in mm
    clippingDepth = 400;   // set the clipping depth in mm (near clipping + depth = far clipping)

    scanYposition = 240;    // the Y position of the scanline in the kinect image - should be the center to avoid geometric distortion -> kinectHeight = 480 / 2
    borderOffset = 50;      // the offset (in pixel) of the calibration point's position from the canvas borders




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


    touchPoints.resize(kinect.getWidth());
    mappedPoints.resize(kinect.getWidth());

    touchPoints.clear();
    mappedPoints.clear();




    // load the calibration point data from external file
    if( calSettings.loadFile("calibration.xml") ){
		ofLogNotice("calibration settings loaded!");
	}else{
        ofLogNotice("can't find calibration settings file. default values will be loaded.");
	}

    // assign the data from the file - if the file does not exist, use default values
    xMin =  calSettings.getValue("LEFT", -300);
    xMax =  calSettings.getValue("RIGHT", 300);
    yMin =  calSettings.getValue("TOP", nearClip);;
    yMax =  calSettings.getValue("BOTTOM", nearClip + clippingDepth);


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

    touchPoints.clear();
    mappedPoints.clear();

    bool blobFound = false;
    int dotCount = 0;


    // iterate over one entire row of the Kinect image
    for(int x = 0; x < (kinect.getWidth()); x ++) {

        // get the point on that position
        ofPoint probe = kinect.getWorldCoordinateAt(x, scanYposition);

        // check if it is whithin the bounds
        if (probe.x > xMin-borderOffset && probe.x < xMax+borderOffset && probe.z > yMin-borderOffset && probe.z < yMax+borderOffset ) {

            // map it to the the screen space
            float newX = ofMap(probe.x, xMin, xMax, ofGetWidth()  - borderOffset, borderOffset);
            float newY = ofMap(probe.z, yMin, yMax, ofGetHeight() - borderOffset, borderOffset);

            mappedPoints.push_back(glm::vec2(newX, newY));

            if (!blobFound) blobFound = true;
            dotCount++;

        } else {

            // if there is no more detected points but there were previous points in the array
            if (blobFound) {
                
                glm::vec2 newTouchPoint = glm::vec2(0, 0);

                // get the average of all recent points in a row
                for (int i = 0; i < dotCount; i ++) {
                    int indx = mappedPoints.size() - dotCount + i;
                    newTouchPoint += mappedPoints[indx];
                }
                newTouchPoint /= dotCount;

                // shift points to make them relative to the center of the canvas
                newTouchPoint.x -= ofGetWidth() * 0.5;
                newTouchPoint.y -= ofGetHeight() * 0.5;  

                // save the average as one touch point
                touchPoints.push_back(glm::vec2(newTouchPoint.x, newTouchPoint.y));
            

                // reset for the further scanning
                blobFound = false;
                dotCount = 0;

            }

        }
        
    } 

}


//--------------------------------------------------------------
void kinectHandler::drawKinect(){


    // draw the Kinect image + the scan line
    ofBackground(100);
    ofPushMatrix();
        ofTranslate(ofGetWidth()*0.5-kinect.getWidth()*0.5, ofGetHeight()*0.5-kinect.getHeight()*0.5);
        // ofTranslate(-kinect.getWidth()*0.5, -kinect.getHeight()*0.5);

        ofSetColor(255);
        kinect.drawDepth(0, 0);

        ofSetColor(255,0,0);
        ofDrawLine(0, scanYposition, kinect.getWidth(), scanYposition);

    ofPopMatrix();


    ofPushMatrix();
        ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    
        // draw the existing touch points
        for(int i = 0; i < touchPoints.size(); i ++) { 

            ofSetColor(ofColor::skyBlue);
            ofDrawCircle(touchPoints[i], 10);

        }
        
    ofPopMatrix();

}


//--------------------------------------------------------------
void kinectHandler::drawCalibrationAids(){ 

    
    ofColor status = (calPointSwitch == 0) ? ofColor::indianRed : ofColor::paleGreen;
    ofSetColor(status);
    ofDrawCircle(borderOffset, borderOffset, 10);

    status = (calPointSwitch == 1) ? ofColor::indianRed : ofColor::paleGreen;
    ofSetColor(status);
    ofDrawCircle(ofGetWidth() - borderOffset, ofGetHeight() - borderOffset, 10);


}



//--------------------------------------------------------------
void kinectHandler::calibrate() {

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

        // int indx = mappedPoints.size() - dotCount + i;
        averagePoint += tmpPoints[i];
    }
    averagePoint /= tmpPoints.size();


    // assign the values to one of the two calibration points - switching points each call
    if (calPointSwitch == 0) {
        xMax = averagePoint.x;
        yMax =  averagePoint.y;

        ofLogNotice("Top Left calibration point has been set with: " + ofToString(averagePoint.x) + ", " + ofToString(averagePoint.y));
    } else {
        xMin =  averagePoint.x;
        yMin =  averagePoint.y;

        ofLogNotice("Bottom Right calibration point has been set with: " + ofToString(averagePoint.x) + ", " + ofToString(averagePoint.y));
    }

    calPointSwitch = !calPointSwitch;   // to make sure that the next call the other calibration points gets assigned



    // save the calibration points to a XML file to recall them the next app start
    calSettings.setValue("LEFT", xMin);
	calSettings.setValue("RIGHT", xMax);
	calSettings.setValue("TOP", yMin);
    calSettings.setValue("BOTTOM", yMax);

    calSettings.saveFile("calibration.xml");

}


void kinectHandler::exit() {
    kinect.close();	
}