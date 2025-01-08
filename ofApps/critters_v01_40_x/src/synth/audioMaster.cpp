
#include "audioMaster.h"
#include "ofApp.h"

// #define SHOW_ON_CRT


// MASTER AUDIO MODULE
//------------------------------------------------------------------
void audioModule::Master::setup(int screens, bool bStereo){

    stereo = bStereo;
    int numChannels = (stereo) ? 2 : 1;


    // set the gain for each bus
    masterBus.set(1.0);
    fxBus.set(1.0);
    cleanBus.set(1.0);
    // guiPtr->masterGain >> gain;
    guiPtr->lowCutFreq >> loCut.in_freq();  // low cut frequency

    gain.channels(numChannels);
    masterBus.channels(numChannels);
    cleanBus.channels(screens);
    fxBus.channels(screens);
    reverbAmt.resize(numChannels);
    
    compressor.resize(numChannels);
    for(int i = 0; i < numChannels; i++) {
        compressor[i].stereoLink(false);
        guiPtr->compThreshold >> compressor[i].in_threshold();
        guiPtr->compKnee      >> compressor[i].in_knee();

        reverbAmt[i].enableSmoothing(5000);
        reverbAmt[i] >> disasterFX.reverb_ch(i);
    }

    subMasterModules.resize(screens);

    // breatherModules.resize(screens);
    // pumperModules.resize(screens);
    // neuronModules.resize(screens);
    // intestineModules.resize(screens);



    
    for (int i = 0; i < screens; i++) {

        if (stereo && screens == 1) {

            subMasterModules[i].init();
            subMasterModules[i].sumBus.ch(0) >> fxBus.ch(i);      // channel1: breathers
            subMasterModules[i].sumBus.ch(1) >> cleanBus.ch(i);     // channel2: pumpers
            subMasterModules[i].sumBus.ch(2) >> fxBus.ch(i);      // channel3: neurons
            subMasterModules[i].sumBus.ch(3) >> cleanBus.ch(i);     // channel4: intestines

            cleanBus.ch(i) >> compressor[i].ch(0) >> masterBus.ch(0);
                              compressor[i].ch(0) >> masterBus.ch(1);
            fxBus.ch(i)    >> compressor[i].ch(1) >> chorus.ch(0) >> delay.ch(0) >> masterBus.ch(0);     
                              compressor[i].ch(1) >> chorus.ch(1) >> delay.ch(1) >> masterBus.ch(1); 
            masterBus.ch(0)   >> disasterFX.ch(0)    >> loCut.ch(0) >> gain.ch(0);
            masterBus.ch(1)   >> disasterFX.ch(1)    >> loCut.ch(1) >> gain.ch(1);

            gain.ch(0) >> engine.audio_out(0);
            gain.ch(1) >> engine.audio_out(1);



            break;

        } else if (i <= 4) {

            subMasterModules[i].init();
            subMasterModules[i].sumBus.ch(0) >> fxBus.ch(i);      // channel1: breathers
            subMasterModules[i].sumBus.ch(1) >> cleanBus.ch(i);     // channel2: pumpers
            subMasterModules[i].sumBus.ch(2) >> fxBus.ch(i);      // channel3: neurons
            subMasterModules[i].sumBus.ch(3) >> cleanBus.ch(i);     // channel4: intestines

            // ROUTING AUDIO AND FX CHAIN
            // one bus per Screen
            cleanBus.ch(i) >> compressor[i].ch(0) >> masterBus.ch(i);
            fxBus.ch(i)    >> compressor[i].ch(1) >> chorus.ch(i) >> delay.ch(i) >> masterBus.ch(i);     
            masterBus.ch(i)   >> disasterFX.ch(i)    >> loCut.ch(i) >> gain.ch(i);

            // MAIN OUTPUT
            // one mono channel per screen
            // gain.ch(i) >> engine.audio_out(i % 2);
            gain.ch(i) >> engine.audio_out(i);

            // create the audio modules for each screen
            // the max number of modules per screen is derived from observation of the maximum number of organisms in the simulation
            // breatherModules[i].resize(26);
            // pumperModules[i].resize(26);
            // neuronModules[i].resize(64);
            // intestineModules[i].resize(16);

            // for (int j = 0; j < breatherModules[i].size(); j++) {
            //     breatherModules[i][j].setup();
            //     breatherModules[i][j].out_signal() >> fxBus.ch(i);
            // }
        } else {
            ofLogNotice("too many screens for the audio system (max 4 screens)!");
        }

        

    }


    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();

    // defines the audio device used for output
    // #ifdef SHOW_ON_CRT
    if (!stereo) {
        engine.setDeviceID(4);
        engine.setChannels (0, screens);  // four channel setup
    // #else
    } else {
        engine.setDeviceID(2);  // THIS HAS TO BE SET THIS AT THE RIGHT INDEX!!!!   the ID for the audio devices can will be shown in the console on program start
        engine.setChannels (0, 2);  // two channel setup
    }
    // #endif
     
    // engine.setup( 44100, 512, 3); 
    engine.setup( 44100, 2048, 8); 

}



//------------------------------------------------------------------
void audioModule::Master::switchDistortion(bool state, int screen) {
    float amount = (state) ? 1.0f : 0.0f;
    if(stereo) {
        amount >> disasterFX.distortion_ch(0);    
        amount >> disasterFX.distortion_ch(1);    
    } else {
        amount >> disasterFX.distortion_ch(screen);
    }
}

//------------------------------------------------------------------
void audioModule::Master::setReverbAmount(float amount, int screen) {
    // amount >> disasterFX.reverb_ch(screen);
    if(stereo) {
        reverbAmt[0].set(amount);
        reverbAmt[1].set(amount);
    } else {
        reverbAmt[screen].set(amount);
    }
}

audioModule::SubMaster & audioModule::Master::getSubMasterModule(int screen) {
    return subMasterModules[screen]; 
}





// SUB MASTER MODULE
// ------------------------------------------------------------------

void audioModule::SubMaster::init() {

    sumBus.set(1.0);
    blackhole.set(1.0);

    breatherModules.resize(26);
    pumperModules.resize(26);
    neuronModules.resize(64);
    intestineModules.resize(16);


    for (int i = 0; i < breatherModules.size(); i++) {
        breatherModules[i].setup();
        // breatherModules[i] >> sumBus.ch(0);
        breatherModules[i] >> sideChainBreather;
    }

    for (int i = 0; i < pumperModules.size(); i++) {
        pumperModules[i].initiate();
        pumperModules[i] >> sumBus.ch(1);
        pumperModules[i].out_trig() >> sideChainIntestine.in_trig();
    }

    for (int i = 0; i < neuronModules.size(); i++) {
        neuronModules[i].init();
        neuronModules[i] >> sumBus.ch(2);
        neuronModules[i].out_trig() >> sideChainBreather.in_trig();
    }

    for (int i = 0; i < intestineModules.size(); i++) {
        intestineModules[i].init();
        // intestineModules[i] >> sumBus.ch(3);
        intestineModules[i] >> sideChainIntestine;
    }

    sideChainIntestine >> sumBus.ch(0);
    sideChainBreather  >> sumBus.ch(3);

    -20.0f >> sideChainIntestine.in_ducking();
    -6.0f >> sideChainBreather.in_ducking();
    100.0f >> sideChainIntestine.in_hold();
    0.0f >> sideChainBreather.in_hold();

    
}



audioModule::Breather & audioModule::SubMaster::getFreeBreatherModule() {

    for (int i = 0; i < breatherModules.size(); i++) {
        if (breatherModules[i].isFree()) {
            return breatherModules[i];
        }
    }
    ofLogNotice("no free breather module available!");
    return breatherModules[0];

}


audioModule::Pumper & audioModule::SubMaster::getFreePumperModule() {

    for (int i = 0; i < pumperModules.size(); i++) {
        if (pumperModules[i].isFree()) {
            return pumperModules[i];
        }
    }
    ofLogNotice("no free pumper module available!");
    return pumperModules[0];

}


audioModule::Neuron & audioModule::SubMaster::getFreeNeuronModule() {

    for (int i = 0; i < neuronModules.size(); i++) {
        if (neuronModules[i].isFree()) {
            return neuronModules[i];
        }
    }
    ofLogNotice("no free neuron module available!");
    return neuronModules[0];

}


audioModule::Intestine & audioModule::SubMaster::getFreeIntestineModule() {

    for (int i = 0; i < intestineModules.size(); i++) {
        if (intestineModules[i].isFree()) {
            return intestineModules[i];
        }
    }
    ofLogNotice("no free intestine module available!");
    return intestineModules[0];

}


