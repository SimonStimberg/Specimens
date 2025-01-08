
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "audioModules.h"
#include "synthFX.h"





namespace audioModule {     // all class reside within the name space audioModule



    // SUB MASTER AUDIO MODULE
    //------------------------------------------------------------------
    class SubMaster : public pdsp::Patchable { // internal class ----------------------
        //friend class PolySynth;
        public:
            SubMaster(){}
            SubMaster(const SubMaster& other){}

            void init();
            audioModule::Breather  & getFreeBreatherModule();
            audioModule::Pumper    & getFreePumperModule();
            audioModule::Neuron    & getFreeNeuronModule();
            audioModule::Intestine & getFreeIntestineModule();

            pdsp::ParameterAmp      sumBus;
            pdsp::ParameterAmp      blackhole;
            pdsp::Ducker            sideChainBreather;
            pdsp::Ducker            sideChainIntestine;

            vector <audioModule::Breather>    breatherModules;
            vector <audioModule::Pumper>      pumperModules;
            vector <audioModule::Neuron>      neuronModules;
            vector <audioModule::Intestine>   intestineModules;
            
            // void setup(PolySynth & ui, int v, int pitch);  

    }; // end class SubMaster ---------------------------------------------------------



    // MASTER AUDIO MODULE
    //------------------------------------------------------------------
    class Master : public pdsp::Patchable {
        
        public:

            Master(){}    
            Master(const Master& other){}
            // ~Master(){}        // destructor

            void setup(int screens, bool stereo);
            // void initSynth();
            void switchDistortion(bool state, int screen);
            void setReverbAmount(float amount, int screen);


            audioModule::SubMaster & getSubMasterModule(int screen);


            

            // MAIN AUDIO ROUTING
            pdsp::Engine            engine;		// the audio engine

            // busses, routing, parameters
            pdsp::ParameterGain     gain;
            pdsp::ParameterAmp      fxBus;
            pdsp::ParameterAmp      cleanBus;
            pdsp::ParameterAmp      masterBus;


            // effect units
            effect::QuadDelay				delay;
            effect::QuadChorus 				chorus;
            effect::DisasterFX				disasterFX;
            pdsp::LowCut					loCut;
            vector <pdsp::Compressor>		compressor;
            // pdsp::Compressor		compressorA;
            // pdsp::Compressor		compressorB;
            std::vector <pdsp::ValueControl> reverbAmt;

            vector <audioModule::SubMaster> subMasterModules;

            bool stereo;




            // AUDIO MODULES PER SCREEN
            // vector <vector <audioModule::Breather>>     breatherModules;
            // vector <vector <audioModule::Pumper>>       pumperModules;
            // vector <vector <audioModule::Neuron>>       neuronModules;
            // vector <vector <audioModule::Intestine>>   intestineModules;


    }; // end class Master ---------------------------------------------------------



    

}