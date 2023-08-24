#include "molecularSystem.h"

//------------------------------------------------------------------
molecularSystem::molecularSystem() {
    
    // masterPtr = NULL;
}


//------------------------------------------------------------------
void molecularSystem::setup(int width, int height) {

    // masterPtr = ptr;

    worldSize.x = width;
    worldSize.y = height;

    // ofLogNotice("width: " + ofToString(width));
    // ofLogNotice("height: " + ofToString(height));

    flush = false;
    flushTimestamp = 0;
    debugView = false;
    std::fill_n (organismsToRemove, 5, false);
    thereAreCadavers = false;
    doNotDigest = false;
    freshlySpawned = false;
    collapseThreshold = floor(1300 * width/800.0);

    setGui();

    masterBus.set(1.0);
    blackhole.set(1.0);

    allMolecules.reserve(1500);       
    allSprings.reserve(1250);
    neuronConnections.reserve(75);

    liquid.reserve(500);
    breathers.reserve(50);
    pumpers.reserve(50);
    neurons.reserve(100);
    intestines.reserve(25);


    // initialize the bin structure
    k = 5;                      // "binPower" -> means that every bin is the size of 2^k   e.g.: k=5  ->  2^5=32
    int	binSize = 1 << k;		// << means "left shift operator" - this shifts the left value in bit representation by the number of digits defined by the right value
							    // this is equal to multiplying the left with 2^rightValue ->   1 << 4   =   1 * 2^5   =   1 * 32   ->  binSize = 32

	xBins = (int) ceilf((float) width / (float) binSize);
	yBins = (int) ceilf((float) height / (float) binSize);
	bins.resize(xBins * yBins);

    ofLogNotice(ofToString(bins.size()) + " bins created!");
    ofLogNotice("bin size: " + ofToString(binSize));



}


//------------------------------------------------------------------
void molecularSystem::update() {

    // ofLogNotice("updating mol system");

    updateBins();


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
    if(allMolecules.size() > collapseThreshold && !flush) {
        flush = true;    
        flushTimestamp = ofGetElapsedTimeMillis();
        ofLogNotice("timestamp: " + ofToString(flushTimestamp));
    }

    // if the system has collapsed (flush is true) and all molecules have been deleted from the system: restart!
    if(flush && allMolecules.size() <= 0) {
        flush = false;
        addOrganisms(LIQUID, floor(250 * worldSize.x/800.0));
    }

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
void molecularSystem::addRandom(float x, float y) {

    // ofLogNotice("adding new random organisms");

    // check if spawning position is within the vessel
    Molecule * m = new Molecule(this);
    m->reset(0,0,0,0);
    float sdf = m->signedDistanceField(glm::vec2(x, y));


    if(sdf <= -20.0 ) {     // -20 -> keep a border of 20px for safety

        int amount = 3;

        for(unsigned int i = 0; i < amount; i++){ 

            x += ofRandom(-15., 15.);
            y += ofRandom(-15., 15.);

            float probability[4] = {0.2, 0.4, 0.9, 1.0};   // the probability for the different organism types      Breather: 0.2
            float dice = ofRandom(1.);

            if (dice < probability[0]) {
                addBreather(x, y);
            } else if (dice < probability[1]) {
                addPumper(x, y);
            } else if (dice < probability[2]) {
                addNeuron(x, y);
            } else {
                addIntestine(x, y);
            }

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

    // ofLogNotice("before adding breather");


    int amount = 8;     // amount of Molecules the shape will consist of

    Breather * c = new Breather(this, cellType::BREATHER);
    c->set(amount, x, y);

    c->audioModule >> masterBus.ch(0);          // route the audio of the organism to the corresponding stem of that orgainsm type
    // c->audioModule.out_void() >> blackhole; 

    breathers.push_back(c);

    // ofLogNotice("after adding breather");

}


//------------------------------------------------------------------
void molecularSystem::addPumper(float x, float y) {

    int amount = 5;

    Pumper * c = new Pumper(this);
    c->set(amount, x, y);

    c->audioModule >> masterBus.ch(1);
    // c->audioModule.out_void() >> blackhole;

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



// CLEARS AND REASSIGNS THE MOLECULES TO THE BINS
//------------------------------------------------------------------
void molecularSystem::updateBins() {			
	
	// clear all bins
	for(int i = 0; i < bins.size(); i++) {
		bins[i].clear();
	}

	// iterate over all molecules
	// and assign them to the bins based on their position
	unsigned xBin, yBin, bin;
	for(int i = 0; i < allMolecules.size(); i++) {
		Molecule * m = allMolecules[i];
        float x = m->position.x + worldSize.x*0.5;
        float y = m->position.y + worldSize.y*0.5;
		xBin = ((unsigned) x) >> k;		// a fancy (and maybe more performant) way to write  (int)floor(position.x / binSize)		
		yBin = ((unsigned) y) >> k;		// >> "right shift operator" is the equivalent to dividing the left value by 2^rightValue
											// ->   a >> b   =  a/(2^b)
											// value will be "floored" -> like converting a resulting float to int, everything after the comma will be dismissed
		
		bin = yBin * xBins + xBin;				// get the position in the array
		if(xBin < xBins && yBin < yBins)		// just make sure the calculated bin position is within the bounds of the existing array
			bins[bin].push_back(m);
	}

}



// REMOVE DEAD ORGANISMS
//------------------------------------------------------------------
void molecularSystem::cleanUp() {

    // ofLogNotice("Deletion process begin");

    // masterBus.disconnectAll();
    // blackhole.disconnectAll();


    if(organismsToRemove[BREATHER]) {
        vector<Breather *>::iterator itB = breathers.begin();
        for(; itB != breathers.end();){
            if( (*itB)->isDead ){
                delete *itB;
                itB = breathers.erase(itB);
                // ofLogNotice("Breather deleted!");
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
                // ofLogNotice("Pumper deleted!");
            } else
                ++itP;
        }
    }


    if(organismsToRemove[NEURON]) {

        // remove inter-Neuron connections first and disconnect them, so we can safely delete the Neuron
        // using the iterator instead of looping through the index has the great advantage that it does not get messed up once elemets are deleted (otherwise the index changes as elemets are missing resulting that elements are skipped)
        vector<Spring *>::iterator itC = neuronConnections.begin();
        for(; itC != neuronConnections.end();){
            if( (*itC)->removeMe || (*itC)->moleculeA->removeMe || (*itC)->moleculeB->removeMe ){
                
                (*itC)->removeMe = true;
    
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
                // ofLogNotice("Neuron deleted!");
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
                // ofLogNotice("Intestine deleted!");
            } else
                ++itI;
        }
    }


    if(organismsToRemove[LIQUID]) {
        vector<Molecule *>::iterator itL = liquid.begin();
        for(; itL != liquid.end();){
            if( (*itL)->removeMe ){
                itL = liquid.erase(itL);
                // ofLogNotice("Liquid deleted!");
            } else
                ++itL;
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

    if(!organismsToRemove[LIQUID]) {
        // could be OPTIMIZED if the spring removal is happening for each organism directly - but this way is more coherent
        vector<Spring *>::iterator itS = allSprings.begin();
        for(; itS != allSprings.end();){
            if( (*itS)->removeMe ){
                delete *itS;
                itS = allSprings.erase(itS);
            } else
                ++itS;
        }
    }

    // organismsToRemove = {false, false, false, false, false};
    std::fill_n (organismsToRemove, 5, false);
    thereAreCadavers = false;


    // ofLogNotice("Deletion complete.");
    // ofLogNotice("Deletion complete. now reconnecting audio");

    // for(unsigned int i = 0; i < breathers.size(); i++){ 

    //     breathers[i]->audioModule >> masterBus.ch(0);  
    //     breathers[i]->audioModule.out_void() >> blackhole; 

    // }

    // ofLogNotice("audio reconnection done");

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
    // ofLogNotice("getting system pressure");

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