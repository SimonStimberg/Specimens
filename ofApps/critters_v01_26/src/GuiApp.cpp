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
    gui.getGroup("Tune Forces").minimize();


    guiIntestine.setName("Tune Intestine Shape");
    guiIntestine.add(tuneMembraneLength.set("Membrane Length", 22.0, 1., 30.));
    guiIntestine.add(tuneMembraneElasticity.set("Membrane Elasticity", -5., -10, 10));
    guiIntestine.add(tuneMembraneLimitForce.set("Membrane Force limit", 0, -10, 10));
    guiIntestine.add(tuneStructureLength.set("Structure Length", 22.0, 1., 30.));
    guiIntestine.add(tuneStructureElasticity.set("Structure Elasticity", -5., -10, 10));
    guiIntestine.add(tuneStructureLimitForce.set("Structure Force limit", 0, -10, 10));
    guiIntestine.add(membraneColor.set("Membrane Color", ofColor(40, 40, 130), ofColor(0, 0), 255));
    gui.add(guiIntestine);
    gui.getGroup("Tune Intestine Shape").minimize();



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
    gui.getGroup("Tune Canvas Shape").minimize();


    guiGeneral.setName("General Settings");
    guiGeneral.add(switchKinectCalibration.set("Kinect Calibration Mode", false));
    guiGeneral.add(switchScreenMask.set("Screen Mask Calibration Mode", false));
    gui.add(guiGeneral);
    gui.getGroup("General Settings").minimize();


    guiTimings.setName("Timings");
    
    guiTimingsCells.setName("Cells");
    guiTimingsCells.add(cellNextGrowth.set("Cell Next Growth", glm::vec2(100, 0.5), glm::vec2(100, 0.), glm::vec2(3000, 1.)));
    guiTimingsCells.add(cellFreqMultiplier.set("Cell Freq Multiplier", 1.0, 1.0, 2.0));
    guiTimings.add(guiTimingsCells);
    
    guiTimingsNeurons.setName("Neurons");
    // guiTimingsNeurons.setFillColor(ofColor::yellowGreen);
    guiTimingsNeurons.add(neuronGrowSpeed.set("Neuron Grow Speed", 500, 100, 3000));
    guiTimingsNeurons.add(neuronSignalInterval.set("Neuron Signal Inteval", 1500, 100, 3000));
    guiTimings.add(guiTimingsNeurons);
    
    guiTimingsIntestines.setName("Intestines");
    guiTimingsIntestines.add(intestineGrowInterval.set("Intestine Grow Interval", 700, 100, 3000.));
    guiTimingsIntestines.add(intestineDigestionInterval.set("Intestine Digestion Interval", 4000, 1000, 7000));
    guiTimingsIntestines.add(intestineDigestionSpeed.set("Intestine Digestion Speed", 300, 100, 1000)); 
    guiTimings.add(guiTimingsIntestines);



    gui.add(guiTimings);

    gui.getGroup("Timings").getGroup("Cells").setHeaderBackgroundColor(ofColor::fromHex(0x2bdbe6));
    gui.getGroup("Timings").getGroup("Cells").setBorderColor(ofColor::fromHex(0x2bdbe6));
    gui.getGroup("Timings").getGroup("Neurons").setHeaderBackgroundColor(ofColor::fromHex(0xfcfdbd));
    gui.getGroup("Timings").getGroup("Neurons").setBorderColor(ofColor::fromHex(0xfcfdbd));
    gui.getGroup("Timings").getGroup("Intestines").setHeaderBackgroundColor(ofColor::fromHex(0xe690a4));
    gui.getGroup("Timings").getGroup("Intestines").setBorderColor(ofColor::fromHex(0xe690a4));






    tuneCanvasWidth.addListener(this, &GuiApp::maskListener);
    tuneCanvasHeight.addListener(this, &GuiApp::maskListener);
    tuneVerticalBow.addListener(this, &GuiApp::maskListener);
    tuneHorizontalBow.addListener(this, &GuiApp::maskListener);
    tuneEdges.addListener(this, &GuiApp::maskListener);
    tuneXpos.addListener(this, &GuiApp::maskListener);
    tuneYpos.addListener(this, &GuiApp::maskListener);
    maskChanged = false;


    // gui.add(mainPtr->synth.gui);

    

	ofBackground(0);
	ofSetVerticalSync(false);

    
    // defaultSimulationPreset = "/Users/simon/Projects/MasterProject/10_GitRepo/CRT-critters/Presets/Simulation/Intestines06.xml";
    defaultSimulationPreset = "../../../../Presets/Simulation/DigestingIntestines01.xml";   
    defaultSynthPreset = "../../../../Presets/Synth/BreathingFilter04.xml";
    loadPreset("default", 1);
    // loadPreset(defaultPreset, 2);

    // string defaultTubeCalibration = "../../../../Presets/TubeCalibrations/HitachiTubeCalibration.xml";
    string defaultTubeCalibration = "../../../../Presets/TubeCalibrations/PhilipsCalibrationVertical.xml";
    loadPreset(defaultTubeCalibration, 1);
}

void GuiApp::update(){

}

void GuiApp::draw(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
	gui.draw();
    // mainPtr->synth.gui.setPosition(300, 10);
    mainPtr->synth.gui.draw();
    mainPtr->synth.guiVoices.draw();

    // ofDrawRectangle(400, 700, 10, mainPtr->synth.getOutputMeter()*100);
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
    if (key == '7')   mainPtr->molSystem[0].addOrganisms(INTESTINE, 1);
    if (key == '8')   mainPtr->molSystem[0].addOrganisms(BREATHER,  1);
    if (key == '9')   mainPtr->molSystem[0].addOrganisms(PUMPER,    1);
    if (key == '0')   mainPtr->molSystem[0].addOrganisms(NEURON,    1);
    
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

