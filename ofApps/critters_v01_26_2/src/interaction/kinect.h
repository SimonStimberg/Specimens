//
//  kinect.h
//
//  Created by Simon Stimberg on 18.07.22.
//

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxXmlSettings.h"

class kinectHandler{
    
    public:
        // constructor
        kinectHandler();
        
        
        // member functions
        void setup();
        void update();
        void drawKinect();
        void drawCalibrationAids();
        void exit();
        
        void computePoints();
        void calibrate();
        vector<glm::vec2> getTouchPoints() { return touchPoints; };
                
        

        ofxKinect kinect;

        vector <glm::vec2> touchPoints;
        vector <glm::vec2> mappedPoints;


        int nearClip;
        int clippingDepth;    

        int scanYposition;
        int borderOffset;


        int xMin;
        int xMax;
        int yMin;
        int yMax;
    
        bool calPointSwitch;

        ofxXmlSettings calSettings;


    
};
