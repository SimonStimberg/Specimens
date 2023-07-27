
#include "synthFX.h"


// DELAY
StereoDelay::Submodule::Submodule(){
    addModuleInput( "signal", input );
    addModuleOutput( "signal", output );
}

void StereoDelay::patch() {

    channels.resize(2);
    
    addModuleInput("L", channels[0] );
    addModuleInput("R", channels[1] );
    addModuleOutput("L", channels[0] );
    addModuleOutput("R", channels[1] );

    // patching dry and wet
    channels[0].input >> inputFader.ch(0) >> mixL.in_A();;
    channels[1].input >> inputFader.ch(1) >> mixR.in_A();;

    channels[0].input >> inputFader.ch(0) >> delay0 >> mixL.in_B();;
    channels[1].input >> inputFader.ch(1) >> delay1 >> mixR.in_B();;

    // routing the d/w mix to the output
    mixL >> outputFader.ch(0) >> channels[0].output;
    mixR >> outputFader.ch(1) >> channels[1].output;

    mixControl >> mixL.in_fade();
    mixControl >> mixR.in_fade();


    time * (1.f/16.f) >> lDelayTimeControl >> delay0.in_time();
    time * (1.f/16.f) >> rDelayTimeControl >> delay1.in_time();

    lFeedbackControl >> delay0.in_feedback();
    rFeedbackControl >> delay1.in_feedback();
    dampingControl >> delay0.in_damping();
    dampingControl >> delay1.in_damping();

    parameters.setName("stereo delay");

    parameters.add( inputFader.set("input gain", 0, -48, 12) );
    
    parameters.add( lDelayTimeControl.set("L 16th time", 5, 1, 16) );
    parameters.add( lFeedbackControl.set("L feedback", 0.14f, 0.0f, 1.0f));
    parameters.add( rDelayTimeControl.set("R 16th time", 3, 1, 16) );
    parameters.add( rFeedbackControl.set("R feedback", 0.156f, 0.0f, 1.0f));   
    parameters.add( dampingControl.set("damping", 0.5f, 0.0f, 1.0f));  

    parameters.add( mixControl.set("dry / wet", 0.1f, 0.0f, 1.0f));  
   
    parameters.add( outputFader.set("output gain", 0, -48, 12) );

    inputFader.enableSmoothing(50.f);
    outputFader.enableSmoothing(50.f);
    mixControl.enableSmoothing(50.f);
    lDelayTimeControl.enableSmoothing(450.f);
    lDelayTimeControl.enableSmoothing(450.f);
    lFeedbackControl.enableSmoothing(50.f);
    rFeedbackControl.enableSmoothing(50.f);

}

pdsp::Patchable & StereoDelay::ch( int index ){
    index = index%2; // wrap to stereo 
    return channels[index];
}



// CHORUS
StereoChorus::Submodule::Submodule(){
    addModuleInput( "signal", input );
    addModuleOutput( "signal", output );
}

void StereoChorus::patch() {

    channels.resize(2);
    
    addModuleInput("L", channels[0] );
    addModuleInput("R", channels[1] );
    addModuleOutput("L", channels[0] );
    addModuleOutput("R", channels[1] );

    // patching dry and wet
    channels[0].input >> mixL.in_A();;
    channels[1].input >> mixR.in_A();;

    channels[0].input >> chorus.ch(0) >> mixL.in_B();;
    channels[1].input >> chorus.ch(1) >> mixR.in_B();;

    // routing the d/w mix to the output
    mixL >> channels[0].output;
    mixR >> channels[1].output;

    mixControl >> mixL.in_fade();
    mixControl >> mixR.in_fade();


    chorusSpeed >> chorus.in_speed();
    chorusDepth >> chorus.in_depth();
    chorusDelay >> chorus.in_delay();
    

    parameters.setName("stereo chorus");

    parameters.add( chorusSpeed.set("speed (hz)", 0.25f, 0.25f, 1.0f) );
    parameters.add( chorusDepth.set("depth (ms)", 3.5f, 1.0f, 10.0f) );
    parameters.add( chorusDelay.set("delay (ms)", 80, 0, 400) );

    parameters.add( mixControl.set("dry / wet", 0.05f, 0.0f, 1.0f) );  

    mixControl.enableSmoothing(50.f);

}

pdsp::Patchable & StereoChorus::ch( int index ){
    index = index%2; // wrap to stereo 
    return channels[index];
}




void effect::BasiVerb::patch(){
    
    addModuleInput( "signal", lowcut );
    addModuleOutput( "signal", gain.ch(0) );
    addModuleOutput( "0", gain.ch(0) );
    addModuleOutput( "1", gain.ch(1));

    lowcut >> reverb;
	reverb.ch(0) >> gain.ch(0);
    reverb.ch(1) >> gain.ch(1);
	
	timeControl 		>> reverb.in_time();
    densityControl 		>> reverb.in_density();
    dampingControl 		>> reverb.in_damping();
    hiCutControl 		>> reverb.in_hi_cut();
    modFreqControl 		>> reverb.in_mod_freq();
    modAmountControl 	>> reverb.in_mod_amount();
    lowCutControl       >> lowcut.in_freq();
    
    parameters.setName( "reverb" );
    parameters.add( gain.set("reverb gain", 12, -48, 12 ) );
    parameters.add( lowCutControl.set("low cut freq", 100, 20, 1000 ) );
    parameters.add( timeControl.set("rt60", 3.33f, 0.0f, 20.0f ) );
    parameters.add( densityControl.set("density", 0.5f, 0.0f, 1.0f ) );
    parameters.add( dampingControl.set("damping", 0.5f, 0.0f, 1.0f ) );
    parameters.add( hiCutControl.set("high cut freq", 5000, 3000, 20000 ) );
    parameters.add( modFreqControl.set("mod speed (hz)", 0.2f, 0.01f, 1.25f));
    parameters.add( modAmountControl.set("mod depth (ms)", 0.8f, 0.0f, 3.0f));
}

ofParameterGroup & effect::BasiVerb::label( string name ){
    parameters.setName( name );
    return parameters;
}

float effect::BasiVerb::meter_lfo() const {
    return reverb.meter_lfo();
}

pdsp::Patchable & effect::BasiVerb::in_signal() {
    return reverb.in_signal();
}

pdsp::Patchable & effect::BasiVerb::ch( size_t index ) {
    pdsp::wrapChannelIndex( index );
    switch( index ){
        case 0: default: return out("0"); break;
        case 1:          return out("1"); break;
    }
}



void effect::Cruncher::patch(){
    channels.resize(2);
    
    addModuleInput( "L", channels[0].input );
    addModuleInput( "R", channels[1].input );
    
    addModuleOutput( "L", channels[0].output );
    addModuleOutput( "R", channels[1].output );
    addModuleInput( "mod", cutoffModAmt );
    
    cutoffControl >> p2f >> decimate0.in_freq();
                     p2f >> decimate1.in_freq();
                     
    bitsControl      >> bitcrunch0.in_bits();
    bitsControl      >> bitcrunch1.in_bits();
    
    cutoffModAmt     >> p2f;
    cutoffModControl >> cutoffModAmt.in_mod();
    
    channels[0].input >> decimate0 >> bitcrunch0 >> channels[0].output;
    channels[1].input >> decimate1 >> bitcrunch1 >> channels[1].output;
    
    parameters.setName( "cruncher" );
    parameters.add( cutoffControl.set( "decimate", 116, 20, 148) );
    parameters.add( cutoffModControl.set( "decimate mod", 0, 0, 127) );
    parameters.add( bitsControl.set( "bits", 24, 24, 1) );
    
    smoothing(50.0f);
}

ofParameterGroup & effect::Cruncher::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

float effect::Cruncher::meter_decimate_pitch() const {
    return p2f.meter_input();
}

pdsp::Patchable& effect::Cruncher::in_decimate_mod() {
    return in("mod");
}

void effect::Cruncher::smoothing( float ms ){
    cutoffControl.enableSmoothing( ms );
    bitsControl.enableSmoothing( ms );
}

pdsp::Patchable & effect::Cruncher::ch( size_t index ){
    pdsp::wrapChannelIndex( index );
    return channels[index];
}




// 4CH DELAY
effect::QuadDelay::Submodule::Submodule(){
    addModuleInput( "signal", input );
    addModuleOutput( "signal", output );
}

void effect::QuadDelay::patch() {

    channels.resize(4);
    
    addModuleInput("A", channels[0] );
    addModuleInput("B", channels[1] );
    addModuleInput("C", channels[2] );
    addModuleInput("D", channels[3] );
    addModuleOutput("A", channels[0] );
    addModuleOutput("B", channels[1] );
    addModuleOutput("C", channels[2] );
    addModuleOutput("D", channels[3] );

    // patching dry and wet
    channels[0].input >> inputFader.ch(0) >> mix1.in_A();
    channels[1].input >> inputFader.ch(1) >> mix2.in_A();
    channels[2].input >> inputFader.ch(2) >> mix3.in_A();
    channels[3].input >> inputFader.ch(3) >> mix4.in_A();

    channels[0].input >> inputFader.ch(0) >> delay0 >> mix1.in_B();
    channels[1].input >> inputFader.ch(1) >> delay1 >> mix2.in_B();
    channels[2].input >> inputFader.ch(2) >> delay2 >> mix3.in_B();
    channels[3].input >> inputFader.ch(3) >> delay3 >> mix4.in_B();

    // routing the d/w mix to the output
    mix1 >> outputFader.ch(0) >> channels[0].output;
    mix2 >> outputFader.ch(1) >> channels[1].output;
    mix3 >> outputFader.ch(2) >> channels[2].output;
    mix4 >> outputFader.ch(3) >> channels[3].output;

    mixControl >> mix1.in_fade();
    mixControl >> mix2.in_fade();
    mixControl >> mix3.in_fade();
    mixControl >> mix4.in_fade();


    time * (1.f/16.f) >> DelayTimeControl >> delay0.in_time();
    time * (1.f/16.f) >> DelayTimeControl >> delay1.in_time();
    time * (1.f/16.f) >> DelayTimeControl >> delay2.in_time();
    time * (1.f/16.f) >> DelayTimeControl >> delay3.in_time();

    FeedbackControl >> delay0.in_feedback();
    FeedbackControl >> delay1.in_feedback();
    FeedbackControl >> delay2.in_feedback();
    FeedbackControl >> delay3.in_feedback();

    dampingControl >> delay0.in_damping();
    dampingControl >> delay1.in_damping();
    dampingControl >> delay2.in_damping();
    dampingControl >> delay3.in_damping();

    parameters.setName("Delay");

    parameters.add( inputFader.set("input gain", 0, -48, 12) );
    
    parameters.add( DelayTimeControl.set("delay time", 5, 1, 16) );
    parameters.add( FeedbackControl.set("feedback", 0.14f, 0.0f, 1.0f));
    parameters.add( dampingControl.set("damping", 0.5f, 0.0f, 1.0f));  

    parameters.add( mixControl.set("dry / wet", 0.1f, 0.0f, 1.0f));  
   
    parameters.add( outputFader.set("output gain", 0, -48, 12) );

    inputFader.enableSmoothing(50.f);
    outputFader.enableSmoothing(50.f);
    mixControl.enableSmoothing(50.f);
    DelayTimeControl.enableSmoothing(450.f);
    FeedbackControl.enableSmoothing(50.f);
    

}

pdsp::Patchable & effect::QuadDelay::ch( int index ){
    // index = index%2; // wrap to stereo 
    return channels[index];
}






// 4CH CHORUS
effect::QuadChorus::Submodule::Submodule(){
    addModuleInput( "signal", input );
    addModuleOutput( "signal", output );
}

void effect::QuadChorus::patch() {

    channels.resize(4);
    
    addModuleInput("A", channels[0] );
    addModuleInput("B", channels[1] );
    addModuleInput("C", channels[2] );
    addModuleInput("D", channels[3] );
    addModuleOutput("A", channels[0] );
    addModuleOutput("B", channels[1] );
    addModuleOutput("C", channels[2] );
    addModuleOutput("D", channels[3] );

    // patching dry and wet
    channels[0].input >> mix0.in_A();
    channels[1].input >> mix1.in_A();
    channels[2].input >> mix2.in_A();
    channels[3].input >> mix3.in_A();

    channels[0].input >> chorus0 >> mix0.in_B();
    channels[1].input >> chorus1 >> mix1.in_B();
    channels[2].input >> chorus2 >> mix2.in_B();
    channels[3].input >> chorus3 >> mix3.in_B();

    // routing the d/w mix to the output
    mix0 >> channels[0].output;
    mix1 >> channels[1].output;
    mix2 >> channels[2].output;
    mix3 >> channels[3].output;

    mixControl >> mix0.in_fade();
    mixControl >> mix1.in_fade();
    mixControl >> mix2.in_fade();
    mixControl >> mix3.in_fade();


    chorusSpeed >> chorus0.in_speed();
    chorusSpeed >> chorus1.in_speed();
    chorusSpeed >> chorus2.in_speed();
    chorusSpeed >> chorus3.in_speed();

    chorusDepth >> chorus0.in_depth();
    chorusDepth >> chorus1.in_depth();
    chorusDepth >> chorus2.in_depth();
    chorusDepth >> chorus3.in_depth();

    chorusDelay >> chorus0.in_delay();
    chorusDelay >> chorus1.in_delay();
    chorusDelay >> chorus2.in_delay();
    chorusDelay >> chorus3.in_delay();
    

    parameters.setName("Chorus");

    parameters.add( chorusSpeed.set("speed (hz)", 0.25f, 0.25f, 1.0f) );
    parameters.add( chorusDepth.set("depth (ms)", 3.5f, 1.0f, 10.0f) );
    parameters.add( chorusDelay.set("delay (ms)", 80, 0, 400) );

    parameters.add( mixControl.set("dry / wet", 0.05f, 0.0f, 1.0f) );  

    mixControl.enableSmoothing(50.f);

}

pdsp::Patchable & effect::QuadChorus::ch( int index ){
    // index = index%2; // wrap to stereo 
    return channels[index];
}





// DISASTER FX
effect::DisasterFX::Submodule::Submodule(){
    addModuleInput( "signal", input );
    addModuleOutput( "signal", output );
}

void effect::DisasterFX::patch() {

    channels.resize(4);
    reverb.resize(4);
    bitCrush.resize(4);
    mix.resize(4);
    switcherCtrl.resize(4);
    

    // all sound inputs
    addModuleInput("A", channels[0] );
    addModuleInput("B", channels[1] );
    addModuleInput("C", channels[2] );
    addModuleInput("D", channels[3] );

    // all control inputs
    addModuleInput("M0", mix[0].in_fade() );
    addModuleInput("M1", mix[1].in_fade() );
    addModuleInput("M2", mix[2].in_fade() );
    addModuleInput("M3", mix[3].in_fade() );

    addModuleInput("S0", switcherCtrl[0] );
    addModuleInput("S1", switcherCtrl[1] );
    addModuleInput("S2", switcherCtrl[2] );
    addModuleInput("S3", switcherCtrl[3] );

    // all sound outputs
    addModuleOutput("A", channels[0] );
    addModuleOutput("B", channels[1] );
    addModuleOutput("C", channels[2] );
    addModuleOutput("D", channels[3] );


    // for(unsigned int i = 0; i < reverb.size(); i++) { 

    //     3.33f 		>> reverb[i].in_time();
    //     0.5f 		>> reverb[i].in_density();
    //     0.5f 		>> reverb[i].in_damping();
    //     5000 		>> reverb[i].in_hi_cut();
    //     0.2f 		>> reverb[i].in_mod_freq();
    //     0.8f    	>> reverb[i].in_mod_amount();

    // }


    // patching the mixer for the reverb

    channels[0].input >> mix[0].in_A();
    channels[1].input >> mix[1].in_A();
    channels[2].input >> mix[2].in_A();
    channels[3].input >> mix[3].in_A();

    channels[0].input >> reverb[0] * 2.0f >> mix[0].in_B();
    channels[1].input >> reverb[1] * 2.0f >> mix[1].in_B();
    channels[2].input >> reverb[2] * 2.0f >> mix[2].in_B();
    channels[3].input >> reverb[3] * 2.0f >> mix[3].in_B();



    // patching the switch an distortion chain

    switcher0.resize(2);
    switcher1.resize(2);
    switcher2.resize(2);
    switcher3.resize(2);

    mix[0] >> switcher0.input(0);
    mix[1] >> switcher1.input(0);
    mix[2] >> switcher2.input(0);
    mix[3] >> switcher3.input(0);

    mix[0] >> bitCrush[0] * 4.0f >> saturator0 * dB(-3) >> switcher0.input(1);
    mix[1] >> bitCrush[1] * 4.0f >> saturator1 * dB(-3) >> switcher1.input(1);
    mix[2] >> bitCrush[2] * 4.0f >> saturator2 * dB(-3) >> switcher2.input(1);
    mix[3] >> bitCrush[3] * 4.0f >> saturator3 * dB(-3) >> switcher3.input(1);

    switcher0 >> channels[0].output;
    switcher1 >> channels[1].output;
    switcher2 >> channels[2].output;
    switcher3 >> channels[3].output;

    // switch control
    0.0f >> switcherCtrl[0] >> switcher0.in_select();
    0.0f >> switcherCtrl[1] >> switcher1.in_select();
    0.0f >> switcherCtrl[2] >> switcher2.in_select();
    0.0f >> switcherCtrl[3] >> switcher3.in_select();


}

pdsp::Patchable & effect::DisasterFX::ch( int index ){
    // index = index%2; // wrap to stereo 
    return channels[index];
}

pdsp::Patchable & effect::DisasterFX::reverb_ch( int index ){
    // index = index%2; // wrap to stereo 
    return mix[index].in_fade();
}

pdsp::Patchable & effect::DisasterFX::distortion_ch( int index ){
    // index = index%2; // wrap to stereo 
    return switcherCtrl[index];
}


