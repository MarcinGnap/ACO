#include "Reader.h"
#include "timeMeasurement.h"
#include "Graph.h"
#include "ACO.h"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

int main() {
	Reader reader;
	fstream outputFile;

	outputFile.open("results.csv", ios::out);
	cout << "Starting...\n";

	for (int pliki = 0; pliki < sizeof(reader.iRNumber) / sizeof(int); pliki++) {

		double llAvgTime = 0;
		double llAvgTimefloat = 0;

		reader.OpenIni();
		cout << "\nNazwa pliku: " << reader.fileNames[pliki] << endl;
		cout << "Ilosc wykonan: " << reader.iRNumber[pliki] << endl;
		cout << "Optymalny koszt: " << reader.iOCost[pliki] << endl << endl;

		reader.OpenAll(reader.fileNames[pliki]);

		outputFile << "Nazwa pliku: " << reader.sFilename << endl;
		outputFile << "Powtórzenia algorytmu: " << reader.iRNumber[pliki] << endl;
		outputFile << "Optymalny koszt: " << reader.iOCost[pliki] << endl;

		cout << "\n \nChecking input file...\n";

		cout << "\nLoading Graph...\n";
		Graph* myGraph = new Graph(reader.iNOfVertices);
		myGraph->loadGraphFromFile(reader.iNOfVertices, reader.iVertices);

		//cout << "\n \nGraph output: \n";
		//myGraph->display();

		cout << "Calculating...\n";

		double llMinTime = INT_MAX;
		double llMaxTime = 0;

		double llTempTime = 0;

		vector<int> path;
		int cost;

		path.resize(myGraph->getNumbOfVerts() + 1);

		outputFile << "Otrzymany koszt: ;";
		outputFile << "Wspó³czynnik b³êdu[%]: ;";
		outputFile << "Czas wykonywania[s] ;";
		for (int y = 0; y < reader.iRNumber[pliki]; y++) {
			TSP* test = new ACO();
			test->load(myGraph);

			test->menu();

			//test->settingsSimulatedAnnealing(initTemp[init], minTemp[min]);
			//exeTime = test->algorithmSimulatedAnnealing(myGraph->getMatrix(), path, cost, initEra[z], reader.dA);

			//cout << "Time " << exeTime << " s\n";
			//cout << "Found solution " << test->getfoundOptimum() << endl << endl;
			//outputFile << test->getfoundOptimum() << " ; ";
			//outputFile << abs(errorRate - 100) << " ; ";
			//outputFile << exeTime << " ; " << endl;

			//llTempTime = exeTime;
			if (llTempTime < llMinTime) {
				llMinTime = llTempTime;
			}
			else if (llTempTime > llMaxTime) {
				llMaxTime = llTempTime;
			}
			llAvgTime = llAvgTime + llTempTime;
		}
		cout << "Optimal solution " << reader.iOCost[pliki] << endl;

		llAvgTimefloat = llAvgTime / reader.iRNumber[pliki];
		outputFile << "Œredni czas wykonywania algorytmu [s]: " << " ; " << llAvgTimefloat << endl;
		outputFile << "Optymalny koszt: " << " ; " << reader.iOCost[pliki] << endl << endl;
		llAvgTimefloat = 0;
		llAvgTime = 0;
		llTempTime = 0;

		cout << "Maksymalny czas wykonywania algorytmu: " << llMaxTime << endl;
		cout << "Minimalny czas wykonywania algorytmu: " << llMinTime << endl;
		cout << "Sredni czas wykonywania algorytmu: " << llAvgTimefloat << endl;

		cout << endl;
		cout << endl;

		cout << "Done...\n";
	}

	cout << "All done... \nGood luck... \n";
	getchar();
	return 0;
}