
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
