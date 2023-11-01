/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"
#include "ofApp.h"


void GuiApp::setup(){

    ofBackground(0);
	ofSetVerticalSync(false);


    setSimulationGUI();
    setSynthGUI();
	
    maskChanged = false;
    debugMode = false;




    // LOAD DEFAULT SIMULATION PRESETS
    // string defaultSimulationPreset = "../../../../Presets/Simulation/lab30_01.xml";   
    // string defaultSimulationPreset = "../../../../Presets/Simulation/NeuronIndivSprings03.xml";   
    string defaultSimulationPreset = "../../../../Presets/Simulation/friesTV-physics_01.xml";   
    loadPreset(defaultSimulationPreset, 1);



    // LOAD DEFAULT TUBE CALIBRATION PRESETS
    string defaultTubeCalibration = "../../../../Presets/TubeCalibrations/friesTV_x1920.xml";
    // string defaultTubeCalibration = "../../../../Presets/TubeCalibrations/MacMini_4screens02_Philips90%.xml";
    loadPreset(defaultTubeCalibration, 1);


    // LOAD DEFAULT SYNTH PRESETS FOR EACH ORGANISM TYPE
    string defaultSynthPreset = "../../../../Presets/Synth/BreathingNew02.xml";
    loadPreset(defaultSynthPreset, 2);
        defaultSynthPreset = "../../../../Presets/Synth/PressurePumpers01.xml";
    loadPreset(defaultSynthPreset, 2);
        defaultSynthPreset = "../../../../Presets/Synth/NeuronsUnderWater01.xml";
    loadPreset(defaultSynthPreset, 2);
        defaultSynthPreset = "../../../../Presets/Synth/Intestines07.xml";
    loadPreset(defaultSynthPreset, 2);

    string defaultFxPreset = "../../../../Presets/Synth/PostFX05.xml";    
    loadPreset(defaultFxPreset, 2);


}




//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
void GuiApp::setSimulationGUI() {


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

    guiTune.add(tunePressureTest.set("Pressure Test", 1.0, -5.0, 4.0));
    

    gui.add(guiTune);
    gui.getGroup("Tune Forces").minimize();



    guiBreather.setName("Tune Breather Shape");
    guiBreather.add(tuneBreatherExpansionForce.set("Tune Expansion Force", 0.0, -10., 10.));
    guiBreather.add(tuneBreatherExpansionRadius.set("Tune Expansion Radius Factor", 25.0, 1., 80.));
    guiBreather.add(tuneJointLength.set("Joint Length", 22.0, 1., 30.));
    guiBreather.add(tuneJointElasticity.set("Joint Elasticity", -5., -10, 10));
    guiBreather.add(tuneJointLimitForce.set("Joint Force limit", 0, -10, 10));
    gui.add(guiBreather);
    gui.getGroup("Tune Breather Shape").minimize();


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

    guiNeuron.setName("Tune Neuron Shape");
    guiNeuron.add(tuneDendriteLength.set("Dendrite Length", 22.0, 1., 30.));
    guiNeuron.add(tuneDendriteElasticity.set("Dendrite Elasticity", -5., -10, 10));
    guiNeuron.add(tuneDendriteLimitForce.set("Dendrite Force limit", 0, -10, 10));
    gui.add(guiNeuron);
    // gui.getGroup("Tune Neuron Shape").minimize();



    for (int i = 0; i < mainPtr->numScreens; i++) {
        mainPtr->molSystem[i].gui.setName("Shape Tube " + ofToString(i));
        guiCanvas.add(mainPtr->molSystem[i].gui);

        mainPtr->molSystem[i].tuneCanvasWidth.addListener(this, &GuiApp::maskListener);
        mainPtr->molSystem[i].tuneCanvasHeight.addListener(this, &GuiApp::maskListener);
        mainPtr->molSystem[i].tuneVerticalBow.addListener(this, &GuiApp::maskListener);
        mainPtr->molSystem[i].tuneHorizontalBow.addListener(this, &GuiApp::maskListener);
        mainPtr->molSystem[i].tuneEdges.addListener(this, &GuiApp::maskListener);
        mainPtr->molSystem[i].tuneXpos.addListener(this, &GuiApp::maskListener);
        mainPtr->molSystem[i].tuneYpos.addListener(this, &GuiApp::maskListener);

    }

    guiCanvas.setName("Tune Canvas");
    gui.add(guiCanvas);
    gui.getGroup("Tune Canvas").minimize();


    guiGeneral.setName("General Settings");
    guiGeneral.add(switchKinectCalibration.set("Kinect Calibration Mode", false));
    guiGeneral.add(switchScreenMask.set("Screen Mask Calibration Mode", false));
    gui.add(guiGeneral);
    gui.getGroup("General Settings").minimize();


    guiTimings.setName("Timings");
    
    guiTimingsCells.setName("Breathers");
    guiTimingsCells.add(cellNextGrowth.set("Cell Next Growth", glm::vec2(100, 0.5), glm::vec2(100, 0.), glm::vec2(3000, 1.)));
    guiTimingsCells.add(cellFreqMultiplier.set("Cell Freq Multiplier", 1.0, 1.0, 2.0));
    guiTimings.add(guiTimingsCells);

    guiTimingsPumpers.setName("Pumpers");
    guiTimingsPumpers.add(pumperSyncDistance.set("Pumper Sync Distance", 100, 10, 1000));
    guiTimings.add(guiTimingsPumpers);
    
    guiTimingsNeurons.setName("Neurons");
    // guiTimingsNeurons.setFillColor(ofColor::yellowGreen);
    guiTimingsNeurons.add(neuronGrowSpeed.set("Neuron Grow Speed", 500, 100, 3000));
    guiTimingsNeurons.add(neuronSignalInterval.set("Neuron Signal Inteval", 2000, 100, 3000));
    guiTimingsNeurons.add(neuronSyncDistance.set("Neuron Sync Distance", 100, 10, 1000));
    guiTimings.add(guiTimingsNeurons);
    
    guiTimingsIntestines.setName("Intestines");
    guiTimingsIntestines.add(intestineGrowInterval.set("Intestine Grow Interval", 700, 100, 3000.));
    guiTimingsIntestines.add(intestineDigestionInterval.set("Intestine Digestion Interval", 4000, 1000, 7000));
    guiTimingsIntestines.add(intestineDigestionSpeed.set("Intestine Digestion Speed", 300, 100, 1000)); 
    guiTimingsIntestines.add(intestineSyncDistance.set("Intestine Sync Distance", 150, 10, 1000));
    guiTimings.add(guiTimingsIntestines);

    gui.add(guiTimings);
    gui.getGroup("Timings").minimize();

    gui.getGroup("Timings").getGroup("Cells").setHeaderBackgroundColor(ofColor::fromHex(0x2bdbe6));
    gui.getGroup("Timings").getGroup("Cells").setBorderColor(ofColor::fromHex(0x2bdbe6));
    gui.getGroup("Timings").getGroup("Neurons").setHeaderBackgroundColor(ofColor::fromHex(0xfcfdbd));
    gui.getGroup("Timings").getGroup("Neurons").setBorderColor(ofColor::fromHex(0xfcfdbd));
    gui.getGroup("Timings").getGroup("Intestines").setHeaderBackgroundColor(ofColor::fromHex(0xe690a4));
    gui.getGroup("Timings").getGroup("Intestines").setBorderColor(ofColor::fromHex(0xe690a4));

}




//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
void GuiApp::setSynthGUI() {

    guiSynth.setup("SYNTH");
    guiSynth.setPosition(300, 10);
        guiSynth.add( mainPtr->audioMaster.gain.set("master gain", -12, -48, 12) ); 
        // masterGain.enableSmoothing(50.f);
        guiSynth.add( lowCutFreq.set("low cut frequency", 150, 20, 500) ); 
        guiSynth.add( compThreshold.set("compressor threshold", -20, -36, 0) ); 
        guiSynth.add( compKnee.set("compressor knee", 0.0, 0.0, 3.0) ); 
        


    // guiSumFx.setName("SUM FX");

    // guiSynth.add( guiSynthFX );
    // guiSynth.getGroup("FX CHAIN").minimize();


    guiSynthFX.setName("FX CHAIN");
        guiSynthFX.add(mainPtr->audioMaster.chorus.parameters);
        guiSynthFX.add(mainPtr->audioMaster.delay.parameters);
    guiSynth.add( guiSynthFX );
    guiSynth.getGroup("FX CHAIN").minimize();
    


    guiSynthBrthrs.setName("BREATHERS");
        guiSynthBrthrs.add(brthGain.set("gain", -12, -48, 24) );
        guiSynthBrthrs.add(brthFineTune.set("fine tune Sine-Osc", 0.078125f, -0.5f, 0.5f) );
        guiSynthBrthrs.add(brthOscMix.set("Oscillator Mix", 0.5f, 0.0f, 1.0f) );
        guiSynthBrthrs.add(brthPw.set("Pulse Width", 0.5f, 0.0f, 1.0f) );
        guiSynthBrthrs.add(brthPwmAmt.set("PWM amount", 0.0729167f, 0.0f, 0.5f) );
        guiSynthBrthrs.add(brthPwmSpeed.set("PWM speed (hz)", 0.3f, 0.005f, 1.5f));
        guiSynthBrthrs.add(brthCutoff.set("Filter cutoff", 69, 20, 136));
        guiSynthBrthrs.add(brthReso.set("Filter resonance", 0.352041f, 0.0f, 1.0f) );
        brthCutoff.enableSmoothing(100.0f);   

    guiSynth.add( guiSynthBrthrs );
    guiSynth.getGroup("BREATHERS").setHeaderBackgroundColor(ofColor::fromHex(0x2bdbe6));
    guiSynth.getGroup("BREATHERS").setBorderColor(ofColor::fromHex(0x2bdbe6));
    guiSynth.getGroup("BREATHERS").minimize();



    guiSynthPumpers.setName("PUMPERS");
        guiSynthPumpers.add( pmprGainControl.set("gain", -12, -48, 24) );
        guiSynthPumpers.add( pmprPitchControl.set("pitch", 36, 24, 132 ));
        guiSynthPumpers.add( pmprPEnvAmtControl.set("pitch env", 48, 0, 72 ) );
        guiSynthPumpers.add( pmprPEnvAttackControl.set("p env attack",  0, 0, 200 ) );
        guiSynthPumpers.add( pmprPEnvReleaseControl.set("p env release", 50, 5, 500 ) );
        guiSynthPumpers.add( pmprVeloToEnv.set( "p env dyn amt", 0.5f, 0.0f, 1.0f ) );
        guiSynthPumpers.add( pmprFbControl.set( "feedback", 0.0f, 0.0f, 0.25f ) );
        guiSynthPumpers.add( pmprFbModControl.set( "env to fb", 0.0f, 0.0f, 0.25f ) );
        guiSynthPumpers.add( pmprEnvAttackControl.set("attack",  3, 0, 200 ) );
        guiSynthPumpers.add( pmprEnvHoldControl.set( "hold",  40, 0, 300 ) );
        guiSynthPumpers.add( pmprEnvReleaseControl.set("release", 20, 5, 600 ) );

        guiSynthPumpersImpulse.setName("Control Params");
            guiSynthPumpersImpulse.add( pmprImpulseEnvAttack.set("Impulse attack",  3, 0, 200 ) );
            guiSynthPumpersImpulse.add( pmprImpulseEnvHold.set( "Impulsehold",  40, 0, 300 ) );
            guiSynthPumpersImpulse.add( pmprImpulseEnvRelease.set("Impulse release", 20, 5, 600 ) );
            guiSynthPumpersImpulse.add( pmprImpulseAmt.set("Impulse Amount", 0.15f, 0.0f, 0.3f) );
        guiSynthPumpers.add( guiSynthPumpersImpulse );

    guiSynth.add( guiSynthPumpers );
    guiSynth.getGroup("PUMPERS").setHeaderBackgroundColor(ofColor::fromHex(0xf22571));
    guiSynth.getGroup("PUMPERS").setBorderColor(ofColor::fromHex(0xf22571));
    guiSynth.getGroup("PUMPERS").minimize();



    guiSynthNeurons.setName("NEURONS");
        guiSynthNeurons.add(nronGain.set("gain", -24, -48, 24) );
        guiSynthNeurons.add( nronEnvAttack.set("Amp attack",  0, 0, 30 ) );
        guiSynthNeurons.add( nronEnvRelease.set("Amp release", 10, 5, 600 ) );
        guiSynthNeurons.add(nronPitch.set("pitch", 60, 24, 132 ));
        guiSynthNeurons.add(nronFineTune.set("fine tune 2nd osc", 0.078125f, -0.5f, 0.5f) );
        guiSynthNeurons.add(nronOscMix.set("Oscillator Mix", 0.5f, 0.0f, 1.0f) );
        guiSynthNeurons.add(nronCutoff.set("Filter cutoff", 69, 20, 136));
        brthCutoff.enableSmoothing(100.0f);   
        guiSynthNeurons.add(nronReso.set("Filter resonance", 0.352041f, 0.0f, 1.0f) );
        guiSynthNeurons.add(nronFilterModAmt.set("Filter Mod Amount", 75.f, 20.f, 150.f) );
        guiSynthNeurons.add(nronFEnvAttack.set("filter attack",  3, 0, 200 ) );
        guiSynthNeurons.add(nronFEnvHold.set( "filter hold",  40, 0, 300 ) );
        guiSynthNeurons.add(nronSignalSpeed.set("Signal Speed", 50, 20, 100) );

    guiSynth.add( guiSynthNeurons );
    guiSynth.getGroup("NEURONS").setHeaderBackgroundColor(ofColor::fromHex(0xfcfdbd));
    guiSynth.getGroup("NEURONS").setBorderColor(ofColor::fromHex(0xfcfdbd));
    guiSynth.getGroup("NEURONS").minimize();


    guiSynthIntestines.setName("INTESTINES");
        guiSynthIntestines.add(itstGain.set("gain", 6, -48, 24) );
        guiSynthIntestines.add(itstPitch.set("pitch", 24, 12, 84 ));
        guiSynthIntestines.add(itstFineTune.set("fine tune 2nd osc", 0.085f, -0.5f, 0.5f) );
        guiSynthIntestines.add(itstOscMix.set("Oscillator Mix", 0.5f, 0.0f, 1.0f) );
        guiSynthIntestines.add(itstLfoRate.set("LFO rate (hz)", 1.2f, 0.005f, 20.0f));    
        guiSynthIntestines.add(itstPw.set("Pulse Width", 0.5f, 0.0f, 1.0f) );
        guiSynthIntestines.add(itstPwmAmt.set("PWM amount (LFO)", 0.4f, 0.0f, 0.5f) );
        guiSynthIntestines.add(itstCutoff.set("Filter cutoff", 69, 20, 136));
        itstCutoff.enableSmoothing(100.0f);   
        guiSynthIntestines.add(itstReso.set("Filter resonance", 0.352041f, 0.0f, 1.0f) );
        guiSynthIntestines.add(itstFilterModAmt.set("Filter Mod amt (LFO)", 48, 0, 84) );
        guiSynthIntestines.add(itstEnvAttack.set("Amp attack",  200, 0, 500 ) );
        guiSynthIntestines.add(itstEnvDecay.set("Amp decay",  200, 0, 500 ) );
        guiSynthIntestines.add(itstEnvSustain.set("Amp sustain",  0.5, 0., 1. ) );
        guiSynthIntestines.add(itstEnvRelease.set("Amp release", 700, 0, 2000 ) );
 
    guiSynth.add( guiSynthIntestines );
    guiSynth.getGroup("INTESTINES").setHeaderBackgroundColor(ofColor::fromHex(0xe690a4));
    guiSynth.getGroup("INTESTINES").setBorderColor(ofColor::fromHex(0xe690a4));
    guiSynth.getGroup("INTESTINES").minimize();





    // guiSynth.add(mainPtr->mixFader.set("Mix Reverb Dry/Wet", 0.0f, 0.0f, 1.0f));
    // guiSynth.add(mainPtr->reverb.parameters);
    // guiSynth.add(mainPtr->bitCrush.parameters);

}





//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
void GuiApp::update(){

}

void GuiApp::draw(){

    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    int numMolecules = 0;
    for (int i = 0; i < mainPtr->numScreens; i++) {
        numMolecules += mainPtr->molSystem[i].allMolecules.size();
    }

    string infoTxt = "fps: " + ofToString(ofGetFrameRate()) + "\nnum Molecules: " + ofToString(numMolecules);
    ofDrawBitmapString(infoTxt, 20, ofGetHeight()-50);

	gui.draw();
    guiSynth.draw();


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
            // mainPtr->synth.gui.saveToFile(saveFileResult.filePath);
            guiSynth.saveToFile(saveFileResult.filePath);
        }
    }
}

//--------------------------------------------------------------
void GuiApp::loadPreset(string filePath, int panel)
{
 
    ofFile presetFile = ofFile(filePath);
    if (presetFile.exists()) {

        string fileExtension = ofToUpper(presetFile.getExtension());
        
        //We only want wav + mp3
        if (fileExtension == "XML" && panel == 1) {
            gui.loadFromFile(filePath);
        } else if (fileExtension == "XML" && panel == 2) {
            // mainPtr->synth.gui.loadFromFile(filePath);
            guiSynth.loadFromFile(filePath);
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
    // if (key == '5')   mainPtr->molSystem[1].addOrganisms(INTESTINE, 1);
    // if (key == '6')   mainPtr->molSystem[0].addOrganisms(LIQUID,   50);
    // if (key == '7')   {
    //     for (int i = 0; i < mainPtr->numScreens; i++) {
    //         mainPtr->molSystem[i].addOrganisms(INTESTINE, 1);
    //     }
    // }
    // if (key == '8')   mainPtr->molSystem[0].addOrganisms(BREATHER,  1);
    // if (key == '9')   mainPtr->molSystem[0].addOrganisms(PUMPER,    1);
    // if (key == '0')   mainPtr->molSystem[0].addOrganisms(NEURON,    1);

    if( key == 'd' ){ 
        debugMode = !debugMode;
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

