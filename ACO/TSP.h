#pragma once

#include <iostream>
#include <sstream>
#include <cmath>
#include <fstream>
#include <vector>
#include "Graph.h"

class TSP {
public:
	enum crossover {
		MPXI = 0,
		PMX = 1
	};

	enum mutation {
		SWAP = 0,
		SCRAMBLE = 1
	};

	virtual ~TSP() {};

	bool searchInFile(std::ifstream &fin, std::string toFind);
	bool load(Graph* myGraph);
	virtual void menu() = 0;

protected:
	Graph* gm;
	int numberOfCities;
};