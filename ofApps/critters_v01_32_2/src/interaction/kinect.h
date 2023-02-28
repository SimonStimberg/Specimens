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
        void setup(vector<float> sizeFactors);
        void update();
        void drawKinect(int sNum);
        void drawCalibrationAids(int sNum);
        void exit();
        
        void computePoints();
        void calibrate(int sNum);
        vector<glm::vec2> getTouchPoints(int sNum) { return touchPoints[sNum]; };
        glm::vec2 getTriggerPoint(int sNum);
                
        

        ofxKinect kinect;
        int numScreens;

        vector <vector <glm::vec2>> touchPoints;
        vector <vector <glm::vec2>> mappedPoints;
        vector <bool> triggered;
        vector <unsigned int> triggerTimestamp;


        int nearClip;
        int clippingDepth;    

        int scanYposition;
        int borderOffset;

        vector <glm::vec2> vesselSizes;
        glm::vec2 screenRes;


        vector <glm::vec2> refPtA;
        vector <glm::vec2> refPtB;
        int xMin;
        int xMax;
        int yMin;
        int yMax;
    
        bool calPointSwitch;

        ofxXmlSettings calSettings;


    
};
