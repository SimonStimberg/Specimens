/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"

void GuiApp::setup(){

	gui.setup();
    gui.setPosition(10, 10);
    gui.setSize(280, 280);

    guiTune.setName("Tune Forces");
    guiTune.add(tuneFriction.set("Friction", 0.97, 0.9, 0.98));
    
    guiTune.add(tuneRepulsionThresh.set("Repulsion Radius", 18.0, 1., 30.));
    guiTune.add(tuneRepulsionForce.set("Tune Repulsion Force", 0, -10, 10));

    guiTune.add(tuneFlatThresh.set("Flattening min Thresh", 7.0, 1., 30.));
    guiTune.add(tuneFlatDistance.set("Flattening Distance", 20.0, 1., 30.));
    guiTune.add(tuneFlatLimitForce.set("Tune Flattening Force Limit", -3., -10, 10));

    guiTune.add(tuneSpringLength.set("Spring Length", 22.0, 1., 30.));
    guiTune.add(tuneSpringElasticity.set("Tune Spring Elasticity", -5., -10, 10));
    guiTune.add(tuneSpringLimitForce.set("Tune Spring Force limit", 0, -10, 10));

    guiTune.add(switchOscillation.set("Switch breathing", false));

    guiTune.add(tuneIntrusionThresh.set("Interaction Repul Radius", 62.0, 10., 100.));
    guiTune.add(tuneIntrusionForce.set("Interaction Repul Force", -1.0, -5, 5));

    gui.add(guiTune);


    guiCanvas.setName("Tune Canvas Shape");
    guiCanvas.add(tuneCanvasWidth.set("Canvas Width", 330, 200, 360));
    guiCanvas.add(tuneCanvasHeight.set("Canvas Height", 240, 200, 288));
    guiCanvas.add(tuneVerticalBow.set("Vertical Bow", 760, 400, 1200));
    guiCanvas.add(tuneHorizontalBow.set("Horizontal Bow", 740, 400, 1000));
    guiCanvas.add(tuneEdges.set("Smooth Edges", 25, 10, 70));

    gui.add(guiCanvas);



	ofBackground(0);
	ofSetVerticalSync(false);

    string defaultPreset = "/Users/simon/Projects/MasterProject/Simulation Presets/BubblesInSediment_Preset01.xml";
    loadPreset(defaultPreset);
}

void GuiApp::update(){

}

void GuiApp::draw(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
	gui.draw();
}

//--------------------------------------------------------------
void GuiApp::savePreset() {
    ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + "." + ofToLower("xml"), "Save your file");
    if (saveFileResult.bSuccess){
        gui.saveToFile(saveFileResult.filePath);
    }
}

//--------------------------------------------------------------
void GuiApp::loadPreset(string filePath)
{
    ofFile presetFile = ofFile(filePath);
    if (presetFile.exists()) {

        string fileExtension = ofToUpper(presetFile.getExtension());
        
        //We only want wav + mp3
        if (fileExtension == "XML") {
            gui.loadFromFile(filePath);
        }
    }
}

//--------------------------------------------------------------
void GuiApp::keyPressed(int key)
{
    if (key == 's')
    {
        savePreset();
    }
}

//--------------------------------------------------------------
void GuiApp::dragEvent(ofDragInfo dragInfo)
{
    if( dragInfo.files.size() > 0 ){
        // ofFile droppedFile = ofFile(dragInfo.files[0]);
        loadPreset(dragInfo.files[0]);
    }
}


