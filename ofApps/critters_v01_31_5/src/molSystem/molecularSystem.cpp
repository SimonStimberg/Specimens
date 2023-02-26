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
    std::fill_n (organismsToRemove, 5, false);
    thereAreCadavers = false;
    doNotDigest = false;
    collapseThreshold = 900;

    setGui();

    masterBus.set(1.0);
    blackhole.set(1.0);

}


//------------------------------------------------------------------
void molecularSystem::update() {


    // update all organisms in the Molecular System

    for(unsigned int i = 0; i < liquid.size(); i++) {
		liquid[i]->update();
	}

    for(unsigned int i = 0; i < breathers.size(); i++) {
		breathers[i]->update();
	}

    for(unsigned int i = 0; i < pumpers.size(); i++) {
		pumpers[i]->update();
	}

    for(unsigned int i = 0; i < neuronConnections.size(); i++) {
		neuronConnections[i]->update();
	}
    for(unsigned int i = 0; i < neurons.size(); i++) {
		neurons[i]->update();
	}

    for(unsigned int i = 0; i < intestines.size(); i++) {
		intestines[i]->update();
	}
 
    // remove dead organisms
    if(thereAreCadavers) cleanUp();     

    // if the amount of Molecules surpasses a certain threshold the system collapses: the contents will be flushed though a whole in the ground
    if(allMolecules.size() > collapseThreshold) flush = true;   

}


//------------------------------------------------------------------
void molecularSystem::draw() {

    for(unsigned int i = 0; i < liquid.size(); i++){
		liquid[i]->draw();
	}
    for(unsigned int i = 0; i < breathers.size(); i++){
		breathers[i]->draw();
	}
    for(unsigned int i = 0; i < pumpers.size(); i++){
		pumpers[i]->draw();
	}
    for(unsigned int i = 0; i < neurons.size(); i++){
		neurons[i]->draw();
	}
    for(unsigned int i = 0; i < intestines.size(); i++){
		intestines[i]->draw();
	}

    // ofSetColor(ofColor::indianRed);
    // ofFill();
    // if (intrusionPoints.size() > 0) {
    //     for(unsigned int i = 0; i < intrusionPoints.size(); i++){
    //         ofDrawCircle(intrusionPoints[i], 10.);
    //     }
    // }

}



//------------------------------------------------------------------
void molecularSystem::addOrganisms(organismType type, int num) {

    if (num > 0) {

        for(unsigned int i = 0; i < num; i++){ 


            float x = 0.;
            float y = 0.;
            if(type == LIQUID) {
                x = -worldSize.x *0.5 - ofRandom(1000.);
                y = ofRandom(-10., 10.);
            } else {
                x = -worldSize.x *0.5 - 50 - ofRandom(10.);
                y = ofRandom(-10., 10.);
            }


            if (type == LIQUID)    addLiquid(x, y);
            if (type == BREATHER)  addBreather(x, y);
            if (type == PUMPER)    addPumper(x, y);
            if (type == NEURON)    addNeuron(x, y);
            if (type == INTESTINE) addIntestine(x, y);

        }
    }

}


//------------------------------------------------------------------
void molecularSystem::addLiquid(float x, float y) {

    Molecule * m = new Molecule(this);

    float velX = ofRandom(-1.0, 1.0);
    float velY = ofRandom(-1.0, 1.0);

    m->reset(x, y, 0.0, 0.0);
    liquid.push_back(m);
    
    allMolecules.push_back(m);

}


//------------------------------------------------------------------
void molecularSystem::addBreather(float x, float y) {


    int amount = 8;     // amount of Molecules the shape will consist of

    Breather * c = new Breather(this, cellType::BREATHER);
    c->set(amount, x, y);

    c->audioModule >> masterBus.ch(0);          // route the audio of the organism to the corresponding stem of that orgainsm type
    c->audioModule.out_void() >> blackhole; 

    breathers.push_back(c);

}


//------------------------------------------------------------------
void molecularSystem::addPumper(float x, float y) {

    int amount = 5;

    Pumper * c = new Pumper(this);
    c->set(amount, x, y);

    c->audioModule >> masterBus.ch(1);
    c->audioModule.out_void() >> blackhole;

    pumpers.push_back(c);

}


//------------------------------------------------------------------
void molecularSystem::addNeuron(float x, float y) {

    int arms = 3;
    int elements = 3;

    Neuron * n = new Neuron(this);
    n->set(arms, elements, x, y);

    n->audioModule >> masterBus.ch(2);

    neurons.push_back(n);

}


//------------------------------------------------------------------
void molecularSystem::addIntestine(float x, float y) {

    int elements = 5;


    Intestine * n = new Intestine(this);
    n->set(elements, x, y);

    n->audioModule >> masterBus.ch(3);

    intestines.push_back(n);

}



// REMOVE DEAD ORGANISMS
//------------------------------------------------------------------
void molecularSystem::cleanUp() {


    if(organismsToRemove[BREATHER]) {
        vector<Breather *>::iterator itB = breathers.begin();
        for(; itB != breathers.end();){
            if( (*itB)->isDead ){
                delete *itB;
                itB = breathers.erase(itB);
            } else
                ++itB;
        }
    }


    if(organismsToRemove[PUMPER]) {
        vector<Pumper *>::iterator itP = pumpers.begin();
        for(; itP != pumpers.end();){
            if( (*itP)->isDead ){
                delete *itP;
                itP = pumpers.erase(itP);
            } else
                ++itP;
        }
    }


    if(organismsToRemove[NEURON]) {

        // remove inter-Neuron connections first and disconnect them, so we can safely delete the Neuron
        // using the iterator instead of looping through the index has the great advantage that it does not get messed up once elemets are deleted (otherwise the index changes as elemets are missing resulting that elements are skipped)
        vector<Spring *>::iterator itC = neuronConnections.begin();
        for(; itC != neuronConnections.end();){
            if( (*itC)->removeMe ){

                // as every Neuron that is connected has a copy of the connection pointer we have to remove those from theiry connection-array first
                (*itC)->moleculeA->neuronPtr->removeDeadConnections();
                (*itC)->moleculeB->neuronPtr->removeDeadConnections();
                (*itC)->disconnect();   // then disconnect the two Molecules that were bound by that connection

                // now we can safely remove the connection itself
                delete *itC;
                itC = neuronConnections.erase(itC);

            } else
                ++itC;
        }

        // now we can safely remove all tagged Neurons
        vector<Neuron *>::iterator itN = neurons.begin();
        for(; itN != neurons.end();){
            if( (*itN)->isDead ){
                delete *itN;
                itN = neurons.erase(itN);
            } else {
                ++itN;
            }
        }

    }


    if(organismsToRemove[INTESTINE]) {
        vector<Intestine *>::iterator itI = intestines.begin();
        for(; itI != intestines.end();){
            if( (*itI)->isDead ){
                delete *itI;
                itI = intestines.erase(itI);
            } else
                ++itI;
        }
    }


    vector<Molecule *>::iterator itM = allMolecules.begin();
    for(; itM != allMolecules.end();){
    	if( (*itM)->removeMe ){
    		delete *itM;
     		itM = allMolecules.erase(itM);
    	} else
    		++itM;
    }

    // could be OPTIMIZED if the spring removal is happening for each organism directly - but this way is more coherent
    vector<Spring *>::iterator itS = allSprings.begin();
    for(; itS != allSprings.end();){
    	if( (*itS)->removeMe ){
    		delete *itS;
     		itS = allSprings.erase(itS);
    	} else
    		++itS;
    }

    // organismsToRemove = {false, false, false, false, false};
    std::fill_n (organismsToRemove, 5, false);
    thereAreCadavers = false;

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
float molecularSystem::getSystemPressure() 
{
    float pressure = allMolecules.size() / (float)collapseThreshold;
    pressure *= pressure;
    return pressure; 
}


//------------------------------------------------------------------
void molecularSystem::reset(bool random) {

}


//------------------------------------------------------------------
void molecularSystem::setGui() {

    gui.add(tuneCanvasWidth.set("Canvas Width", 330, 140, 360));
    gui.add(tuneCanvasHeight.set("Canvas Height", 240, 140, 288));
    gui.add(tuneVerticalBow.set("Vertical Bow", 760, 600, 2000));
    gui.add(tuneHorizontalBow.set("Horizontal Bow", 740, 600, 2000));
    gui.add(tuneEdges.set("Smooth Edges", 25, 0, 60));
    gui.add(tuneXpos.set("X Position", 0.0, -50.0, 50.0));
    gui.add(tuneYpos.set("Y Position", 0.0, -50.0, 50.0));
    gui.add(tuneRotation.set("Rotation", 0.0, -15.0, 15.0));

}