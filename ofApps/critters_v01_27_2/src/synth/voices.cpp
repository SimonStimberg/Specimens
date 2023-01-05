
#include "voices.h"



// void Voice::setup( PolySynth * synth, int v, int pitch){
void Voice::setup(int pitch){


    // INPUTS
    addModuleInput("pitch", voicePitch);
    addModuleInput("gain", voiceLevel);
    // addModuleInput("timeMod", timeMod.in_distance());
    addModuleInput("timeMod", timeModLFO.in_freq());
    // addModuleInput("gain", amp.in_mod());
    addModuleInput("cutoff", filter.in_cutoff() );
    addModuleInput("tone", mix.in_fade());


    // OUTPUTS
    addModuleOutput("output", amp );
    // addModuleOutput("L", amp.out_L() );
    // addModuleOutput("R", amp.out_R() );
        // addModuleOutput("signal", amp);



    // synthPtr = synth;

    



    // SIGNAL PATH
    
    // mix >> filter >> amp >> pan;
    // mix >> amp >> pan;
    mix >> filter >> amp;
    voiceLevel >> amp.in_mod();


    // DEFAULT VALUES
    // 1.0 >> amp.in_mod();
    // 0.0f >> pan.in_pan();
    pitch >> voicePitch;
    0.5 >> pulseWidth;
    0.0729167 >> pwmAmount;    
    0.3 >> pwmSpeed;
    69 >> filterCutoff;
    0.352041 >> filterReso;
    0.5 >> mix.in_fade();  // mix between the two OSCs

    




    // OSC 1 - Pulse Wave
                  osc1.out_pulse() >> mix.in_A();
                        voicePitch >> osc1.in_pitch();
    0.13f >> driftLFO1.out_random() >> osc1.in_pitch();   
        //  vibratoLfo.out_triangle() >> osc1.in_pitch();  
    // 1.5f >> driftLFO1.out_random() >> ui.drift_amt.ch(v) >> osc1.in_pitch();   
    //     vibratoLfo.out_triangle() >> ui.vibratoAmt.ch(v) >> osc1.in_pitch();  


    // OSC 2 - Sine Wave
                    osc2.out_sine() >> mix.in_B();
                         voicePitch >> osc2.in_pitch();
    0.13f >> driftLFO2.out_random() >> osc2.in_pitch();
                           0.078125 >> osc2.in_pitch(); // fine detune for slight beating
        //  vibratoLfo.out_triangle() >> osc2.in_pitch();    
                                    //  ui.octaveShift * 12 >> osc2.in_pitch();  
                                    //          ui.fineTune >> osc2.in_pitch();
           
    // 1.5f >> driftLFO2.out_random() >> ui.drift_amt.ch(v) >> osc2.in_pitch();
    //                                  ui.octaveShift * 12 >> osc2.in_pitch();  
    //                                          ui.fineTune >> osc2.in_pitch();
    //     vibratoLfo.out_triangle() >> ui.vibratoAmt.ch(v) >> osc2.in_pitch();  



    

    
    // MODULATIONS AND CONTROL
                     pulseWidth >> osc1.in_pw();
    lfo.out_sine() >> pwmAmount >> osc1.in_pw();
                       pwmSpeed >> lfo.in_freq(); 
                    //             ui.pw >> osc1.in_pw();
                    //       ui.pwmSpeed >> lfo.in_freq(); 

                    //   ui.vibratoSpeed >> vibratoLfo.in_freq();


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






// float PolySynth::Voice::meter_mod_env() const{
//     return modEnv.meter_output();
// }

float Voice::meter_pitch() const{
    return osc1.meter_pitch();
}
