
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"



namespace audioModule {


class Breather : public pdsp::Patchable { // internal class ----------------------
        //friend class PolySynth;
    public:
        Breather(){}
        Breather(const Breather& other){}
        
        // void setup(PolySynth * synth, int v, int pitch);  
        void setup(int pitch);  
            
        // pdsp::Patchable & in_pan() { return in("pan"); }
        
//        float meter_mod_env() const;
       float meter_pitch() const;

       pdsp::Patchable & in_pitch() { return in("pitch"); }
       pdsp::Patchable & in_gain() { return in("gain"); }
       pdsp::Patchable & in_cutoff() { return in("cutoff"); }
       pdsp::Patchable & in_timeMod() { return in("timeMod"); }
    //    pdsp::Patchable & out_sig() { return out("output"); }

    private:
        pdsp::PatchNode     voiceTrigger;
        pdsp::PatchNode     voicePitch;
        pdsp::PatchNode     voiceLevel;
        pdsp::PatchNode     fineTune;
        
        pdsp::PatchNode     pulseWidth;
        pdsp::Amp           pwmAmount;
        pdsp::PatchNode     pwmSpeed;
        pdsp::PatchNode     filterCutoff;
        pdsp::PatchNode     filterReso;

        
        pdsp::VAOscillator          osc1;
        pdsp::VAOscillator          osc2;
        pdsp::LFO					driftLFO1;
        pdsp::LFO					driftLFO2;
        pdsp::LFO					vibratoLfo;
        pdsp::LFO                   lfo;

        pdsp::VAFilter              filter;
        pdsp::LFO			        filterModLfo;
        pdsp::Amp                   amp;
        pdsp::LinearCrossfader      mix;

        pdsp::AHR                   shotEnv;
        pdsp::LFOPhasor             timeModLFO;
        

        // pdsp::ADSR          ampEnv;
        // pdsp::ADSR          modEnv;
        
        
     
}; // end Breather class -------------------------------------------------------


class Pumper : public pdsp::Patchable{ 
    // class based on SinePercussion by Nicola Pisanti

    public:
        Pumper(){ patch(); }
        Pumper(const Pumper & other){ patch(); }

        void longerEnvelopes();

        void enableDB( float minValue=-18.0f );
        void disableDB( );
        
        float meter() const ;
        float impulseOut() const ;

        pdsp::Patchable & in_trig();
        pdsp::Patchable & in_pitch();
        pdsp::Patchable & out_void() { return out("blackhole"); }

        ofParameterGroup parameters;

        ofParameterGroup & label( string name );

    private:
        void patch();

        pdsp::PatchNode     triggers;
        pdsp::FMOperator    osc;
        pdsp::AHR           aEnv;
        pdsp::AHR           pEnv;
        pdsp::Amp           amp;
        pdsp::Amp           pModAmt;

        pdsp::Parameter        fbModControl;
        pdsp::Parameter        fbControl;
        pdsp::Amp              fbModAmt;
        
        pdsp::Parameter        pitchControl;    
        pdsp::Parameter        veloToEnv;
        pdsp::Parameter        pEnvAmtControl;
        pdsp::Parameter        pEnvAttackControl;
        pdsp::Parameter        pEnvReleaseControl;

        pdsp::Parameter        envAttackControl;
        pdsp::Parameter        envHoldControl;
        pdsp::Parameter        envReleaseControl;

        pdsp::Amp              gain;
        pdsp::DBtoLin          dBtoLin;
        pdsp::Parameter        gainControl;  


        pdsp::AHR          controlEnv;
        pdsp::LFOPhasor    impulseLFO;
        pdsp::LHDelay      delaySignal;
        pdsp::ToGateTrigger gate;


};

} // end namespaces