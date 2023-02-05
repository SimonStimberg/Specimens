// #pragma once
#include "audioModules.h"
#include "ofApp.h"



void audioModule::Breather::setup(int pitch){


    // INPUTS
    addModuleInput("pitch", voicePitch);
    addModuleInput("level", level);
    addModuleInput("tune", fineTune);
    addModuleInput("cutoff", filter.in_cutoff() );    
    addModuleInput("bRate", breathRate);
    addModuleInput("tone", mix.in_fade());


    // OUTPUTS
    addModuleOutput("output", gain );
    addModuleOutput("blackhole", intoVoid ); 



    // SIGNAL PATH
    
    // mix >> filter >> amp >> pan;
    // mix >> amp >> pan;
    mix >> filter >> amp >> gain;



    // DEFAULT VALUES
    pitch >> voicePitch;
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



    // FILTER STUFF
                filterCutoff >> filter.in_cutoff(); 
                  filterReso >> filter.in_reso();
                                //    ui.filterModSpeed >> filterModLfo.in_freq();
    // filterModLfo.out_sine() >> ui.filterModAmt.ch(v) >> filter.in_cutoff();


    // BREATH CONTROL OSCILLATOR
    controlAmp.set(1.0f);
    breathRate >> controlLFO.in_freq();
    0.95 >> meterGate.in_threshold();

}

void audioModule::Breather::startBreathing() { 

    // start the lfo
    controlLFO.out_sine() >> controlAmp >> intoVoid;
    controlLFO.out_sine() >> meterGate >> intoVoid;

}

float audioModule::Breather::meter_pitch() const{
    return osc1.meter_pitch();
}

float audioModule::Breather::ctrlLfoOut() const {
    return controlAmp.meter_output();
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






void audioModule::Pumper::initiate(){

    // INPUTS
    addModuleInput("trig", triggers); 
    addModuleInput("pitch", osc.in_pitch()); 
    addModuleInput("impulseRate", impulseRate); 
    addModuleInput("trigPhase", impulseLFO.in_retrig()); 
    addModuleInput("phase", impulseLFO.in_phase_start()); 
    addModuleInput("velocity", velocity); 
    
    // OUTPUTS
    addModuleOutput("signal", gain ); 
    // addModuleOutput("signal", testOut ); 
    addModuleOutput("blackhole", intoVoid ); 


    // PATCHING

    osc >> amp >> delaySignal >> gain;  // main signal chain with delay to sync to the visual animation
    
    
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

 
}   


void audioModule::Pumper::startImpulse(){

    // initialize the impulse LFO
    impulseLFO.out_trig() >> controlEnv >> intoVoid;  // the control envelope has to be patched to a blackhole in order to being processed by the DSP
    impulseLFO.out_trig() >> triggers;  // trigger the sound
    impulseLFO.out_phase() >> meterAmp >> intoVoid;

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
    return controlEnv.meter_output();
    // return outNode.getCRValue();
}

float audioModule::Pumper::impulseCount() const {
    return controlEnv.meter_triggers();
}

pdsp::Patchable & audioModule::Pumper::in_trig() {
    return in("trig");
}
pdsp::Patchable & audioModule::Pumper::in_pitch() {
    return in("pitch");
}






void audioModule::Neuron::init(){


    // INPUTS
    addModuleInput("trig", trigger); 
    addModuleInput("pitch", pitch);
    addModuleInput("sigDuration", signalDuration);
    // addModuleInput("gain", gain);
    // addModuleInput("cutoff", filter.in_cutoff() );
    
    // addModuleInput("timeMod", timeModLFO.in_freq());
    // addModuleInput("tone", mix.in_fade());


    // OUTPUTS
    addModuleOutput("output", gain );



    // SIGNAL PATH
    
    // mix >> amp >> gain;
    mix >> filter >> amp >> gain;
    // gain >> dBtoLin >> amp.in_mod();



    // CONTOL PATCHING
    // 60 >> pitch;
    guiPtr->nronGain >> dBtoLin >> gain.in_mod();
    guiPtr->nronPitch >> pitch;
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
                filterCutoff >> filter.in_cutoff(); 
                // 20 >> filter.in_cutoff(); 
                  filterReso >> filter.in_reso();
                                //    ui.filterModSpeed >> filterModLfo.in_freq();
    // filterModLfo.out_sine() >> ui.filterModAmt.ch(v) >> filter.in_cutoff();



    // ENVELOPES AND TRIGGERS

    // Amp envelope
    ampEnv.set(1.0, 100.0, 20.0, 1.0);    // Attack, Hold, Release, Velocity
    signalDuration >> ampEnv.in_hold();

    trigger >> ampEnv >> amp.in_mod();

    // Filter envelope
    fltrEnv.set(1.0, 1.0, 100.0);    // Attack, Hold, Release, Velocity
    signalDuration >> fltrEnv.in_release();

    trigger >> fltrEnv * guiPtr->nronFilterModAmt >> filter.in_cutoff();



    pitchEnv.set(0.0f, 0.0f,  250.0f);
    signalDuration * 2. >> pitchEnv.in_release();

    trigger >> pitchEnv;
    // pitchEnv * 0.5 >> osc1.in_pitch();
    // pitchEnv * 0.5 >> osc2.in_pitch();

}





void audioModule::Intestine::init(){


    // INPUTS
    addModuleInput("trig", trigger); 
    addModuleInput("pitch", pitch);
    addModuleInput("lfoRate", lfoRate);
    // addModuleInput("gain", gain);
    // addModuleInput("cutoff", filter.in_cutoff() );
    
    // addModuleInput("timeMod", timeModLFO.in_freq());
    // addModuleInput("tone", mix.in_fade());


    // OUTPUTS
    addModuleOutput("output", gain );




    // CONTOL PATCHING
    // 60 >> pitch;
    guiPtr->itstGain >> dBtoLin >> gain.in_mod();
    guiPtr->itstPitch >> pitch;
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

    // guiPtr->itstFEnvAttack       >> fltrEnv.in_attack();
    // guiPtr->itstFEnvHold         >> fltrEnv.in_hold();
    // guiPtr->itstFEnvRelease      >> fltrEnv.in_release();




    // SIGNAL PATH
    
    // mix >> amp >> gain;
    // mix >> filter >> amp >> gain;       
    mix * 4.0f >> saturator >> filter >> amp >> gain;       // saturator?





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

    // Filter envelope
    // fltrEnv.set(1.0, 1.0, 100.0);    // Attack, Hold, Release, Velocity
    // signalDuration >> fltrEnv.in_release();

    // trigger >> fltrEnv * guiPtr->itstFilterModAmt >> filter.in_cutoff();



    // pitchEnv.set(0.0f, 0.0f,  250.0f);
    // signalDuration * 2. >> fltrEnv.in_release();

    // trigger >> pitchEnv;
    // pitchEnv * 12. >> osc1.in_pitch();
    // pitchEnv * 12. >> osc2.in_pitch();

}