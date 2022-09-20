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

    for(unsigned int i = 0; i < cells.size(); i++) {
		cells[i]->update();
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

}


//------------------------------------------------------------------
void molecularSystem::draw() {

    for(unsigned int i = 0; i < cells.size(); i++){
		cells[i]->draw();
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
vector<float> molecularSystem::getCellInflation() {

    vector<float> cellInflations;
    for(unsigned int i = 0; i < cells.size(); i++){       
        cellInflations.push_back(cells[i]->getInflation());
    }
    return cellInflations;
}

//------------------------------------------------------------------
vector<int> molecularSystem::getCellSizes() {

    vector<int> cellSizes;
    for(unsigned int i = 0; i < cells.size(); i++){       
        cellSizes.push_back(cells[i]->cellMolecules.size() );
    }
    return cellSizes;
}


//------------------------------------------------------------------
void molecularSystem::addMolecules(int num) {

    if (num > 0) {
  
        for(unsigned int i = 0; i < num; i++){
            int x = ofRandom(-worldSize.x*0.4, worldSize.x*0.4);
            int y = ofRandom(-worldSize.y*0.4, worldSize.y*0.4);
            // int x = ofRandom(-50, 50);
            // int y = ofRandom(-50, 50);

            Molecule * m = new Molecule(this);
            float velX = ofRandom(-0.9, 0.9);
	        float velY = ofRandom(-0.9, 0.9);
            // m->reset(x, y, velX, velY);
            m->reset(x, y, 0.0, 0.0);

            liquid.push_back(m);
            
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
            // int amount = ofRandom(15, 31);
            // int amount = ofRandom(4, 9) * 2 + 1;   // 6, 15
            int amount = 5;


            Cell * c = new Cell(this);
            c->set(amount, x, y);

            cells.push_back(c);

            for(unsigned int j = 0; j < amount; j++){
                molecules.push_back(c->cellMolecules[j]);
            }


        }
    }
}


//------------------------------------------------------------------
void molecularSystem::addNeurons(int num) {

    if (num > 0) {
  
        for(unsigned int i = 0; i < num; i++){
            int x = ofRandom(-worldSize.x*0.4, worldSize.x*0.4);
            int y = ofRandom(-worldSize.y*0.4, worldSize.y*0.4);
            int arms = 3;
            // int arms = floor(ofRandom(3, 6));
            int elements = 3;


            Neuron * n = new Neuron(this);
            n->set(arms, elements, x, y);

            neurons.push_back(n);


            for(unsigned int j = 0; j < n->neuronMolecules.size(); j++){
                molecules.push_back(n->neuronMolecules.at(j));
            }

        }
    }
}


//------------------------------------------------------------------
void molecularSystem::addIntestines(int num) {

    if (num > 0) {
  
        for(unsigned int i = 0; i < num; i++){
            int x = ofRandom(-worldSize.x*0.4, worldSize.x*0.4);
            int y = ofRandom(-worldSize.y*0.4, worldSize.y*0.4);
            // int arms = 3;
            // int arms = floor(ofRandom(3, 6));

            int elements = 5;


            Intestine * n = new Intestine(this);
            n->set(elements, x, y);

            intestines.push_back(n);


            for(unsigned int j = 0; j < n->intestineMolecules.size(); j++){
                molecules.push_back(n->intestineMolecules.at(j));
            }

        }
    }
}


//------------------------------------------------------------------
void molecularSystem::reset(bool random) {

    if (random) {

        molecules.clear();

        int liquidNum = liquid.size();
        liquid.clear();
        addMolecules(liquidNum);

        int cellNum = cells.size();
        cells.clear();
        addCells(cellNum);

        int neuronNum = neurons.size();
        neurons.clear();
        addNeurons(neuronNum);

        // for(unsigned int i = 0; i < molecules.size(); i++){
            // liquid[i]->reset(ofRandom(100), ofRandom(100), ofRandom(-3.9, 3.9), ofRandom(-3.9, 3.9));
        // }

    } else {

        // int num = molecules.size();
        // molecules.clear();
        // springs.clear();

        // setup(num, worldSize.x, worldSize.y);

    }


}