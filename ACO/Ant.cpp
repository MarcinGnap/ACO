#include "Ant.h"

Ant::Ant(int numberOfCities) {
	tabu = new int[numberOfCities];
	path = new int[numberOfCities];
}

Ant::~Ant() {
	delete[]tabu;
	delete[]path;
}