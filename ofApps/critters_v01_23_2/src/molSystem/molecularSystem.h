#pragma once
#include "ofMain.h"
#include "Molecule.h"
#include "Spring.h"
#include "organisms.h"

// enum class cellType;

class molecularSystem{

	public:

        

		molecularSystem();
        void setup(int width, int height);
        void update();
        void draw();

        vector <float> getCellInflation();
        vector <int> getCellSizes();

        void addLiquid(int num);
        void addBreathers(int num);
        void addPumpers(int num);
        void addNeurons(int num);
        void addIntestines(int num);
        void reset(bool random);

        void setIntrusionPoints(vector <glm::vec2> pts) { intrusionPoints = pts; }


        vector <Molecule *> molecules;
        vector <Molecule *> liquid;
        // vector <Spring *> springs;
        vector <Cell *> breathers;
        vector <Cell *> pumpers;
        vector <Neuron *> neurons;
        vector <Intestine *> intestines;

        vector <glm::vec2> intrusionPoints;

        glm::vec2 worldSize;

        bool flush;
        bool debugView;


    private:

};


