#include "molecularSystem.h"
#include "ofApp.h"

//------------------------------------------------------------------
Cell::Cell() {
    systemPtr = NULL;
}

Cell::Cell(molecularSystem * system) {

	Cell();
	this->systemPtr = system;
	
}


void Cell::set(int num, int x, int y) {

	for (int i = 0; i < num; i++) {
        float radius = 1.5*num;
        float angle = 360.0 / num * i;
        float xPos = x + radius * cos(ofDegToRad(angle));
        float yPos = y + radius * sin(ofDegToRad(angle));

        Molecule * m = new Molecule(systemPtr, this);
        m->reset(xPos, yPos, 0., 0.);

        cellMolecules.push_back(m);
    }

    for (int i = 0; i < num; i++) {
        Spring * s = new Spring(systemPtr);
        
        int j = (i < num-1) ? i+1 : 0;  // add the next - if reached the end: do the circle
        s->reset(cellMolecules[i], cellMolecules[j]);
        
        cellMolecules[i]->addBonding(cellMolecules[j]);
        cellMolecules[j]->addBonding(cellMolecules[i]);        

        springs.push_back(s);
    }
	// this->systemPtr = system;
	
}



//------------------------------------------------------------------
void Cell::update() {


	for(unsigned int i = 0; i < springs.size(); i++) {
		springs[i]->update();
	}

    for(unsigned int i = 0; i < cellMolecules.size(); i++) {
		cellMolecules[i]->update();
	}
	
}


//------------------------------------------------------------------
void Cell::draw() {

    for(unsigned int i = 0; i < springs.size(); i++){
		springs[i]->draw();
	}
    for(unsigned int i = 0; i < cellMolecules.size(); i++){
		cellMolecules[i]->draw();
	}

}


