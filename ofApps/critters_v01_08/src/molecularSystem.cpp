#include "molecularSystem.h"

//------------------------------------------------------------------
molecularSystem::molecularSystem() {
    

}


//------------------------------------------------------------------
void molecularSystem::setup(int width, int height) {

    worldSize.x = width;
    worldSize.y = height;


    // for (int i = 0; i < num; i++) {
    //     float radius = 0.5*num;
    //     float angle = 360.0 / num * i;
    //     float x = radius * cos(ofDegToRad(angle));
    //     float y = radius * sin(ofDegToRad(angle));

    //     Molecule * m = new Molecule(this);
    //     m->reset(x, y);

    //     molecules.push_back(m);
    // }
    // for (int i = 0; i < num; i++) {
    //     Spring * s = new Spring(this);
        
    //     int j = (i < num-1) ? i+1 : 0;  // add the next - if reached the end: do the circle
    //     s->reset(molecules[i], molecules[j]);
        
    //     molecules[i]->addBonding(molecules[j]);
    //     molecules[j]->addBonding(molecules[i]);        

    //     springs.push_back(s);
    // }

    // Molecule * m = new Molecule(this);
    // m->reset(0, 0);

    // molecules.push_back(m);
}


//------------------------------------------------------------------
void molecularSystem::update() {

    // for(unsigned int i = 0; i < springs.size(); i++) {
	// 	springs[i]->update();
	// }

    for(unsigned int i = 0; i < cells.size(); i++) {
		cells[i]->update();
	}

    for(unsigned int i = 0; i < molecules.size(); i++) {
		liquid[i]->update();
	}

}


//------------------------------------------------------------------
void molecularSystem::draw() {

    for(unsigned int i = 0; i < cells.size(); i++){
		cells[i]->draw();
	}
    for(unsigned int i = 0; i < molecules.size(); i++){
		liquid[i]->draw();
	}
}


//------------------------------------------------------------------
void molecularSystem::addMolecules(int num) {

    if (num > 0) {
  
        for(unsigned int i = 0; i < num; i++){
            int x = ofRandom(-worldSize.x*0.5, worldSize.x*0.5);
            int y = ofRandom(-worldSize.y*0.5, worldSize.y*0.5);

            Molecule * m = new Molecule(this);
            float velX = ofRandom(-3.9, 3.9);
	        float velY = ofRandom(-3.9, 3.9);
            m->reset(x, y, velX, velY);

            liquid.push_back(m);
            // molecules.push_back(&liquid[liquid.size()-1]);
            molecules.push_back(m);
        }
    }
}


//------------------------------------------------------------------
void molecularSystem::addCells(int num) {

    if (num > 0) {
  
        for(unsigned int i = 0; i < num; i++){
            int x = ofRandom(-worldSize.x*0.4, worldSize.x*0.4);
            int y = ofRandom(-worldSize.y*0.4, worldSize.y*0.4);
            int amount = ofRandom(9, 21);


            Cell * c = new Cell(this);
            c->set(amount, x, y);

            cells.push_back(c);

            for(unsigned int i = 0; i < amount; i++){
                molecules.push_back(c->molecules[i]);
            }


        }
    }
}


//------------------------------------------------------------------
void molecularSystem::reset(bool random) {

    if (random) {

        for(unsigned int i = 0; i < molecules.size(); i++){
            // liquid[i]->reset(ofRandom(100), ofRandom(100), ofRandom(-3.9, 3.9), ofRandom(-3.9, 3.9));
        }

    } else {

        // int num = molecules.size();
        // molecules.clear();
        // springs.clear();

        // setup(num, worldSize.x, worldSize.y);

    }


}