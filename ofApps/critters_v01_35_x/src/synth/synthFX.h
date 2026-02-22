
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"


class StereoDelay : public pdsp::Patchable {

public:
    StereoDelay() { patch(); };
    StereoDelay(const StereoDelay & other) { patch(); };
    
    ofParameterGroup parameters;

    pdsp::ParameterAmp lDelayTimeControl;
    pdsp::ParameterAmp rDelayTimeControl;
    
    pdsp::Parameter lFeedbackControl;
    pdsp::Parameter rFeedbackControl;

    pdsp::Patchable & ch( int index );

private:
    struct Submodule : public pdsp::Patchable {
        Submodule();
        pdsp::PatchNode input;
        pdsp::PatchNode output;
    };
    
    std::vector<Submodule> channels;

    pdsp::ParameterGain inputFader;
    pdsp::ParameterGain outputFader;

    void patch();

    pdsp::Delay delay0;
    pdsp::Delay delay1;
    
    pdsp::OneBarTimeMs time;

    pdsp::Parameter dampingControl;

    pdsp::LinearCrossfader mixL;
    pdsp::LinearCrossfader mixR;
    pdsp::Parameter mixControl;
    
};



class StereoChorus : public pdsp::Patchable {

public:
    StereoChorus() { patch(); };
    StereoChorus(const StereoChorus & other) { patch(); };
    
    ofParameterGroup parameters;

    pdsp::Patchable & ch( int index );

private:
    struct Submodule : public pdsp::Patchable {
        Submodule();
        pdsp::PatchNode input;
        pdsp::PatchNode output;
    };
    
    std::vector<Submodule> channels;

    pdsp::DimensionChorus   chorus;       
        
    pdsp::Parameter chorusSpeed;
    pdsp::Parameter chorusDepth;
    pdsp::Parameter chorusDelay;

    pdsp::LinearCrossfader mixL;
    pdsp::LinearCrossfader mixR;
    pdsp::Parameter mixControl;


    pdsp::ParameterGain inputFader;
    pdsp::ParameterGain outputFader;

    void patch();

    
};


namespace effect {  
    



    class BasiVerb : public pdsp::Patchable {

    public:    
        BasiVerb() { patch(); }
        BasiVerb( const BasiVerb & other ) { patch (); }
        
        float meter_lfo() const;

        pdsp::Patchable & in_signal();

        pdsp::Patchable & ch( size_t index );

        ofParameterGroup & label( string name );

        ofParameterGroup parameters;
            pdsp::ParameterGain	gain;
            pdsp::Parameter     timeControl;
            pdsp::Parameter     densityControl;
            pdsp::Parameter     dampingControl;
            pdsp::Parameter     hiCutControl;
            pdsp::Parameter     modFreqControl;
            pdsp::Parameter     modAmountControl;
            pdsp::Parameter     lowCutControl;
        
    private:    
        void patch();
            
        pdsp::BasiVerb 	    reverb;
        pdsp::LowCut        lowcut;
        
    };
        



        
    class Cruncher : public pdsp::Patchable {

    public:    
        Cruncher() { patch(); }
        Cruncher( const Cruncher & other ) { patch (); }
        
        ofParameterGroup parameters;

        pdsp::Patchable& in_decimate_mod();

        void smoothing( float ms );

        float meter_decimate_pitch() const;

        pdsp::Patchable & ch( size_t index );
        
        ofParameterGroup & label( std::string name );

    private:    
        void patch();
        std::vector<pdsp::ChannelNode> channels;


        pdsp::Decimator     decimate0;
        pdsp::Decimator     decimate1;
        pdsp::Bitcruncher   bitcrunch0;
        pdsp::Bitcruncher   bitcrunch1;
        pdsp::PitchToFreq   p2f;

        pdsp::Parameter     cutoffControl;
        pdsp::Parameter     cutoffModControl;
        pdsp::Amp           cutoffModAmt;
        pdsp::Parameter     bitsControl;

    };





    class QuadDelay : public pdsp::Patchable {

    public:
        QuadDelay() { patch(); };
        QuadDelay(const QuadDelay & other) { patch(); };
        
        ofParameterGroup parameters;

        pdsp::ParameterAmp DelayTimeControl;
        // pdsp::ParameterAmp rDelayTimeControl;
        
        pdsp::Parameter FeedbackControl;
        // pdsp::Parameter rFeedbackControl;

        pdsp::Patchable & ch( int index );

    private:
        struct Submodule : public pdsp::Patchable {
            Submodule();
            pdsp::PatchNode input;
            pdsp::PatchNode output;
        };
        
        std::vector<Submodule> channels;

        pdsp::ParameterGain inputFader;
        pdsp::ParameterGain outputFader;

        void patch();

        pdsp::Delay delay0;
        pdsp::Delay delay1;
        pdsp::Delay delay2;
        pdsp::Delay delay3;
        
        pdsp::OneBarTimeMs time;

        pdsp::Parameter dampingControl;

        pdsp::LinearCrossfader mix1;
        pdsp::LinearCrossfader mix2;
        pdsp::LinearCrossfader mix3;
        pdsp::LinearCrossfader mix4;
        pdsp::Parameter mixControl;
        
    };





    class QuadChorus : public pdsp::Patchable {

    public:
        QuadChorus() { patch(); };
        QuadChorus(const QuadChorus & other) { patch(); };
        
        ofParameterGroup parameters;

        pdsp::Patchable & ch( int index );

    private:
        struct Submodule : public pdsp::Patchable {
            Submodule();
            pdsp::PatchNode input;
            pdsp::PatchNode output;
        };
        
        std::vector<Submodule> channels;

        pdsp::DimensionChorus   chorus0;       
        pdsp::DimensionChorus   chorus1;
        pdsp::DimensionChorus   chorus2;
        pdsp::DimensionChorus   chorus3;       
            
        pdsp::Parameter chorusSpeed;
        pdsp::Parameter chorusDepth;
        pdsp::Parameter chorusDelay;

        pdsp::LinearCrossfader mix0;
        pdsp::LinearCrossfader mix1;
        pdsp::LinearCrossfader mix2;
        pdsp::LinearCrossfader mix3;
        pdsp::Parameter mixControl;


        pdsp::ParameterGain inputFader;
        pdsp::ParameterGain outputFader;

        void patch();

        
    };



    class DisasterFX : public pdsp::Patchable {

    public:
        DisasterFX() { patch(); };
        DisasterFX(const DisasterFX & other) { patch(); };
        
        ofParameterGroup parameters;

        pdsp::Patchable & ch( int index );
        pdsp::Patchable & reverb_ch( int index );
        pdsp::Patchable & distortion_ch( int index );      
        

    private:
        struct Submodule : public pdsp::Patchable {
            Submodule();
            pdsp::PatchNode input;
            pdsp::PatchNode output;
        };


        void patch();

        
        std::vector<Submodule> channels;
        
		std::vector <effect::BasiVerb>		reverb;
        std::vector <effect::Cruncher>		bitCrush;
		// std::vector <pdsp::Saturator1>		saturator;    
        pdsp::Saturator1		saturator0;
        pdsp::Saturator1		saturator1;
        pdsp::Saturator1		saturator2;
        pdsp::Saturator1		saturator3;


        std::vector <pdsp::LinearCrossfader>    mix;
        std::vector <pdsp::PatchNode>		    switcherCtrl;


        pdsp::Switch	        switcher0;
        pdsp::Switch	        switcher1;
        pdsp::Switch	        switcher2;
        pdsp::Switch	        switcher3;
      
        
    };

    

}   // end namespace

