
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"
#include "synthFX.h"
#include "voices.h"


class PolySynth : public pdsp::Patchable {

    public:


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



