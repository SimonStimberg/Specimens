# Version History

 
## Version v01

###  v01.01
- Basic Particle System with
  - a Molecule Class and a Molecular System Class to manage all the molecules
  - all the Molecules are linked from the system via a pointer
  - also the system itself is referenced in each molecule via a pointer so they can communicate without copying the instances
  - a repulsion force for every molecule 

###  v01.02
- a copy of v01.01 with the difference that the molecules in the System class are not saved as pointers but as real instances
  - found it less confusing and error prone but in the end it turned out not to make any difference, hence the pointer version might be the better choise to avoid memory intense copying of instances

###  v01.03
- Iteration of v01.01 (the version with pointers)
  - added parameter in the molecular system class to control the world size (aka canvas size) from outside - so it no longer depends on the oF canvas size  
  This is a first step towards the idea of drawing the system to a Frame Buffer which can then be easier send to a micro controller (which controlls a TV)

### v01.04
- Fullscreen Mode on second monitor to test visual behavior + appearence on plugged CRT screen

### v01.05
- added a Spring class and connected the Molecules with springs

### v01.06
- added a flattening force to make the particles align with their neighbors to that they are connected via a string
  - to smoothen out the particle chains/macro-molecules into rather round curves then zigzag patterns

### v01.07
- added a Gui to tune the forces in the system
  - to adjust them between each other to achieve a natural looking behaviour
    - (in order to later derive constant values from them)
- made the flattening force propotional to the distance to the desired (completely flat) position
  - (so the more zig-zaggy the more flattening)

### v01.08
- writing a Cell class to organize all molecules that are connected with springs seperately
- and to handle the creation etc. of a single cell at once  
#### v01.08.2
- alternatively tried to solve the problem without a class

### v01.09
- Repulsion works for all molecules against all
- new function in Molecule class: expansion (aka breathing)
- reset function for all molecules and cells  
#### v01.09.2  
- extra window for the GUI

### v01.10
- with Serial connection for sending the position values to the ESP
- failed / discontinued for the moment

### v01.11
- continuation von v01.09.2
- added Gravity
- added curved vertex line for the cells to look round
- set Framerate to 50fps to be closeer to the TV native framerate (50i) 
#### v01.11.2
- save/load Parameter Presets
- save by pressing 's' and load by dragging into the preset window
- hardcoded interchangable default preset

### v01.12
- increased expansion force (breathing): now cells have the power to breathe under water (pressure) :)
- mouse interaction: a repulsion force on mouse position

### v01.13
- added signed distance functions for boundary checking
- now every implicit geometry (also combinations) can be used as boundary shape
- Molecules now live in a roundly shaped vessel, redrawing the curves of a CRT screen
#### v01.13.2
- added flush: particles can drain through a hole in the ground
- added mask for TV screen mapping and enhanced mapping interface parameters

### v01.14
- improved breathing motion + better controls for breathing
- cells tend less to overlap

### v01.15
- integrated synth module (ofxPDSP)
- every cell has a synth voice with a different pitch
- increasing inflation of the cells opens the filter of the synth voice

### v01.16
- added Neuron Class
- adjusted Parameters for Springs etc to fit for Neurons

### v01.16.2
- Neurons now have several arm elements that are drawn by curved lines
- debugged some memory errors caused by wrong iterators

### v01.16.3
- added Neuron connectivity
  - Neuron arms connect to others on command and form a net
- added enum Molecule type name
- added basic heartbeat movement fore smaller Cells

### v01.17
- Neuron no longer connect twice to the same Neuron -> more networking
- new color scheme (partly sticked to the old)

### v01.17.2
- added Neuron growth
- their arms grow until they reach a max number of elements or being connected to another neuron

### v01.17.3
- added Kinect "touch interaction"
- the audience can move their hands in front of the screen and their finger positions serve as interaction points towards the system

### v01.18
- added Intestine class
- some general restructuring: a folder structure for the source files
- fallback Mouse Interference

### v01.18.2
- added different Spring types for the Intestine
- reworked the Spring Class with types and added appropriate parameters to the Gui app

### v01.18.3
- implemented Intestine growth
- with new grown elements being added at the end

### v01.18.4
- to make the growth more natural, the adding of a new Intestine element (newly grown) should be added somewhere in the middle
- tried to implement it the real way, to splice in new molecules in between of the array, turned out super complicated

### v01.18.5
- solved the "grow in the middle" problem with a work-around
- a new element is added at the end, and the elements shift their position to the element before until the shift reaches a point in the middle
- the look is the same as the method above but with way less hustle

### v01.19
- experiments with new organism forms in the class "Gland"
- different variations of Cell combinations/alterations but nothing satisfying

### v01.19.2
- just altered the breathing Cell into a new form with an additional Spring - the rest stays the same (-> no Gland class)
- created an own header file for the organisms header directives

### v01.20
- implemented Cell growth
- Springs have a disconnect function

### v01.20.2
- introduced Cell sub-types: Breathers + Pumpers
- adjusted the behavior functions accordingly

### v01.20.3
- moved the expansion/inflation function from the Molecule class to the Cell class

### v01.21
- split up the Cell class into subTypes of Breathers and Pumpers
- reworked the getInflation function into depending on the idle length of the springs
- changed the synth to have an addVoice function
- added a Voice module input for gain

### v01.21.2
- solved the problem on Voice resetting its patching once the vector of voices is resized
- on addVoice it iterates over all Voices repatching them
- the Voice's pitches are saved in a vector in the synth class and are restored that way

### v01.21.3
- bypassed the effect chain temporarily which was resulting in low gain
- added a vector with valueControls for controlling all the Voices gain inputs with smoothing
- introduced decibel scaling (hence logarithmical scaling) for controling the gain of the voices by the inflation values

### v01.21.4
- several valueControls and custom inTags for the voices
- to control gain, cutoff and pitch
- adjusted the getInflation scaling/mapping
- added color change (saturation+brightness) of Cells according to Inflation
- added again slightly Chorus + Delay to the sum to brighten up the sound

### v01.21.5
- added rhythmic modulation via an LFO that triggers an Envelope
- inflation is mapped to LFO frequency

### v01.22
- started to implement Neuron signal firing

### v01.22.1
- Neuron signaling works on one Dendrite each if it is connected

### v01.22.2
- signaling works accros the Dentrites of both connected Neurons
- there is a Signal path of Molecules stored that the signal can follow
- only mature Neurons will connect

### v01.23
- added a digesting function to the Intestines
- simply pushing the surrounding molecules into the perpendicular direction

### v01.23.2
- attempt for a more smooth digesting visualisation
- dependent on the distance to the food in the tunnel, the displacement is made 
- result was not significantly more smooth - but needs way more calculation

### v01.23.3
- continuation of v01.23 with the previous digestion approach
- redesign of the molecularSystem, having a global function to add organisms defined by a type and a number of amount
- the single add functions are adding only one instance at a specific point
- changed the enums, since there was a global conflict

### v01.23.4
- added digestionStatus function for retrieveing the status of the digestion
- mapped food size and color to the digestion status
- changed the Cell to start inflation always on a fresh sine cycle
- defined the condition for the Cell to die when it inflates to much

### v01.24
- added remove mechanism in the molecularSystem class
- added remove function for Breather Cells
- renamed molecules to allMolecules in molecularSystem class

### v01.24.2
- made Cells spawn several Neurons on death
- added remove mechanism also to the Pumpers and Intestines

### v01.24.3
- remove mechanism also for Neurons
- moved the inter-Neuron connections into the higher-ordered molecularSystem class

### v01.25
- GUI access for Organism growth + behavior (ongoing implementation)
- easy organism spawn through num keys

### v01.25.2
- Neurons die after amount of signals, the interval shortens towards end
- also Neurons are now shaking, when they fire a signal
- hacky implementation to compensate the phase shift that occurs if the inflation frequency is changed

### v01.25.3
- added cycle count to the Cells
- inflation frequency is mapped to the numbers of cycles relative to some max cycle value
- the cell explodes after a defined maximum cycles

### v01.26
- implemented multiple screens/vessels
- the canvas tune parameters for translation was moved into the signed distance function as well 

### v01.26.2
- added individual masks for each screen 

### v01.26.3
- moved Canvas Tune Gui into the molecularSystem calls - now every Screen has its own mask
- added anti-aliasing for FBOs

### v01.26.4
- replaced the lenght() calls in the code by squared length functions where possible (only 3. still left: 7)
- added screen size factor array for convenience

### v01.26.5
- added bools for the deletion routine to iterate only over the organisms that are to be deleted
- optimized more length() calls 

### v01.27
- divided the voice class into an extra class (instead of an embedded subclass of the synth class)
- made connection from organism to a voice via a Pointer to directly control its parameters 
- deleted all ui references from the voice class -> shall be controlled via the module inputs

### v01.27.2
- every cell has its own Voice instance -> no more synth class with a resizing vector
- the voices are summed in a masterBus in the molSystem, then summed in the ofApp for output

### v01.27.3
- made the synth voices globally controllable via the GUI

### v01.27.4
- voices are now audioModules
- created a Pumpers audioModule based on the sinePercussion by Nikola Pisanti
- split the Cell Class into a separate Pumpers class

### v01.27.5
- Pumpers Highlight color on impulse
- Pumper control impulse rate + die after impulse count
- global FX channel with Chorus+Delay on ofApp level

### vß1.27.6
- workaround for Pumpers delay problem
- new audioModule for Neurons

### v01.27.7
- audioModule for Intestines
- audio disconnect at death to avoid segmentation fault error

### v01.27.8
- more complex gravity and brownian motion on particles
- organisms are spawned at the top of the screen

### v01.28
- detune Breathers depending on SystemPressure (amount of molecules in vessel)


### v01.28.2
- individual sends for Chorus+Delay for each organism synth
- discontinued!

### v01.28.3
- rotation of canvas for simulation of upright TV
- retuned Breathers shape and breathing motion (own parameters)

### v01.28.4
- Breather inflation controlled by audioModule
- improvements and tweaks on the Breather behavior

### v01.28.5
- audio effects for System collapse
- added FX from nppatches to the SynthFX collection.

### v01.29
- implemented firefly algorithm to the Neurons

### v01.29.2
- sync algorithm for the pumpers (to half the phase)

### v01.29.3
- applied sync mechanism also between the pumpers and Neurons + Intestines
- first implementation of "delete me if im out of the screen"

### v01.30
- adjustment for 4 screen setup (multiMonitorFullScreen)
- spawing per mouse click

### v01.30.2
- added arousal + valence variables for Breather
- interaction causes arousal (faster + greater motion), organisms calms down to silence over time
- named Cell -> Breather class

### v01.30.3
- arousal mechanism for Pumper
- affects impulse rate, sound velocity and contraction intension
- organisms fall asleep after a while

### v01.30.4
- attempt for Breather sync algorithm
- Breathers should sync their frequency in harmonic relation to others independent of fixed frequency values
- not working satisfiable

### v01.30.5
- other Breather sync algorithm
- Breathers stear towards the most proximate frequency corresponding to a harmonic of a (fix) base frequency 
- (is a little bit cheating but the result is fine)

### v01.30.6
- added arousal mechanism for Neurons and Intestines
- both differ behavior according to arousal level and calm down after a while without interaction

### v01.31
- cleaned and tidied up version for Abgabe
- orientation is back to landscape orientattion

### v01.31.2
- merged v1.31 with the changes made on MacMini for the installation setup
- mainly a heads up display showing framerate and molecule number

### v01.31.3
- kinect re-implementation
- matched the calibration to a TV upright orientation

### v01.31.4
- implemented TV calibration and interaction for all 4 TVs
- mapping is done for each screen separately

### v01.31.5
- attempt to implement interaction through long term pointing
- did not find a solution

### v01.31.6
- interaction alternative: spawning on first pointing 
- next spawn only when all touchpoint have left the screen once
- fixed Segmentation fault bug during deletion, as Neurons where connecting to others that were about to be deleted in the same step