// #pragma once
#include "audioModules.h"
#include "ofApp.h"


// THIS FILE CONTAINS THE SOURCE CODE OF ALL AUDIOMODULES 




// BREATHER AUDIOMODULE
//------------------------------------------------------------------

void audioModule::Breather::setup(){


    // INPUTS
    addModuleInput("pitch", voicePitch);
    addModuleInput("level", level);
    addModuleInput("tune", fineTune);
    addModuleInput("cutoff", filter.in_cutoff() );    
    addModuleInput("bRate", breathRate);
    addModuleInput("tone", mix.in_fade());
    addModuleInput("roughness", noiseAmp.in_mod());
    addModuleInput("bypass", bypass.in_mod());


    // OUTPUTS
    addModuleOutput("output", bypass );
    addModuleOutput("blackhole", intoVoid ); 


    // bIsFree = true;

    // bypass.resize(2);

    // SIGNAL PATH
    
    // mix >> filter >> amp >> pan;
    // mix >> amp >> pan;
    // mix >> filter >> amp >> gain;
    mix >> filter >> amp >> gain >> bypass;
    // mix >> filter >> amp >> gain >> bypass.input(0);

    0.0 >> bypass.in_mod();


    // DEFAULT VALUES
    // pitch >> voicePitch;
    // 0.0 >> voicePitch;
    level >> amp.in_mod();
    guiPtr->brthGain >> dBtoLin >> gain.in_mod();
    guiPtr->brthFineTune >> fineTune;
    guiPtr->brthPw >> pulseWidth;
    guiPtr->brthPwmAmt >> pwmAmount;    
    guiPtr->brthPwmSpeed >> pwmSpeed;
    guiPtr->brthCutoff >> filterCutoff;
    guiPtr->brthReso >> filterReso;
    guiPtr->brthOscMix >> mix.in_fade();  // mix between the two OSCs



    // OSC 1 - Pulse Wave
                         osc1.out_pulse() >> mix.in_A();
                               voicePitch >> osc1.in_pitch();
    1.5f >> driftLFO1.out_random() * 0.13 >> osc1.in_pitch();   
 

    // OSC 2 - Sine Wave
                          osc2.out_sine() >> mix.in_B();
                               voicePitch >> osc2.in_pitch();
    1.5f >> driftLFO2.out_random() * 0.13 >> osc2.in_pitch();
                                 fineTune >> osc2.in_pitch(); // fine detune for slight beating
    

    
    // MODULATIONS AND CONTROL
                     pulseWidth >> osc1.in_pw();
    lfo.out_sine() >> pwmAmount >> osc1.in_pw();
                       pwmSpeed >> lfo.in_freq(); 


    // NOISE
    // noise >> noiseAmp >> filter;
    bitNoise >> noiseAmp >> filter;
    voicePitch >> bitNoise.in_pitch();
    100 >> bitNoise.in_decimation();
    8 >> bitNoise.in_bits();


    // FILTER STUFF
                filterCutoff >> filter.in_cutoff(); 
                  filterReso >> filter.in_reso();
                                //    ui.filterModSpeed >> filterModLfo.in_freq();
    // filterModLfo.out_sine() >> ui.filterModAmt.ch(v) >> filter.in_cutoff();


    // BREATH CONTROL OSCILLATOR
    controlAmp.set(1.0f);
    breathRate >> controlLFO.in_freq();
    0.95 >> meterGate.in_threshold();

    muteAmp.set(0.000000000001f);
    0.000000000001f >> muteEnv.in_velocity();

}

void audioModule::Breather::setFrequency(int pitch) {
    pitch >> voicePitch;
}

void audioModule::Breather::startBreathing() { 

    // start the lfo
    // controlLFO.out_sine() >> controlAmp >> intoVoid;
    controlLFO.out_sine() >> controlAmp >> muteAmp >> amp.in_mod();
    // controlLFO.out_sine() >> meterGate >> intoVoid;
    controlLFO.out_sine() >> meterGate >> muteEnv >> amp.in_mod();

}

float audioModule::Breather::meter_pitch() const{
    return osc1.meter_pitch();
}

float audioModule::Breather::ctrlLfoOut() const {
    // return controlAmp.meter_output();
    return ofClamp(controlAmp.meter_output(), -1.0f, 1.0f);
}

int audioModule::Breather::cycleCount() {
    // count the gate triggers
    if (meterGate.meter_gate() > 0 && !newPhase) newPhase = true;
    if (meterGate.meter_gate() < 1 && newPhase) {
        count++;
        newPhase = false;
    }
    return count;
}







// PUMPER AUDIOMODULE
//------------------------------------------------------------------

void audioModule::Pumper::initiate(){

    // INPUTS
    addModuleInput("trig", triggers); 
    addModuleInput("pitch", osc.in_pitch()); 
    addModuleInput("impulseRate", impulseRate); 
    addModuleInput("trigPhase", impulseLFO.in_retrig()); 
    addModuleInput("phase", impulseLFO.in_phase_start()); 
    addModuleInput("velocity", velocity); 
    
    // OUTPUTS
    addModuleOutput("signal", bypass ); 
    // addModuleOutput("signal", testOut ); 
    addModuleOutput("blackhole", intoVoid ); 
    addModuleOutput("trigger", triggers ); 


    // bIsFree = true;


    // PATCHING

    osc >> amp >> delaySignal >> gain >> bypass;  // main signal chain with delay to sync to the visual animation
    
    0.0 >> bypass.in_mod(); // mute the module by default

    
    triggers >> aEnv >> amp.in_mod();  
 
                                       pEnv >> fbModAmt >> osc.in_fb();
    triggers >> pEnv.set(0.0f, 0.0f,  50.0f) >> pModAmt >> osc.in_pitch();
                               guiPtr->pmprPitchControl >> osc.in_pitch();
                                 floor(ofRandom(-5, 5)) >> osc.in_pitch();


    guiPtr->pmprGainControl >> dBtoLin >> gain.in_mod();
                   velocity >> dBtoLin >> gain.in_mod();
    guiPtr->pmprVeloToEnv              >> pEnv.in_velocity();
    guiPtr->pmprPEnvAmtControl         >> pModAmt.in_mod();
    
    guiPtr->pmprEnvAttackControl       >> aEnv.in_attack();
    guiPtr->pmprEnvHoldControl         >> aEnv.in_hold();
    guiPtr->pmprEnvReleaseControl      >> aEnv.in_release();
    
    guiPtr->pmprPEnvAttackControl      >> pEnv.in_attack();
    guiPtr->pmprPEnvReleaseControl     >> pEnv.in_release();
    
    guiPtr->pmprFbControl              >> osc.in_fb();
    guiPtr->pmprFbModControl           >> fbModAmt.in_mod();



    // CONTROL IMPULSE for the contraction of the shape

    // Set the envelope that gets triggered
    controlEnv.set(10.0, 20.0, 50.0, 1.0);    // Attack, Hold, Release, Velocity

    guiPtr->pmprImpulseEnvAttack  >> controlEnv.in_attack();
    guiPtr->pmprImpulseEnvHold    >> controlEnv.in_hold();
    guiPtr->pmprImpulseEnvRelease >> controlEnv.in_release();

    impulseRate >> impulseLFO;   // patch the impulse rate of the LFO
    meterAmp.set(1.0);


    // DELAY THE OUTPUT

    110.0f >> delaySignal.in_time();    // delay the sound to synchronize with the visual impact of the contraction

    // hack to counteract a bug in the delay which seems to chop of the end of the signal by the delay amount
    // by mixing in a longer, not perceivable signal, this tricks the buffer length
    ghostEnv.set(10.0, 100.0, 500.0, 1.0);
    triggers >> ghostEnv * 0.01 >> delaySignal; 

    muteAmp.set(0.000000000001f);
 
}   


void audioModule::Pumper::startImpulse(){

    // initialize the impulse LFO
    // impulseLFO.out_trig() >> controlEnv >> intoVoid;  // the control envelope has to be patched to a blackhole in order to being processed by the DSP
    // impulseLFO.out_phase() >> meterAmp >> intoVoid;
    impulseLFO.out_trig() >> controlEnv >> muteAmp >> amp.in_mod();
    impulseLFO.out_phase() >> meterAmp >> muteAmp;
    impulseLFO.out_trig() >> triggers;  // trigger the sound

}

void audioModule::Pumper::blockModule(){
    // turn on new instance and mark as blocked
    1.0f >> bypass.in_mod();
    bIsFree = false; 
}


void audioModule::Pumper::freeModule(){
    // reset the module and mark as free
    countOffset += impulseCount();
    0.0 >> bypass.in_mod();
    bIsFree = true;
}


void audioModule::Pumper::enableDB( float minValue ){
    aEnv.enableDBTriggering( minValue, 0.0f);
}
void audioModule::Pumper::disableDB( ){
    aEnv.disableDBTriggering( );
}

float audioModule::Pumper::meter() const {
    // return aEnv.meter_output();
    // return gate.meter_gate();
    return meterAmp.meter_output();
}

float audioModule::Pumper::impulseOut() const {
    return ofClamp(controlEnv.meter_output(), 0.0f, 1.0f);
    // return outNode.getCRValue();
}

float audioModule::Pumper::impulseCount() const {
    return controlEnv.meter_triggers() - countOffset;
}

pdsp::Patchable & audioModule::Pumper::in_trig() {
    return in("trig");
}
pdsp::Patchable & audioModule::Pumper::in_pitch() {
    return in("pitch");
}







// NEURON AUDIOMODULE
//------------------------------------------------------------------

void audioModule::Neuron::init(){


    // INPUTS
    addModuleInput("trig", trigger); 
    addModuleInput("pitch", pitch);
    addModuleInput("sigDuration", signalDuration);


    // OUTPUTS
    addModuleOutput("output", bypass );
    addModuleOutput("trigger", trigger ); 



    // SIGNAL PATH
    
    mix >> filter >> amp >> gain >> bypass;   // bypass
    // mix >> amp >> gain;   // bypass
    // mix >> gain;

    0.0 >> bypass.in_mod(); // mute the module by default



    // CONTOL PATCHING
    0.0 >> pitch;
    guiPtr->nronGain >> dBtoLin >> gain.in_mod();
    // guiPtr->nronPitch >> pitch;
    guiPtr->nronFineTune >> fineTune;
    guiPtr->nronCutoff >> filterCutoff;
    guiPtr->nronReso >> filterReso;
    guiPtr->nronOscMix >> mix.in_fade();  // mix between the two OSCs

    guiPtr->nronEnvAttack       >> ampEnv.in_attack();
    // guiPtr->nronEnvHold         >> ampEnv.in_hold();
    guiPtr->nronEnvRelease      >> ampEnv.in_release();

    guiPtr->nronFEnvAttack       >> fltrEnv.in_attack();
    guiPtr->nronFEnvHold         >> fltrEnv.in_hold();
    // guiPtr->nronFEnvRelease      >> fltrEnv.in_release();



    // OSC 1 - Pulse Wave
                           osc1.out_saw() >> mix.in_A();
                                    pitch >> osc1.in_pitch();
                                    //   -12 >> osc1.in_pitch();
    1.5f >> driftLFO1.out_random() * 0.13 >> osc1.in_pitch();   
 

    // OSC 2 - Sine Wave
                           osc2.out_saw() >> mix.in_B();
                                    pitch >> osc2.in_pitch();
    1.5f >> driftLFO2.out_random() * 0.13 >> osc2.in_pitch();
                                 fineTune >> osc2.in_pitch(); // fine detune for slight beating
    


    // FILTER STUFF
                filterCutoff >> filter.in_cutoff();  /// +++
                // 20 >> filter.in_cutoff(); 
                  filterReso >> filter.in_reso();    /// +++
                                //    ui.filterModSpeed >> filterModLfo.in_freq();
    // filterModLfo.out_sine() >> ui.filterModAmt.ch(v) >> filter.in_cutoff();



    // ENVELOPES AND TRIGGERS

    // Amp envelope
    ampEnv.set(1.0, 100.0, 20.0, 1.0);    // Attack, Hold, Release, Velocity
    signalDuration >> ampEnv.in_hold();

    trigger >> ampEnv >> amp.in_mod();
    // 1.0 >> amp.in_mod();    // debug

    // Filter envelope
    fltrEnv.set(1.0, 1.0, 100.0);    // Attack, Hold, Release, Velocity
    signalDuration >> fltrEnv.in_release();

    trigger >> fltrEnv * guiPtr->nronFilterModAmt >> filter.in_cutoff();



    // pitchEnv.set(0.0f, 0.0f,  250.0f);
    // signalDuration * 2. >> pitchEnv.in_release();

    // trigger >> pitchEnv;

}


void audioModule::Neuron::blockModule(){
    // turn on new instance and mark as blocked
    reset();
    1.0f >> bypass.in_mod();
    bIsFree = false; 
}


void audioModule::Neuron::freeModule(){
    // reset the module and mark as free
    0.0 >> bypass.in_mod();
    bIsFree = true;
}


void audioModule::Neuron::reset(){

    // reset filter envelope, without it a bug occurs where the filter envelope is not triggered - super weird

    fltrEnv.set(1.0, 1.0, 100.0);    // Attack, Hold, Release, Velocity
    signalDuration >> fltrEnv.in_release();

    trigger >> fltrEnv * guiPtr->nronFilterModAmt >> filter.in_cutoff();

}







// INTESTINE AUDIOMODULE
//------------------------------------------------------------------

void audioModule::Intestine::init(){


    // INPUTS
    addModuleInput("trig", trigger); 
    addModuleInput("pitch", pitch);
    addModuleInput("lfoRate", lfoRate);
    addModuleInput("cutoff", filterCutoff);  


    // OUTPUTS
    addModuleOutput("output", gain );




    // CONTOL PATCHING
    // 60 >> pitch;
    guiPtr->itstGain >> dBtoLin >> gain.in_mod();
    // guiPtr->itstPitch >> pitch;
    guiPtr->itstFineTune >> fineTune;
    guiPtr->itstPw >> pulseWidth;
    guiPtr->itstPwmAmt >> pwmAmount.in_mod();    
    // guiPtr->itstLfoRate >> lfoRate;
    guiPtr->itstOscMix >> mix.in_fade();  // mix between the two OSCs

    guiPtr->itstCutoff >> filterCutoff;
    guiPtr->itstReso >> filterReso;
    guiPtr->itstFilterModAmt >> filterModAmt.in_mod();
    

    guiPtr->itstEnvAttack       >> ampEnv.in_attack();
    guiPtr->itstEnvDecay        >> ampEnv.in_decay();
    guiPtr->itstEnvSustain      >> ampEnv.in_sustain();
    guiPtr->itstEnvRelease      >> ampEnv.in_release();




    // SIGNAL PATH
    
    // mix >> amp >> gain;
    // mix >> filter >> amp >> gain;       
    mix * 4.0f >> saturator >> filter >> amp >> gain;       // saturator?

    // 0.0 >> bypass.in_mod(); // mute the module by default





    // OSC 1 - Rectangle Wave
                           osc1.out_pulse() >> mix.in_A();
                                    pitch >> osc1.in_pitch();
                                    //   -12 >> osc1.in_pitch();
    1.5f >> driftLFO1.out_random() * 0.13 >> osc1.in_pitch();   
 

    // OSC 2 - Rectangle Wave
                           osc2.out_pulse() >> mix.in_B();
                                    pitch >> osc2.in_pitch();
    1.5f >> driftLFO2.out_random() * 0.13 >> osc2.in_pitch();
                                 fineTune >> osc2.in_pitch(); // fine detune for slight beating
    



    // PULSE WIDTH MODULATION
                    //  pulseWidth >> osc1.in_pw();
                            0.5 >> osc1.in_pw();
    lfo.out_sine() >> pwmAmount >> osc1.in_pw();

                            0.5 >> osc2.in_pw();
    lfo.out_sine() >> pwmAmount >> osc2.in_pw();
                       
                       lfoRate >> lfo.in_freq(); 
                           0.125 >> lfo.in_phase_start();




    // FILTER STUFF
                      filterCutoff >> filter.in_cutoff(); 
                // 20 >> filter.in_cutoff(); 
                        filterReso >> filter.in_reso();
    lfo.out_sine() >> filterModAmt >> filter.in_cutoff();
                                //    ui.filterModSpeed >> filterModLfo.in_freq();
    




    // ENVELOPES AND TRIGGERS

    // Amp envelope
    ampEnv.set(100.0, 100.0, 0.8, 500.0);    // Attack, Decay, Sustain, Release, (Velocity)
    // signalDuration >> ampEnv.in_hold();

    trigger >> ampEnv >> amp.in_mod();
    trigger >> lfo.in_retrig();

    killSwitch >> trigger;      // to switch the filter (and the sound) off from within the audioModule


}


void audioModule::Intestine::blockModule(){
    // turn on new instance and mark as blocked
    // 1.0f >> bypass.in_mod();
    bIsFree = false; 
}


void audioModule::Intestine::freeModule(){
    // reset the module and mark as free
    ofLogNotice("freeing intestine module");
    killSwitch.off();       // trigger off signal -> does not take effect if the output is set to 0.0 afterwards (through bypass)
    // 0.0f >> bypass.in_mod();     // bypas
    bIsFree = true;
}