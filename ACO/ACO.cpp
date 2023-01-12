#include "ACO.h"
#include "Timer.h"
#include "timeMeasurement.h"
#include <assert.h>

// runtime Structures and global variables
ACO::ACO()
{
	alpha = 1.0;
	beta = 5.0;
	qval = 100;
	maxTours = 100;
	antsPopulation = 50;
	evaporationRate = 0.5;
	best = INT_MAX;
}

// function init() - initializes the entire graph
void ACO::init()
{
	pheromones = new double*[gm->getNumbOfVerts()];
	for (int i = 0; i < gm->getNumbOfVerts(); i++)
	{
		pheromones[i] = new double[gm->getNumbOfVerts()];
		for (int j = 0; j < gm->getNumbOfVerts(); j++)
		{
			pheromones[i][j] = 1.0 / gm->getNumbOfVerts();
		}
	}

	// initializing the ants
	for (int i = 0, to = 0; i < antsPopulation; i++)
	{
		if (to == gm->getNumbOfVerts())
			to = 0;

		Ant *ant = new Ant(gm->getNumbOfVerts());
		ant->curCity = to++;

		for (int from = 0; from < gm->getNumbOfVerts(); from++)
		{
			ant->tabu[from] = 0;
			ant->path[from] = -1;
		}

		ant->pathIndex = 1;
		ant->path[0] = ant->curCity;
		ant->nextCity = -1;
		ant->tourLength = 0;

		// loading first city into tabu list
		ant->tabu[ant->curCity] = 1;
		ants.push_back(ant);
	}
}

// reinitialize all ants and redistribute them
void ACO::restartAnts()
{
	for (int ant = 0, to = 0; ant < antsPopulation; ant++)
	{
		if (ants[ant]->tourLength < best)
		{
			best = ants[ant]->tourLength;
			bestIndex = ant;
		}

		ants[ant]->nextCity = -1;
		ants[ant]->tourLength = 0;

		for (int i = 0; i < gm->getNumbOfVerts(); i++)
		{
			ants[ant]->tabu[i] = 0;
			ants[ant]->path[i] = -1;
		}

		if (to == gm->getNumbOfVerts())
			to = 0;

		ants[ant]->curCity = to++;

		ants[ant]->pathIndex = 1;
		ants[ant]->path[0] = ants[ant]->curCity;

		ants[ant]->tabu[ants[ant]->curCity] = 1;
	}
}

double ACO::antProduct(int from, int to)
{
	return pow(pheromones[from][to], alpha) * pow(1.0 / gm->getValueOfEdge(from, to), beta);
}

int ACO::selectNextCity(int ant)
{
	int from, to;
	double denom = 0.0;

	from = ants[ant]->curCity;

	for (to = 0; to < gm->getNumbOfVerts(); to++)
	{
		if (ants[ant]->tabu[to] == 0)
		{
			denom += antProduct(from, to);
		}
	}

	assert(denom != 0.0);

	do
	{
		double p;
		to++;

		if (to >= gm->getNumbOfVerts())
			to = 0;
		if (ants[ant]->tabu[to] == 0)
		{
			p = antProduct(from, to) / denom;

			double x = (double)rand() / RAND_MAX;
			if (x < p)
			{
				break;
			}
		}
	} while (true);

	return to;
}

int ACO::simulateAnts()
{
	int k;
	int moving = 0;

	for (k = 0; k < antsPopulation; k++)
	{
		// checking if there are any more cities to visit
		if (ants[k]->pathIndex < gm->getNumbOfVerts())
		{
			ants[k]->nextCity = selectNextCity(k);
			ants[k]->tabu[ants[k]->nextCity] = 1;
			ants[k]->path[ants[k]->pathIndex++] = ants[k]->nextCity;

			ants[k]->tourLength += gm->getValueOfEdge(ants[k]->curCity, ants[k]->nextCity);
			
			//handle last case->last city to first
			if (ants[k]->pathIndex == gm->getNumbOfVerts())
			{
				ants[k]->tourLength += gm->getValueOfEdge(ants[k]->path[gm->getNumbOfVerts() - 1], ants[k]->path[0]);
			}
			updateAntTrail(k);

			//pheromones[ants[k]->curCity][ants[k]->nextCity] += qval;
			//pheromones[ants[k]->nextCity][ants[k]->curCity] = pheromones[ants[k]->curCity][ants[k]->nextCity];

			ants[k]->curCity = ants[k]->nextCity;
			moving++;
		}
	}
	//updatePheromones();
	return moving;
}

void ACO::updatePheromones() {
	int from, to;

	for (from = 0; from < gm->getNumbOfVerts(); from++)
	{
		for (to = 0; to < gm->getNumbOfVerts(); to++)
		{
			pheromones[from][to] *= evaporationRate;

			if (pheromones[from][to] < 0.0)
			{
				pheromones[from][to] = 1.0 / gm->getNumbOfVerts();
			}
		}
	}
}


void ACO::updateAntTrail(int curAnt)
{
	// CAS
	//pheromones[fromAnt][toAnt] += qval / ants[curAnt]->tourLength;
	//pheromones[toAnt][fromAnt] = pheromones[fromAnt][toAnt];

	// DAS
	pheromones[ants[curAnt]->curCity][ants[curAnt]->nextCity] += qval;
	pheromones[ants[curAnt]->nextCity][ants[curAnt]->curCity] = pheromones[ants[curAnt]->curCity][ants[curAnt]->nextCity];

	// QAS
	//pheromones[fromAnt][toAnt] += qval / gm->getValueOfEdge(ants[curAnt]->curCity, ants[curAnt]->nextCity);
	//pheromones[toAnt][fromAnt] = pheromones[fromAnt][toAnt];
}


// updating trails
void ACO::updateTrails(int fromAnt, int toAnt)
{
	int from, to;
	// pheromones evaporation
	for (from = 0; from < gm->getNumbOfVerts(); from++)
	{
		for (to = 0; to < gm->getNumbOfVerts(); to++)
		{
			if (from != to)
			{
				pheromones[from][to] *= (1.0 - evaporationRate);

				if (pheromones[from][to] < 0.0)
				{
					pheromones[from][to] = 1.0 / gm->getNumbOfVerts();
				}
			}
		}
	}
}

long long ACO::menu()
{
	antsPopulation = gm->getNumbOfVerts();
	int curTime = 0;

	timeMeasurement* tM = new timeMeasurement();

	std::cout << "S-ACO:";
	std::cout << "MaxTime=" << maxTours * gm->getNumbOfVerts();

	srand(static_cast<unsigned int>(time(nullptr)));

	auto o1 = chrono::high_resolution_clock::now();

	init();

	while (curTime++ < maxTours * gm->getNumbOfVerts())
	{
		if (simulateAnts() == 0)
		{
			updateTrails(0, 0);
			if (curTime != maxTours * gm->getNumbOfVerts())
				restartAnts();

			std::cout << "\n Time is " << curTime << "(" << best << ")";
		}
	}
	auto o2 = chrono::high_resolution_clock::now();

	std::cout << "\nBest tour = " << best << std::endl << std::endl << std::endl;
	return tM->tMTest(o1, o2);
}

double ACO::getBest()
{
	return best;
}

void ACO::setNumberOfAnts(int antsPopulation) {
	ACO::antsPopulation = antsPopulation;
}

/* DAS, u góry jest CAS, QAS to przez aktualn¹ wagê krawêdzi
for (int ant = 0; ant < antsPopulation; ant++)
{
	for (int i = 0; i < gm->getNumbOfVerts(); i++)
	{
		if (i < gm->getNumbOfVerts() - 1)
		{
			from = ants[ant]->path[i];
			to = ants[ant]->path[i + 1];
		}
		else
		{
			from = ants[ant]->path[i];
			to = ants[ant]->path[0];
		}

		pheromones[from][to] += qval;
		pheromones[to][from] = pheromones[from][to];

	}
}*/