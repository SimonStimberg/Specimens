#include "molecularSystem.h"

//------------------------------------------------------------------
molecularSystem::molecularSystem() {
    

}


//------------------------------------------------------------------
void molecularSystem::setup(int num, int width, int height) {

    worldSize.x = width;
    worldSize.y = height;


    for (int i = 0; i < num; i++) {
        float radius = 3.0*num;
        float angle = 360.0 / num * i;
        float x = radius * cos(ofDegToRad(angle));
        float y = radius * sin(ofDegToRad(angle));

        Molecule * m = new Molecule(this);
        m->reset(x, y);

        molecules.push_back(m);
    }
    for (int i = 0; i < num; i++) {
        Spring * s = new Spring(this);
        
        int j = (i < num-1) ? i+1 : 0;  // add the next - if reached the end: do the circle
        s->reset(molecules[i], molecules[j]);
        
        molecules[i]->addBonding(molecules[j]);
        molecules[j]->addBonding(molecules[i]);        

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
		molecules[i]->reset(0, 0);
	}
}