#pragma once

#include "TSP.h"
#include "Ant.h"

class ACO : public TSP {
	double alpha;
	double beta;    // this parameter raises the weight of distance over pheromone
	double evaporationRate; // evaporation rate
	int qval;
	int maxTours;
	int antsPopulation;
	std::vector<Ant*> ants;
	double** pheromones;
	double best;
	int bestIndex;

	void init();
	void restartAnts();
	double antProduct(int from, int to);
	int selectNextCity(int ant);
	int simulateAnts();
	void updateTrails();
	void updatePheromones(int from, int to);
public:
	ACO();

	void setNumberOfAnts(int antsPopulation);

	long long menu() override;
	double getBest() override;
};