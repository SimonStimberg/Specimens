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