/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"
#include "ofApp.h"


void GuiApp::setup(){

	gui.setup();
    gui.setPosition(10, 10);
    gui.setSize(280, 280);

    guiTune.setName("Tune Forces");
    guiTune.add(tuneFriction.set("Friction", 0.97, 0.9, 0.98));
    
    guiTune.add(tuneRepulsionThresh.set("Repulsion Radius", 18.0, 1., 70.));
    guiTune.add(tuneRepulsionForce.set("Tune Repulsion Force", 0, -10, 10));

    guiTune.add(tuneFlatThresh.set("Flattening min Thresh", 7.0, 1., 30.));
    guiTune.add(tuneFlatDistance.set("Flattening Distance", 20.0, 1., 30.));
    guiTune.add(tuneFlatLimitForce.set("Tune Flattening Force Limit", -3., -10, 10));

    guiTune.add(tuneSpringLength.set("Spring Length", 22.0, 1., 30.));
    guiTune.add(tuneSpringElasticity.set("Tune Spring Elasticity", -5., -10, 10));
    guiTune.add(tuneSpringLimitForce.set("Tune Spring Force limit", 0, -10, 10));

    guiTune.add(switchOscillation.set("Switch breathing", false));
    guiTune.add(tuneExpansionForce.set("Tune Expansion Force", 0.0, -10., 10.));
    guiTune.add(tuneExpansionRadius.set("Tune Expansion Radius Factor", 3.0, 1., 5.));
    guiTune.add(tuneOscillationAmount.set("Tune Oscillation Amount", 0.1, 0., 0.2));

    guiTune.add(tuneIntrusionThresh.set("Interaction Repul Radius", 62.0, 10., 100.));
    guiTune.add(tuneIntrusionForce.set("Interaction Repul Force", -1.0, -5, 5));

    guiTune.add(switchConnections.set("Switch connections", false));

    gui.add(guiTune);


    guiCanvas.setName("Tune Canvas Shape");
    guiCanvas.add(tuneCanvasWidth.set("Canvas Width", 330, 200, 360));
    guiCanvas.add(tuneCanvasHeight.set("Canvas Height", 240, 200, 288));
    guiCanvas.add(tuneVerticalBow.set("Vertical Bow", 760, 600, 2000));
    guiCanvas.add(tuneHorizontalBow.set("Horizontal Bow", 740, 600, 2000));
    guiCanvas.add(tuneEdges.set("Smooth Edges", 25, 0, 60));
    guiCanvas.add(tuneXpos.set("X Position", 0.0, -50.0, 50.0));
    guiCanvas.add(tuneYpos.set("Y Position", 0.0, -50.0, 50.0));
    guiCanvas.add(tuneRotation.set("Rotation", 0.0, -15.0, 15.0));

    gui.add(guiCanvas);


    guiGeneral.setName("General Settings");
    guiGeneral.add(switchKinectCalibration.set("Kinect Calibration Mode", false));
    guiGeneral.add(switchScreenMask.set("Screen Mask Calibration Mode", false));

    gui.add(guiGeneral);





    tuneCanvasWidth.addListener(this, &GuiApp::maskListener);
    tuneCanvasHeight.addListener(this, &GuiApp::maskListener);
    tuneVerticalBow.addListener(this, &GuiApp::maskListener);
    tuneHorizontalBow.addListener(this, &GuiApp::maskListener);
    tuneEdges.addListener(this, &GuiApp::maskListener);
    maskChanged = false;


    // gui.add(mainPtr->synth.gui);

    

	ofBackground(0);
	ofSetVerticalSync(false);

    // defaultSimulationPreset = "/Users/simon/Projects/MasterProject/10_GitRepo/CRT-critters/Presets/Simulation/Sediment+TVmap+Breathing04.xml";
    defaultSimulationPreset = "/Users/simon/Projects/MasterProject/10_GitRepo/CRT-critters/Presets/Simulation/TouchInterference01.xml";
    defaultSynthPreset = "/Users/simon/Projects/MasterProject/10_GitRepo/CRT-critters/Presets/Synth/BreathingFilter03.xml";
    loadPreset("default", 1);
    // loadPreset(defaultPreset, 2);
}

void GuiApp::update(){

}

void GuiApp::draw(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
	gui.draw();
    // mainPtr->synth.gui.setPosition(300, 10);
    mainPtr->synth.gui.draw();
}

//--------------------------------------------------------------
void GuiApp::savePreset(int panel) {
    if(panel == 1) {
        ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + "." + ofToLower("xml"), "Save SIMULATION parameters");
        if (saveFileResult.bSuccess){
            gui.saveToFile(saveFileResult.filePath);
        }
    } else if(panel == 2) {
        ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + "." + ofToLower("xml"), "Save SYNTH parameters");
        if (saveFileResult.bSuccess){
            mainPtr->synth.gui.saveToFile(saveFileResult.filePath);
        }
    }
}

//--------------------------------------------------------------
void GuiApp::loadPreset(string filePath, int panel)
{
    if (filePath=="default" && panel==1) { filePath = defaultSimulationPreset; } 
    if (filePath=="default" && panel==2) { filePath = defaultSynthPreset; } 

    ofFile presetFile = ofFile(filePath);
    if (presetFile.exists()) {

        string fileExtension = ofToUpper(presetFile.getExtension());
        
        //We only want wav + mp3
        if (fileExtension == "XML" && panel == 1) {
            gui.loadFromFile(filePath);
        } else if (fileExtension == "XML" && panel == 2) {
            mainPtr->synth.gui.loadFromFile(filePath);
        }
    }
}

//--------------------------------------------------------------
void GuiApp::keyPressed(int key)
{
    if (key == '1')
    {
        savePreset(1);
    }
    if (key == '2')
    {
        savePreset(2);
    }
}

//--------------------------------------------------------------
void GuiApp::dragEvent(ofDragInfo dragInfo)
{
    if( dragInfo.files.size() > 0 ){
        // ofFile droppedFile = ofFile(dragInfo.files[0]);
        if(dragInfo.position.x < ofGetWidth()*0.5) {
            loadPreset(dragInfo.files[0], 1);
        } else {
            loadPreset(dragInfo.files[0], 2);
        }
        
    }
}

//--------------------------------------------------------------
void GuiApp::maskListener(float & val)
{
    if(ofGetMousePressed(0) == false && ofGetMousePressed(2) == false) {
        maskChanged = true;
    }
    
}

