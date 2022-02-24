#include "molecularSystem.h"

//------------------------------------------------------------------
molecularSystem::molecularSystem() {
    

}


//------------------------------------------------------------------
void molecularSystem::setup(int num, int width, int height) {

    worldSize.x = width;
    worldSize.y = height;

    for (int i = 0; i < num; i++) {
        Molecule * m = new Molecule(this);
        m->reset();

        molecules.push_back(m);
    }
    for (int i = 0; i < num -1; i++) {
        Spring * s = new Spring(this);
        
        // do the circle
        if (i < num-1) {
            s->reset(molecules[i], molecules[i+1]);
        } else {
            s->reset(molecules[i], molecules[0]);
        }
        

        springs.push_back(s);
    }
}


//------------------------------------------------------------------
void molecularSystem::update() {

    for(unsigned int i = 0; i < springs.size(); i++) {
		springs[i]->update();
	}

    for(unsigned int i = 0; i < molecules.size(); i++) {
		molecules[i]->update();
	}

}


//------------------------------------------------------------------
void molecularSystem::draw() {

    for(unsigned int i = 0; i < springs.size(); i++){
		springs[i]->draw();
	}
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