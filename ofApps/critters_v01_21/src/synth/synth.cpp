
#include "synth.h"

void PolySynth::setup(){

    // voices.clear();
    // voiceLevels.clear();
    // voiceFilterCutoffs.clear();


    // int numVoices = pitches.size();
    
    // voices.resize( numVoices );
    // voiceLevels.resize( numVoices );
    // voiceFilterCutoffs.resize( numVoices );
    
    // for( size_t i=0; i<voices.size(); ++i ){
    //     voices[i].setup( *this, i, pitches[i]);
    // }   
    
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

     chorus.ch(0) >> delay.ch(0) >> comp.ch(0) >> loCut.ch(0) >> gain.ch(0) >> engine.audio_out(0);
     chorus.ch(1) >> delay.ch(1) >> comp.ch(1) >> loCut.ch(1) >> gain.ch(1) >> engine.audio_out(1);

        

     //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(5); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 

}

void PolySynth::addVoice(float pitch) {

    
    voices.push_back(Voice());
    voiceLevels.push_back(pdsp::Parameter());
    voiceFilterCutoffs.push_back(pdsp::Parameter());
    // voices.resize(voices.size()+1);
    // voiceLevels.resize(voiceLevels.size()+1);
    // voiceFilterCutoffs.resize(voiceFilterCutoffs.size()+1);

    int thisNum = voices.size()-1;

    voiceLevels[thisNum].set(0.0);
    voiceFilterCutoffs[thisNum].set(0.0);

    int pitches[7] = {60, 63, 67, 70, 74, 80, 84};

    for( size_t i=0; i<voices.size(); ++i ){

        voices[i].setup( *this, i, pitches[i]);

        voices[i].out("L") >> chorus.ch(0);
        voices[i].out("R") >> chorus.ch(1);


    }


    0.0 >> voices[thisNum].in_pan();

    0.75f >> voices[thisNum].in("gain");
    

    // Voice * v = new Voice();
    // voices.push_back(v);
    // voiceLevels.push_back(pdsp::Parameter());
    // voiceFilterCutoffs.push_back(pdsp::Parameter());

    // v->setup( *this, voices.size()-1, pitch);

    // v->out("L") >> chorus.ch(0);
    // v->out("R") >> chorus.ch(1);

    // 0.0 >> v->in_pan();
    // voiceLevels[voices.size()-1].set(0.5);
    // voiceFilterCutoffs[voices.size()-1].set(0.0);


    // voices.push_back(new Voice());
    

}


void PolySynth::setUI() {

    gui.setup("SYNTH");
    gui.add( gain.set("master gain", 0, -64, 12) ); 
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


void PolySynth::setVoiceLevels(vector <float> & levels) {
    if (voices.size() == levels.size()) {
        
        for( size_t i=0; i<voices.size(); ++i ){

            float newLevel = ofMap(ofClamp(levels[i], 0., 1.), 0., 1., 0., 1.0);
            voiceLevels[i].set(newLevel*10.0);
            newLevel >> voices[i].in("gain");

            float newCutoff = ofMap(ofClamp(levels[i], 0., 1.), 0., 1., -20., 0.);  // values are being apparently not overwritten but added to the overall value
            // voiceFilterCutoffs[i].set(newCutoff);
            
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





void PolySynth::Voice::setup( PolySynth & ui, int v, int pitch){

    addModuleInput("pan", pan.in_pan());
    addModuleInput("pitch", voicePitch);
    addModuleInput("gain", voiceAmp.in_mod());
    addModuleInput("cutoff", filter.in_cutoff() );
    addModuleInput("tone", mix.in_fade());
    // addModuleOutput("signal", voiceAmp);
    addModuleOutput("L", pan.out_L() );
    addModuleOutput("R", pan.out_R() );


    // SIGNAL PATH
    
    mix >> filter >> voiceAmp >> pan;

    // OSC 1 - Pulse Wave
                                        osc1.out_pulse() >> mix.in_A();
                                                   pitch >> osc1.in_pitch();
    1.5f >> driftLFO1.out_random() >> ui.drift_amt.ch(v) >> osc1.in_pitch();   
        vibratoLfo.out_triangle() >> ui.vibratoAmt.ch(v) >> osc1.in_pitch();  

    // OSC 2 - Sine Wave
                                         osc2.out_sine() >> mix.in_B();
                                                   pitch >> osc2.in_pitch();
    1.5f >> driftLFO2.out_random() >> ui.drift_amt.ch(v) >> osc2.in_pitch();
                                     ui.octaveShift * 12 >> osc2.in_pitch();  
                                             ui.fineTune >> osc2.in_pitch();
        vibratoLfo.out_triangle() >> ui.vibratoAmt.ch(v) >> osc2.in_pitch();       



    

    
    // MODULATIONS AND CONTROL
    lfo.out_sine() >> ui.pwmAmt.ch(v) >> osc1.in_pw();
                                ui.pw >> osc1.in_pw();
                          ui.pwmSpeed >> lfo.in_freq(); 

                      ui.vibratoSpeed >> vibratoLfo.in_freq();


    // FILTER STUFF
                                           ui.cutoff >> filter.in_cutoff(); 
                                             ui.reso >> filter.in_reso();
                                   ui.filterModSpeed >> filterModLfo.in_freq();
    // filterModLfo.out_sine() >> ui.filterModAmt.ch(v) >> filter.in_cutoff();


    //  voiceTrigger >> modEnv.set(400.0f, 600.0f, 1.0f, 800.0f) >> ui.modAmt.ch(v) >> filter.in_cutoff(); 

    
    // (voiceTrigger >> (ampEnv.set(200.0f, 400.0f, 1.0f, 600.0f ) * 0.1f)) >> voiceAmp.in_mod();
                    // ui.voiceLevels[v] >> voiceAmp.in_mod();
             ui.voiceFilterCutoffs[v] >> filter.in_cutoff(); 


    
    

        ui.oscMix >> mix.in_fade();  
        // -1.0f >> pan.in_pan();
}





// float PolySynth::Voice::meter_mod_env() const{
//     return modEnv.meter_output();
// }

// float PolySynth::Voice::meter_pitch() const{
//     return oscillator.meter_pitch();
// }
