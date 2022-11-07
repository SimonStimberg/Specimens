
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"
#include "synthFX.h"

class PolySynth : public pdsp::Patchable {

public:


    class Voice : public pdsp::Patchable { // internal class ----------------------
        //friend class PolySynth;
    public:
        Voice(){}
        Voice(const Voice& other){}
        
        void setup(PolySynth & ui, int v, int pitch);  
            
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
        

        // pdsp::ADSR          ampEnv;
        // pdsp::ADSR          modEnv;
        
        
     
    }; // end Voice class -------------------------------------------------------




    // public API --------------------------------------------------------------
    void setup();
    void patch();
    void addVoice(float pitch);
    void setUI();
    void setVoiceLevels(vector <float> & levels);
    void setVoicePans(vector <float> & panPos);
    void setVoiceCutoffs(vector <float> & cutFreqs);
    // float getOutputMeter() { return envFollowerPeak.meter_output(); }


    // pdsp modules
    pdsp::Engine            engine;
	pdsp::ParameterGain     gain;
    pdsp::ParameterAmp      masterBus;
    pdsp::LowCut            loCut;
    pdsp::Compressor        comp;

    // pdsp::AbsoluteValue     absolute;
    // pdsp::EnvelopeFollower  envFollowerPeak;

    std::vector<Voice>              voices;
    std::vector<pdsp::Parameter>    voiceLevels;            // redundancy because of testing which way is better to control Parameter or ValueControl?
    std::vector<pdsp::Parameter>    voiceFilterCutoffs; 
    std::vector<pdsp::ValueControl> voiceAmps;
    std::vector<pdsp::ValueControl> voiceCutoffFreqs;   
    std::vector<pdsp::ValueControl> voicePitchDivergence;   
    std::vector<pdsp::ValueControl> voiceTimeMods;   
    std::vector<float>              voicePitches;

    StereoDelay delay;
    StereoChorus chorus;


    // GUI stuff
    ofxPanel            gui;
    ofParameterGroup    uiOsc;
        ofParameterGroup    osc1;
        ofParameterGroup    osc2;
    ofParameterGroup    uiFilter;
    
    ofxPanel            guiVoices;
   


private: // --------------------------------------------------------------------
    
    // oscillators control
    pdsp::ParameterAmp  drift_amt;
    pdsp::Parameter     oscMix;

    pdsp::Parameter     pw;
    pdsp::Parameter     pwmSpeed;
    pdsp::ParameterAmp  pwmAmt;  

    pdsp::Parameter     octaveShift;
    pdsp::Parameter     fineTune;

    pdsp::Parameter     vibratoSpeed;
    pdsp::ParameterAmp  vibratoAmt;


    // filter control
    pdsp::Parameter     cutoff;
    pdsp::Parameter     reso;		
    
    pdsp::Parameter     filterModSpeed;
	pdsp::ParameterAmp  filterModAmt; 

    	

};



