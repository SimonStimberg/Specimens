
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"



namespace audioModule {


class Breather : public pdsp::Patchable { // internal class ----------------------
        //friend class PolySynth;
    public:
        Breather(){}
        Breather(const Breather& other){}
        
        void setup(int pitch);  
        void startBreathing();
                    
        float meter_pitch() const;
        float ctrlLfoOut() const;
        int cycleCount();

        pdsp::Patchable & in_pitch() { return in("pitch"); }
        pdsp::Patchable & in_level() { return in("level"); }
        pdsp::Patchable & in_tune() { return in("tune"); }
        pdsp::Patchable & in_breathRate() { return in("bRate"); }
        pdsp::Patchable & in_cutoff() { return in("cutoff"); }
    //    pdsp::Patchable & in_timeMod() { return in("timeMod"); }
    //    pdsp::Patchable & out_sig() { return out("output"); }
        pdsp::Patchable & out_void() { return out("blackhole"); }

    private:
        pdsp::PatchNode     voicePitch;
        pdsp::PatchNode     level;
        pdsp::PatchNode     fineTune;
        pdsp::PatchNode     breathRate;
        
        
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
        pdsp::DBtoLin               dBtoLin;
        pdsp::Amp                   gain;

        pdsp::LFO                   controlLFO;
        pdsp::Amp                   controlAmp;
        pdsp::ToGateTrigger         meterGate;
        pdsp::PatchNode             intoVoid;

        int  count    = 0;
        bool newPhase = false;
        

        // pdsp::ADSR          ampEnv;
        // pdsp::ADSR          modEnv;
        
        
     
}; // end Breather class -------------------------------------------------------





class Pumper : public pdsp::Patchable{ 
    // class loosly based on SinePercussion by Nicola Pisanti

    public:
        // Pumper(){ patch(); }
        // Pumper(const Pumper & other){ patch(); }
        Pumper(){}
        Pumper(const Pumper & other){}


        void initiate();
        void startImpulse();
 
        void enableDB( float minValue=-18.0f );
        void disableDB( );
        
        float meter() const ;
        float impulseOut() const ;
        float impulseCount() const ;

        pdsp::Patchable & in_trig();
        pdsp::Patchable & in_pitch();
        pdsp::Patchable & in_impulseRate() { return in("impulseRate"); }
        pdsp::Patchable & out_void() { return out("blackhole"); }

        



    private:
        

        pdsp::PatchNode     triggers;
        pdsp::FMOperator    osc;
        pdsp::AHR           aEnv;
        pdsp::AHR           pEnv;
        pdsp::Amp           amp;
        pdsp::Amp           pModAmt;

        pdsp::Amp              fbModAmt;
        pdsp::Amp              gain;
        pdsp::DBtoLin          dBtoLin;


        pdsp::AHR               controlEnv;
        pdsp::LFOPhasor         impulseLFO;
        pdsp::PatchNode         impulseRate;

        pdsp::LHDelay           delaySignal;
        pdsp::PatchNode         intoVoid;
        pdsp::AHR               ghostEnv;


 
};


class Neuron : public pdsp::Patchable { // Neuron class ----------------------


    public:

        Neuron(){}
        Neuron(const Neuron& other){}
        
        void init();  
            
        
//        float meter_mod_env() const;
    //    float meter_pitch() const;

       pdsp::Patchable & in_trig() { return in("trig"); }
       pdsp::Patchable & in_pitch() { return in("pitch"); }
       pdsp::Patchable & in_sigDuration() { return in("sigDuration"); }

    //    pdsp::Patchable & in_gain() { return in("gain"); }
    //    pdsp::Patchable & in_cutoff() { return in("cutoff"); }
    //    pdsp::Patchable & in_timeMod() { return in("timeMod"); }
    //    pdsp::Patchable & out_sig() { return out("output"); }



    private:

        // control nodes
        pdsp::PatchNode     trigger;
        pdsp::PatchNode     pitch;
        pdsp::PatchNode     fineTune;
        pdsp::PatchNode     signalDuration;
        
        pdsp::PatchNode     filterCutoff;
        pdsp::PatchNode     filterReso;

        
        // oscillators
        pdsp::VAOscillator          osc1;
        pdsp::VAOscillator          osc2;
        pdsp::LFO					driftLFO1;
        pdsp::LFO					driftLFO2;
        pdsp::LinearCrossfader      mix;

        // filter, amp & envelopes
        pdsp::VAFilter              filter;
        pdsp::AHR                   fltrEnv;
        pdsp::Amp                   amp;
        pdsp::AHR                   ampEnv;
        pdsp::DBtoLin               dBtoLin;
        pdsp::Amp                   gain;

        pdsp::AHR                   pitchEnv;
        

        // pdsp::LFO					vibratoLfo;
        // pdsp::LFO                   lfo;
        // pdsp::LFO			        filterModLfo;
        // pdsp::LFOPhasor             timeModLFO;

            
     
}; // end Neuron class -------------------------------------------------------





class Intestine : public pdsp::Patchable { // Intestine class ----------------------


    public:

        Intestine(){}
        Intestine(const Intestine& other){}
        
        void init();  
            
        
//        float meter_mod_env() const;
    //    float meter_pitch() const;

       pdsp::Patchable & in_trig() { return in("trig"); }
       pdsp::Patchable & in_pitch() { return in("pitch"); }
       pdsp::Patchable & in_LFOrate() { return in("lfoRate"); }

    //    pdsp::Patchable & in_gain() { return in("gain"); }
    //    pdsp::Patchable & in_cutoff() { return in("cutoff"); }
    //    pdsp::Patchable & in_timeMod() { return in("timeMod"); }
    //    pdsp::Patchable & out_sig() { return out("output"); }



    private:

        // control nodes
        pdsp::PatchNode     trigger;
        pdsp::PatchNode     pitch;
        pdsp::PatchNode     fineTune;
        pdsp::PatchNode     lfoRate;
        
        pdsp::PatchNode     filterCutoff;
        pdsp::PatchNode     filterReso;

        
        // oscillators
        pdsp::VAOscillator          osc1;
        pdsp::VAOscillator          osc2;
        pdsp::LFO					driftLFO1;
        pdsp::LFO					driftLFO2;
        pdsp::LinearCrossfader      mix;

        pdsp::PatchNode             pulseWidth;
        pdsp::Amp                   pwmAmount;
        pdsp::PatchNode             pwmSpeed;

        pdsp::LFO                   lfo;

        // filter, amp & envelopes
        pdsp::ADSR                  ampEnv;
        pdsp::Amp                   amp;
        pdsp::Amp                   gain;
        pdsp::DBtoLin               dBtoLin;
        pdsp::Saturator1            saturator;

        pdsp::VAFilter              filter;
        pdsp::Amp                   filterModAmt;
        // pdsp::AHR                   fltrEnv;
        
        

        // pdsp::AHR                   pitchEnv;
        

        // pdsp::LFO					vibratoLfo;
        // pdsp::LFO                   lfo;
        // pdsp::LFO			        filterModLfo;
        // pdsp::LFOPhasor             timeModLFO;

            
     
}; // end Intestine class -------------------------------------------------------




} // end namespaces