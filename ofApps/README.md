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
  - found it less confusion and error prone but in the end it turned out not to make any difference, hence the pointer version might be the better choise to avoid memory intense copying of instances
    