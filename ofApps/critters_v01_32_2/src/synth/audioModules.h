
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"


// THIS HEADER CLASS CONTAINS THE AUDIOMODULE CLASSES FOR ALL ORGANISMS


namespace audioModule {     // all class reside within the name space audioModule





    // BREATHER AUDIOMODULE
    //------------------------------------------------------------------
    class Breather : public pdsp::Patchable { 
            

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
            pdsp::Patchable & in_roughness() { return in("roughness"); }
            pdsp::Patchable & bypass() { return in("bypass"); }
            pdsp::Patchable & out_void() { return out("blackhole"); }


        private:

            pdsp::PatchNode     voicePitch;
            pdsp::PatchNode     level;
            pdsp::PatchNode     fineTune;
            pdsp::PatchNode     breathRate;
            pdsp::Switch        bypass;
            
            
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

            pdsp::WhiteNoise            noise;
            pdsp::BitNoise              bitNoise;
            pdsp::Amp                   noiseAmp;

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
            
        
    }; // end Breather class -------------------------------------------------------





    // PUMPER AUDIOMODULE
    //------------------------------------------------------------------
    class Pumper : public pdsp::Patchable{ 
        // class loosly based on SinePercussion by Nicola Pisanti

        public:

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
            pdsp::Patchable & in_trigPhase() { return in("trigPhase"); };
            pdsp::Patchable & in_phase() { return in("phase"); };
            pdsp::Patchable & in_pitch();
            pdsp::Patchable & in_impulseRate() { return in("impulseRate"); }
            pdsp::Patchable & in_velocity() { return in("velocity"); }
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
            pdsp::PatchNode         velocity;

            pdsp::AHR               controlEnv;
            pdsp::LFOPhasor         impulseLFO;
            pdsp::PatchNode         impulseRate;

            pdsp::LHDelay           delaySignal;
            pdsp::PatchNode         intoVoid;
            pdsp::AHR               ghostEnv;

            pdsp::Amp               meterAmp;
            
    }; // end Pumper class -------------------------------------------------------





    // NEURON AUDIOMODULE
    //------------------------------------------------------------------
    class Neuron : public pdsp::Patchable { 


        public:

            Neuron(){}
            Neuron(const Neuron& other){}
            
            void init();  

            pdsp::Patchable & in_trig() { return in("trig"); }
            pdsp::Patchable & in_pitch() { return in("pitch"); }
            pdsp::Patchable & in_sigDuration() { return in("sigDuration"); }


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
            
                
        
    }; // end Neuron class -------------------------------------------------------





    // INTESTINE AUDIOMODULE
    //------------------------------------------------------------------
    class Intestine : public pdsp::Patchable { 


        public:

            Intestine(){}
            Intestine(const Intestine& other){}
            
            void init();  
                
            pdsp::Patchable & in_trig() { return in("trig"); }
            pdsp::Patchable & in_pitch() { return in("pitch"); }
            pdsp::Patchable & in_LFOrate() { return in("lfoRate"); }
            pdsp::Patchable & in_cutoff() { return in("cutoff"); }


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
                
                
        
    }; // end Intestine class -------------------------------------------------------




} // end namespaces