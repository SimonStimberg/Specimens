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