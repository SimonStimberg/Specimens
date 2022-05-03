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
    

    gui.add(guiTune);


	ofBackground(0);
	ofSetVerticalSync(false);
}

void GuiApp::update(){

}

void GuiApp::draw(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
	gui.draw();
}
