#include "molecularSystem.h"

//------------------------------------------------------------------
molecularSystem::molecularSystem() {
    

}


//------------------------------------------------------------------
void molecularSystem::setup(int num) {

    for (int i = 0; i < num; i++) {
        Molecule * m = new Molecule(this);
        m->reset();

        molecules.push_back(m);
    }
}


//------------------------------------------------------------------
void molecularSystem::update() {

    for(unsigned int i = 0; i < molecules.size(); i++) {
		molecules[i]->update();
	}

}


//------------------------------------------------------------------
void molecularSystem::draw() {

    for(unsigned int i = 0; i < molecules.size(); i++){
		molecules[i]->draw();
	}
}


//------------------------------------------------------------------
void molecularSystem::reset() {

    for(unsigned int i = 0; i < molecules.size(); i++){
		molecules[i]->reset();
	}
}