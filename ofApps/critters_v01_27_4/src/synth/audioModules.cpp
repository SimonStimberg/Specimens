// #pragma once
#include "audioModules.h"
#include "ofApp.h"



void audioModule::Breather::setup(int pitch){


    // INPUTS
    addModuleInput("pitch", voicePitch);
    addModuleInput("gain", voiceLevel);
    addModuleInput("cutoff", filter.in_cutoff() );
    
    addModuleInput("timeMod", timeModLFO.in_freq());
    addModuleInput("tone", mix.in_fade());


    // OUTPUTS
    addModuleOutput("output", amp );



    // SIGNAL PATH
    
    // mix >> filter >> amp >> pan;
    // mix >> amp >> pan;
    mix >> filter >> amp;
    voiceLevel >> amp.in_mod();



    // DEFAULT VALUES
    // 1.0 >> amp.in_mod();
    pitch >> voicePitch;
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


    //  voiceTrigger >> modEnv.set(400.0f, 600.0f, 1.0f, 800.0f) >> ui.modAmt.ch(v) >> filter.in_cutoff(); 

    
    // (voiceTrigger >> (ampEnv.set(200.0f, 400.0f, 1.0f, 600.0f ) * 0.1f)) >> voiceAmp.in_mod();
                    // ui.voiceLevels[v] >> voiceAmp.in_mod();
                        //    voiceLevel >> voiceAmp.in_mod();
            //  ui.voiceFilterCutoffs[v] >> filter.in_cutoff(); 


    // TRIGGER STUFF

    // Set the envelope that gets triggered
    // shotEnv.set(10.0, 20.0, 50.0, 1.0);    // Attack, Hold, Release, Velocity

    // 1.0 >> timeModLFO;  // default frequency 1 Hz

    // do the routing
    // timeModLFO.out_trig() >> shotEnv >> voiceAmp.in_mod();; 
        
    

        // ui.oscMix >> mix.in_fade();  
        // -1.0f >> pan.in_pan();
}

float audioModule::Breather::meter_pitch() const{
    return osc1.meter_pitch();
}






void audioModule::Pumper::patch(){

    addModuleInput("trig", triggers); 
    addModuleInput("pitch", osc.in_pitch()); 
    addModuleOutput("signal", gain ); 
    addModuleOutput("blackhole", controlEnv ); 

    //patching
                     triggers >> aEnv >> amp.in_mod();
                                                           osc >> amp >> gain;
    
 
                                        pEnv >>fbModAmt >> osc.in_fb();
    triggers >> pEnv.set(0.0f, 0.0f,  50.0f) >> pModAmt >> osc.in_pitch();
                                           guiPtr->pmprPitchControl >> osc.in_pitch();

    guiPtr->pmprGainControl >> dBtoLin  >> gain.in_mod();
    guiPtr->pmprVeloToEnv       >> pEnv.in_velocity();
    guiPtr->pmprPEnvAmtControl     >> pModAmt.in_mod();
    
    guiPtr->pmprEnvAttackControl  >> aEnv.in_attack();
    guiPtr->pmprEnvHoldControl    >> aEnv.in_hold();
    guiPtr->pmprEnvReleaseControl >> aEnv.in_release();
    
    guiPtr->pmprPEnvAttackControl  >> pEnv.in_attack();
    guiPtr->pmprPEnvReleaseControl >> pEnv.in_release();
    
    guiPtr->pmprFbControl >> osc.in_fb();
    guiPtr->pmprFbModControl >> fbModAmt.in_mod();



    // CONTROL IMPULSE

    // Set the envelope that gets triggered
    controlEnv.set(10.0, 20.0, 50.0, 1.0);    // Attack, Hold, Release, Velocity

    0.5 >> impulseLFO;  // default frequency 1 Hz

    delaySignal.set(500);
    gate.set(0.5, 0.5);

    // do the routing
    impulseLFO.out_trig() >> controlEnv; // >> amp.in_mod(); 
    // controlEnv >> gate >> triggers;
    // controlEnv >> delaySignal >> amp.in_mod();
    impulseLFO.out_trig() >> triggers;

    // process.process(controlEnv);

    guiPtr->pmprImpulseEnvAttack  >> controlEnv.in_attack();
    guiPtr->pmprImpulseEnvHold    >> controlEnv.in_hold();
    guiPtr->pmprImpulseEnvRelease >> controlEnv.in_release();




    // parameters.setName("sine percussion");
    // parameters.add( gainControl.set("gain", -12, -48, 24) );
    // parameters.add( pitchControl.set("pitch", 36, 24, 132 ));
    // parameters.add( pEnvAmtControl.set("pitch env", 48, 0, 72 ) );
    // parameters.add( pEnvAttackControl.set("p env attack",  0, 0, 200 ) );
    // parameters.add( pEnvReleaseControl.set("p env release", 50, 5, 500 ) );
    // parameters.add( veloToEnv.set( "p env dyn amt", 0.5f, 0.0f, 1.0f ) );
    // parameters.add( fbControl.set( "feedback", 0.0f, 0.0f, 0.25f ) );
    // parameters.add( fbModControl.set( "env to fb", 0.0f, 0.0f, 0.25f ) );
    // parameters.add( envAttackControl.set("attack",  3, 0, 200 ) );
    // parameters.add( envHoldControl.set( "hold",  40, 0, 300 ) );
    // parameters.add( envReleaseControl.set("release", 20, 5, 600 ) );
    
}   

ofParameterGroup & audioModule::Pumper::label( string name ){
    parameters.setName( name );
    return parameters;
}

void audioModule::Pumper::longerEnvelopes() {
    envAttackControl.set("attack",  3, 0, 2000 );
    envHoldControl.set( "hold",  40, 0, 2000 );
    envReleaseControl.set("release", 20, 5, 4000 );
    pEnvAttackControl.set("p env attack",  0, 0, 2000 );
    pEnvReleaseControl.set("p env release", 50, 5, 2000 ); 
}

void audioModule::Pumper::enableDB( float minValue ){
    aEnv.enableDBTriggering( minValue, 0.0f);
}
void audioModule::Pumper::disableDB( ){
    aEnv.disableDBTriggering( );
}

float audioModule::Pumper::meter() const {
    return aEnv.meter_output();
}

float audioModule::Pumper::impulseOut() const {
    return controlEnv.meter_output();
    // return outNode.getCRValue();
}

pdsp::Patchable & audioModule::Pumper::in_trig() {
    return in("trig");
}
pdsp::Patchable & audioModule::Pumper::in_pitch() {
    return in("pitch");
}