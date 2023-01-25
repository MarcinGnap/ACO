#pragma once

#include "TSP.h"
#include "Ant.h"

class ACO : public TSP {
	double alpha;
	double beta;    // this parameter raises the weight of distance over pheromone
	double evaporationRate; // evaporation rate
	float qval;
	int maxTours;
	int antsPopulation;
	std::vector<Ant*> ants;
	double** pheromones;
	double best;
	int bestIndex;
	int das;

	void init();
	void restartAnts();
	double antProduct(int from, int to);
	int selectNextCity(int ant);
	int simulateAnts();
	void updateTrails();
	void updateAntTrail(int curAnt);
	void updatePheromones();
public:
	ACO();

	void setNumberOfAnts(int antsPopulation);

	long long menu(double alfa, double betha, float q, std::string acos) override;
	double getBest() override;
};