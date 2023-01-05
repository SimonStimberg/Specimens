
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
// #include "synth.h"


class PolySynth;


class Voice : public pdsp::Patchable { // internal class ----------------------
        //friend class PolySynth;
    public:
        Voice(){}
        Voice(const Voice& other){}
        
        void setup(PolySynth * synth, int v, int pitch);  
            
        pdsp::Patchable & in_pan() { return in("pan"); }
        
//        float meter_mod_env() const;
       float meter_pitch() const;

       pdsp::Patchable & in_pitch() { return in("pitch"); }
       pdsp::Patchable & in_gain() { return in("gain"); }
       pdsp::Patchable & in_cutoffFreq() { return in("cutoff"); }
       pdsp::Patchable & in_timeMod() { return in("timeMod"); }

    private:
        pdsp::PatchNode     voiceTrigger;
        pdsp::PatchNode     voicePitch;
        pdsp::PatchNode     voiceLevel;
        
        pdsp::VAOscillator          osc1;
        pdsp::VAOscillator          osc2;
        pdsp::LFO					driftLFO1;
        pdsp::LFO					driftLFO2;
        pdsp::LFO					vibratoLfo;
        pdsp::LFO                  lfo;

        pdsp::VAFilter              filter;
        pdsp::LFO			        filterModLfo;
        pdsp::Amp                   voiceAmp;
        pdsp::LinearCrossfader      mix;
        pdsp::Panner                pan;

        pdsp::AHR                   shotEnv;
        pdsp::LFOPhasor             timeModLFO;

        pdsp::ValueControl          valCtrl;

        PolySynth *                 synthPtr;
        

        // pdsp::ADSR          ampEnv;
        // pdsp::ADSR          modEnv;
        
        
     
}; // end Voice class -------------------------------------------------------