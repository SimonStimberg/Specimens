#include "molecularSystem.h"

//------------------------------------------------------------------
molecularSystem::molecularSystem() {
    

}


//------------------------------------------------------------------
void molecularSystem::setup(int num) {

    molecules.assign(num, Molecule());

    for(unsigned int i = 0; i < molecules.size(); i++){
		molecules[i].reset(this);
	}	


//     for (int i = 0; i < num; i++) {
//         Molecule * m = new Molecule(this);
//         m->reset();
//         // need to be scattered or else no work
//         // b->setLoc(lx + ofRandom(-dev, dev),
//         //         ly + ofRandom(-dev, dev));
//         // b.setLoc(lx, ly);
// //			boids.add(b);
//         molecules.push_back(m);
//     }
}


//------------------------------------------------------------------
void molecularSystem::update() {

    for(unsigned int i = 0; i < molecules.size(); i++) {
		molecules[i].update();
	}

}


//------------------------------------------------------------------
void molecularSystem::draw() {

    for(unsigned int i = 0; i < molecules.size(); i++){
		molecules[i].draw();
	}
}
