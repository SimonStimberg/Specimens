
#include "synth.h"

void PolySynth::setup(){

    // initialize vectors and allocate space by resizeing them

    // voices.clear();
    // voiceLevels.clear();
    // voiceFilterCutoffs.clear();


    // int numVoices = pitches.size();
    
    // voices.resize( numVoices );
    // voiceLevels.resize( numVoices );
    // voiceFilterCutoffs.resize( numVoices );
    

    
    patch();
    // setUI();
    
}


void PolySynth::patch() {

    //-------------- PATCHING ----------------------
    // for(int i=0; i<voices.size(); ++i){

    //      // connect each voice to the master gain (master bus)
    //      voices[i].out("L") >> chorus.ch(0);
    //      voices[i].out("R") >> chorus.ch(1);
    //  }

    500 >> loCut.in_freq();
    masterBus.set(1.0);

    //  chorus.ch(0) >> delay.ch(0) >> comp.ch(0) >> loCut.ch(0) >> gain.ch(0) >> engine.audio_out(0);
    //  chorus.ch(1) >> delay.ch(1) >> comp.ch(1) >> loCut.ch(1) >> gain.ch(1) >> engine.audio_out(1);

    masterBus.ch(0) >> gain.ch(0) >> engine.audio_out(0);
    masterBus.ch(1) >> gain.ch(1) >> engine.audio_out(1);

    // masterBus.ch(0) >> chorus.ch(0) >> delay.ch(0) >> gain.ch(0) >> engine.audio_out(0);
    // masterBus.ch(1) >> chorus.ch(1) >> delay.ch(1) >> gain.ch(1) >> engine.audio_out(1);

    // gain.ch(0) >> absolute >> envFollowerPeak >> engine.blackhole();

        

     //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(5); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 

}



void PolySynth::setUI() {

    gui.setup("SYNTH");
    gui.add( gain.set("master gain", -12, -64, 12) ); 
    gain.enableSmoothing(50.f);
    
    uiOsc.setName("oscillator control");
    uiOsc.add( oscMix.set("oscillator mix", 0.5f, 0.0f, 1.0f) );
    uiOsc.add( drift_amt.set("oscillator drift", 0.13f, 0.0f, 0.5f) ); 
    uiOsc.add( vibratoAmt.set("vibrato amount", 0.15f, 0.0f, 1.0f) );
    uiOsc.add( vibratoSpeed.set("vibrato speed (hz)", 7.0f, 4.0f, 10.0f));

        osc1.setName("osc 1 (pulse)");
        osc1.add(pw.set("pulse width", 0.5f, 0.0f, 1.0f) );
        osc1.add(pwmAmt.set("pwm amount", 0.2f, 0.0f, 0.5f) );
        osc1.add(pwmSpeed.set("pwm speed (hz)", 0.3f, 0.005f, 1.5f));

        osc2.setName("osc 2 (sine)");
        osc2.add(octaveShift.set("octave", 0, -1, 2) );
        osc2.add(fineTune.set("fine tune", 0.0f, -0.5f, 0.5f) );
        
        uiOsc.add( osc1 );
        uiOsc.add( osc2 );

    uiFilter.setName("filter");
    uiFilter.add(cutoff.set("cutoff pitch", 80, 20, 136));
    uiFilter.add(reso.set("resonance", 0.15f, 0.0f, 1.0f) );
    uiFilter.add( filterModAmt.set("LFO mod amount", 10, 0, 80) );
    uiFilter.add( filterModSpeed.set("LFO speed (hz)", 0.1f, 0.0f, 1.0f));
    cutoff.enableSmoothing(100.0f);   

    gui.add( uiOsc );
    gui.add( uiFilter );
    gui.add( chorus.parameters );
    gui.add( delay.parameters );
    gui.setPosition(20, 20);


    guiVoices.setup("voice levels");
    for( size_t i=0; i<voices.size(); ++i ){
        guiVoices.add(voiceLevels[i].set("voice "+ofToString(i), 1.0f, 0.0f, 1.0f) );
        voiceLevels[i].enableSmoothing(50.0f);
    }
    guiVoices.setPosition(260, 20);

}



void PolySynth::addVoice(float pitch) {

   

    
    voices.push_back(Voice());
    
    voiceLevels.push_back(pdsp::Parameter());
    voiceFilterCutoffs.push_back(pdsp::Parameter());
    
    voiceAmps.push_back(pdsp::ValueControl());
    voiceCutoffFreqs.push_back(pdsp::ValueControl());
    voicePitchDivergence.push_back(pdsp::ValueControl());
    voiceTimeMods.push_back(pdsp::ValueControl());
    
    voicePitches.push_back(pitch);


    ofLogNotice("all pitches to set: " + ofToString(voicePitches));


    int thisNum = voices.size()-1;

    voiceLevels[thisNum].set(0.0);
    voiceFilterCutoffs[thisNum].set(0.0);
    voiceAmps[thisNum].set(1.0);
    voiceCutoffFreqs[thisNum].set(0.0);
    voicePitchDivergence[thisNum].set(0.0);
    voiceTimeMods[thisNum].set(1000.0);



    // all routings have to be repatched after altering the vector size (adding elements) because they get lost
    for( size_t i=voices.size()-1; i<voices.size(); ++i ){

        voices[i].setup( this, i, voicePitches[i]);
        // voices[i].setup( this, 0, 60);

        // voiceAmps[i] >> voices[i].in_gain();
        // voiceAmps[i].enableSmoothing(50.0f); // 50ms smoothing

        // voiceCutoffFreqs[i] >> voices[i].in_cutoffFreq();
        // voiceCutoffFreqs[i].enableSmoothing(50.0f); // 50ms smoothing

        // voicePitchDivergence[i] >> voices[i].in_pitch();
        // voicePitchDivergence[i].enableSmoothing(50.0f); // 50ms smoothing

        // voiceTimeMods[i] >> voices[i].in_timeMod();
        // voiceTimeMods[i].enableSmoothing(50.0f); // 50ms smoothing
        // voiceTimeMods[i].disableSmoothing();

        voices[i].out("L") >> masterBus.ch(0);
        voices[i].out("R") >> masterBus.ch(1);

    }

}



void PolySynth::setVoiceLevels(vector <float> & levels) {
    if (voices.size() == levels.size()) {
        
        for( size_t i=0; i<voices.size(); ++i ){

            float newLevel = ofMap(ofClamp(levels[i], 0., 1.), 0., 1., -48.0, 0.0);
            // float newLevel = ofMap(ofClamp(levels[i], 0., 1.), 0., 1., 0.0, 1.0);
            // float newLevel = ofMap(levels[i], 0., 1., -48.0, 0.0);
            // voiceLevels[i].set(newLevel*10.0);
            // newLevel >> voices[i].in("gain");
            // voiceAmps[i].set(dB(newLevel));
            // voiceAmps[i].set(newLevel);

            // float newCutoff = ofMap(ofClamp(levels[i], 0., 1.), 0., 1., -20., 30.);  // values are being apparently not overwritten but added to the overall value
            float newCutoff = ofMap(levels[i], 0., 1., -20., 30.);
            // voiceFilterCutoffs[i].set(newCutoff);
            voiceCutoffFreqs[i].set(newCutoff);

            float newPitchDiv = ofMap(levels[i], 0., 1., 0., 1.);
            // voiceFilterCutoffs[i].set(newCutoff);
            // voicePitchDivergence[i].set(newPitchDiv);

            // float newTimeMod = ofMap(levels[i], 0., 1., 340.0, 75.0, true);
            float newTimeMod = ofMap(levels[i], 0., 1., 2.0, 20.0, true);
            // voiceTimeMods[i].set(newTimeMod);
            newTimeMod >> voices[i].in_timeMod();


            
        }   
        
    }

}


void PolySynth::setVoicePans(vector <float> & panPos) {
    
    if (voices.size() == panPos.size()) {   

        for( size_t i=0; i<voices.size(); ++i ){

            panPos[i] >> voices[i].in_pan();
        }   
        
    }

}


void PolySynth::setVoiceCutoffs(vector <float> & cutFreqs) {
    
    if (voices.size() == cutFreqs.size()) {   

        for( size_t i=0; i<voices.size(); ++i ){

            float newCutoff = ofMap(cutFreqs[i], 0.0, 1.0, -40.0, 40.0, false);

            // voiceFilterCutoffs[i].set(cutFreqs[i]);
            voiceFilterCutoffs[i].set(newCutoff);

            // float newTone = ofMap(cutFreqs[i], 0.0, 1.0, 1.0, 0.0, true);

            // newTone >> voices[i].in("tone");
            

            // cutFreqs[i] >> voices[i].in_pan();
        }   
        
    }

}




