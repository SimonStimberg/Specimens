#include "molecularSystem.h"

//------------------------------------------------------------------
molecularSystem::molecularSystem() {
    

}


//------------------------------------------------------------------
void molecularSystem::setup(int width, int height) {

    worldSize.x = width;
    worldSize.y = height;

    flush = false;
    debugView = false;

    // intrusionPoints.push_back(glm::vec2(-100, 100));
}


//------------------------------------------------------------------
void molecularSystem::update() {

    // float xPos = 
    // float yPos = 
    // intrusionPoints[0].x = ofGetMouseX() - ofGetWidth() * 0.5;
    // intrusionPoints[0].y = ofGetMouseY() - ofGetHeight() * 0.5;

    // ofLogNotice("Mouse X: " + ofToString(ofGetMouseX()));

    for(unsigned int i = 0; i < breathers.size(); i++) {
		breathers[i]->update();
	}

    for(unsigned int i = 0; i < pumpers.size(); i++) {
		pumpers[i]->update();
	}

    for(unsigned int i = 0; i < liquid.size(); i++) {
		liquid[i]->update();
	}

    for(unsigned int i = 0; i < neurons.size(); i++) {
		neurons[i]->update();
	}

    for(unsigned int i = 0; i < intestines.size(); i++) {
		intestines[i]->update();
	}

    if(cleanUp) removeOrganisms();

}


//------------------------------------------------------------------
void molecularSystem::draw() {

    for(unsigned int i = 0; i < breathers.size(); i++){
		breathers[i]->draw();
	}
    for(unsigned int i = 0; i < pumpers.size(); i++){
		pumpers[i]->draw();
	}
    for(unsigned int i = 0; i < liquid.size(); i++){
		liquid[i]->draw();
	}
    for(unsigned int i = 0; i < neurons.size(); i++){
		neurons[i]->draw();
	}
    for(unsigned int i = 0; i < intestines.size(); i++){
		intestines[i]->draw();
	}
}


//------------------------------------------------------------------
void molecularSystem::removeOrganisms() {

    vector<Cell *>::iterator it = breathers.begin();
    for(; it != breathers.end();){
    	if( (*it)->isDead ){
    		delete *it;
     		it = breathers.erase(it);
    	}else
    		++it;
    }

    vector<Molecule *>::iterator itm = allMolecules.begin();
    for(; itm != allMolecules.end();){
    	if( (*itm)->removeMe ){
    		delete *itm;
     		itm = allMolecules.erase(itm);
    	}else
    		++itm;
    }

    vector<Spring *>::iterator its = allSprings.begin();
    for(; its != allSprings.end();){
    	if( (*its)->removeMe ){
    		delete *its;
     		its = allSprings.erase(its);
    	}else
    		++its;
    }

    cleanUp = false;

}


//------------------------------------------------------------------
vector<float> molecularSystem::getCellInflation() {

    vector<float> cellInflations;
    for(unsigned int i = 0; i < breathers.size(); i++){       
        cellInflations.push_back(breathers[i]->getInflation());
    }
    return cellInflations;
}

//------------------------------------------------------------------
vector<int> molecularSystem::getCellSizes() {

    vector<int> cellSizes;
    for(unsigned int i = 0; i < breathers.size(); i++){       
        cellSizes.push_back(breathers[i]->cellMolecules.size() );
    }
    return cellSizes;
}


//------------------------------------------------------------------
void molecularSystem::addLiquid(float x, float y) {

    Molecule * m = new Molecule(this);
    float velX = ofRandom(-0.9, 0.9);
    float velY = ofRandom(-0.9, 0.9);
    // m->reset(x, y, velX, velY);
    m->reset(x, y, 0.0, 0.0);

    liquid.push_back(m);
    
    allMolecules.push_back(m);

}


//------------------------------------------------------------------
void molecularSystem::addBreather(float x, float y) {

    // int amount = ofRandom(15, 31);
    // int amount = ofRandom(4, 9) * 2 + 1;   // 6, 15
    int amount = 8;


    Cell * c = new Cell(this, cellType::BREATHER);
    c->set(amount, x, y);

    breathers.push_back(c);

    // for(unsigned int j = 0; j < amount; j++){
    //     allMolecules.push_back(c->cellMolecules[j]);
    // }

}

//------------------------------------------------------------------
void molecularSystem::addPumper(float x, float y) {

    // int amount = ofRandom(15, 31);
    // int amount = ofRandom(4, 9) * 2 + 1;   // 6, 15
    int amount = 5;


    Cell * c = new Cell(this, cellType::PUMPER);
    c->set(amount, x, y);

    pumpers.push_back(c);

    for(unsigned int j = 0; j < amount; j++){
        allMolecules.push_back(c->cellMolecules[j]);
    }

}


//------------------------------------------------------------------
void molecularSystem::addNeuron(float x, float y) {


    int arms = 3;
    // int arms = floor(ofRandom(3, 6));
    int elements = 3;


    Neuron * n = new Neuron(this);
    n->set(arms, elements, x, y);

    neurons.push_back(n);


    for(unsigned int j = 0; j < n->neuronMolecules.size(); j++){
        allMolecules.push_back(n->neuronMolecules.at(j));
    }

}


//------------------------------------------------------------------
void molecularSystem::addIntestine(float x, float y) {

    int elements = 5;


    Intestine * n = new Intestine(this);
    n->set(elements, x, y);

    intestines.push_back(n);


    for(unsigned int j = 0; j < n->intestineMolecules.size(); j++){
        allMolecules.push_back(n->intestineMolecules.at(j));
    }

}


//------------------------------------------------------------------
void molecularSystem::addOrganisms(organismType type, int num) {

    if (num > 0) {

        for(unsigned int i = 0; i < num; i++){ 

            float x = ofRandom(-worldSize.x*0.4, worldSize.x*0.4);
            float y = ofRandom(-worldSize.y*0.4, worldSize.y*0.4);

            if (type == LIQUID)    addLiquid(x, y);
            if (type == BREATHER)  addBreather(x, y);
            if (type == PUMPER)    addPumper(x, y);
            if (type == NEURON)    addNeuron(x, y);
            if (type == INTESTINE) addIntestine(x, y);

        }
    }

}

//------------------------------------------------------------------
void molecularSystem::reset(bool random) {

    if (random) {

        // allMolecules.clear();

        // int liquidNum = liquid.size();
        // liquid.clear();
        // addLiquid(liquidNum);

        // int cellNum = cells.size();
        // cells.clear();
        // // addCells(cellNum);

        // int neuronNum = neurons.size();
        // neurons.clear();
        // addNeurons(neuronNum);

        // for(unsigned int i = 0; i < allMolecules.size(); i++){
            // liquid[i]->reset(ofRandom(100), ofRandom(100), ofRandom(-3.9, 3.9), ofRandom(-3.9, 3.9));
        // }

    } else {

        // int num = allMolecules.size();
        // allMolecules.clear();
        // springs.clear();

        // setup(num, worldSize.x, worldSize.y);

    }


}