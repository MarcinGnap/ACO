#include "Graph.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

void Graph::printGraph()
{
	std::cout << "  ";
	for (int i = 0; i < numbOfVerts; i++) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < numbOfVerts; i++) {
		std::cout << i << " ";
		for (int j = 0; j < numbOfVerts; j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

Graph::Graph(int numberOfVerticles)
{
	updateGraph(numberOfVerticles);
}

Graph::~Graph()
{
	for (int i = 0; i < numbOfVerts; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

void Graph::createRandomGraph()
{
	//lastTime = (lastTime + 1) % (1<<30);

	int maxValue = 100;
	srand(GetTickCount64());// samo srnad() nie wystarczalo aby szybko generowac inne wartosci dla malych problemow time(NULL)

	for (int i = 0; i < numbOfVerts; i++) {
		for (int j = 0; j < numbOfVerts; j++) {
			matrix[i][j] = rand() % maxValue;
		}
	}

}

void Graph::createRandomGraph(int addSmth)
{
	int maxValue = 100;
	srand(GetTickCount64() + addSmth);// samo srnad() nie wystarczalo aby szybko generowac inne wartosci dla malych problemow time(NULL)

	for (int i = 0; i < numbOfVerts; i++) {
		for (int j = 0; j < numbOfVerts; j++) {
			matrix[i][j] = rand() % maxValue;
		}
	}
}

void Graph::loadGraphFromFile(int iVerNum, int **iVerMatrix)
{
	numbOfVerts = iVerNum;
	for (int i = 0; i < numbOfVerts; i++) {
		for (int j = 0; j < numbOfVerts; j++) {
			matrix[i][j] = iVerMatrix[i][j];
		}
	}
	/*std::string fileName;
	std::cout << "Podaj filename : ";
	std::cin >> fileName;
	std::fstream file;
	file.open(fileName, std::ios::in);
	if (file.good() == true) {
		int a;
		if (file >> a)
		{
			//printf("%d ", a);
		}
		updateGraph(a);
		for (int i = 0; i < numbOfVerts; i++) {
			for (int j = 0; j < numbOfVerts; j++) {
				file >> matrix[i][j];
			}

		}

	}*/
}

/*void Graph::saveToFile()
{
	string fileName;
	cout << "Podaj nazwe pliku do ktorego chcesz zpaisac graf: ";
	cin >> fileName;
	std::fstream graphOutput;
	graphOutput.open(fileName, ios::out);	//  | ios::app
	if (graphOutput.good() == true) {
		graphOutput << numbOfVerts << endl;
		for (int i = 0; i < numbOfVerts; i++) {
			for (int j = 0; j < numbOfVerts; j++) {
				graphOutput << matrix[i][j] << " ";
			}
			graphOutput << endl;

		}
		graphOutput.close();
	}
}*/

int Graph::getValueOfEdge(int startPoint, int endPoint)
{
	return matrix[startPoint][endPoint];
}

void Graph::setValueOfEdge(int startPoint, int endPoint, int vlaueToSet)
{
	matrix[startPoint][endPoint] = vlaueToSet;
}

int Graph::getNumbOfVerts()
{
	return numbOfVerts;
}

void Graph::updateGraph(int newSize)
{
	if (newSize != numbOfVerts) {
		this->setNumbOfVerts(newSize);
		matrix = new int*[numbOfVerts];
		for (int i = 0; i < numbOfVerts; i++) {
			matrix[i] = new int[numbOfVerts];	//wiersze
		}
		bool fillWithZeros = true;
		if (fillWithZeros) {
			for (int i = 0; i < numbOfVerts; i++) {
				for (int j = 0; j < numbOfVerts; j++) {
					matrix[i][j] = 0;
				}
			}
		}
	}
}

void Graph::setNumbOfVerts(int number)
{
	numbOfVerts = number;
}