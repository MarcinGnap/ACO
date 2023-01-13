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
		double avrageErrorRate = 0;
		double avrageError = 0;

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

		long long llMinTime = INT_MAX;
		long long llMaxTime = 0;
		double llMinErrorRate = INT_MAX;
		double llMaxErrorRate = 0;
		double llMinError = INT_MAX;
		double llMaxError = 0;

		long long llTempTime = 0;
		long long exeTime;

		float errorRate;
		float error;
		vector<int> path;
		int cost;

		vector<double> alfa = { 0.1, 0.5, 1, 2, 3, 5 };
		vector<double> beta = { 1, 2, 3, 4, 5, 6 };

		path.resize(myGraph->getNumbOfVerts() + 1);

		outputFile << "Otrzymany koszt: ;";
		outputFile << "Współczynnik błędu[j]: ;";
		outputFile << "Współczynnik błędu[%]: ;";
		outputFile << "Czas wykonywania[ms] ;";
		for (int al = 0; al < alfa.size(); al++) {
			cout << "\nDoing for alpha = " << alfa[al] << " ... \n \n";
			outputFile << "Rozwiązania dla alpha = " << alfa[al] << endl;
			for (int be = 0; be < beta.size(); be++) {
				cout << "\nDoing for beta = " << beta[be] << " ... \n \n";
				outputFile << "Rozwiązania dla beta = " << beta[be] << endl;

				llAvgTime = 0;
				llAvgTimefloat = 0;
				avrageErrorRate = 0;
				avrageError = 0;

				llMinTime = INT_MAX;
				llMaxTime = 0;
				llMinErrorRate = INT_MAX;
				llMaxErrorRate = 0;
				llMinError = INT_MAX;
				llMaxError = 0;

				for (int y = 0; y < reader.iRNumber[pliki]; y++) {
					TSP* test = new ACO();
					test->load(myGraph);

					exeTime = test->menu(alfa[al], beta[be]);

					error = abs(reader.iOCost[pliki] - test->getBest());
					errorRate = (error / reader.iOCost[pliki]) * 100;

					cout << "Error " << error << "\n";
					cout << "Error rate " << errorRate << "%\n";
					cout << "Time " << exeTime / (double)1000000 << " ms\n";
					cout << "Found solution " << test->getBest() << endl << endl;
					outputFile << test->getBest() << " ; ";
					outputFile << error << " ; ";
					outputFile << errorRate << " ; ";
					outputFile << exeTime / (double)1000000 << " ; " << endl;

					avrageErrorRate = avrageErrorRate + errorRate;
					llAvgTime = llAvgTime + llTempTime;
					avrageError = avrageError + error;

					llTempTime = exeTime;
					if (llTempTime < llMinTime) {
						llMinTime = llTempTime;
					}
					else if (llTempTime > llMaxTime) {
						llMaxTime = llTempTime;
					}
					if (errorRate < llMinErrorRate) {
						llMinErrorRate = errorRate;
					}
					else if (errorRate > llMaxErrorRate) {
						llMaxErrorRate = errorRate;
					}
					if (error < llMinError) {
						llMinError = error;
					}
					else if (error > llMaxError) {
						llMaxError = error;
					}
				}
				cout << "Optimal solution " << reader.iOCost[pliki] << endl;

				llAvgTimefloat = llAvgTime / reader.iRNumber[pliki];
				outputFile << "Średni czas wykonywania algorytmu [ms]: " << " ; " << llAvgTimefloat / (double)1000000 << endl;
				outputFile << "Średni współczynnik błędu [j]: " << " ; " << avrageError / reader.iRNumber[pliki] << endl;
				outputFile << "Średni współczynnik błędu [%]: " << " ; " << avrageErrorRate / reader.iRNumber[pliki] << endl;
				outputFile << "Optymalny koszt: " << " ; " << reader.iOCost[pliki] << endl << endl;
				llAvgTimefloat = 0;
				llAvgTime = 0;
				llTempTime = 0;

				cout << "Maksymalny wspolczynnik bledu [j]: " << llMaxError << endl;
				cout << "Minimalny wspolczynnik bledu [j]: " << llMinError << endl;
				cout << "Sredni wspolczynnik bledu [j]: " << avrageError / reader.iRNumber[pliki] << endl;
				cout << "Maksymalny wspolczynnik bledu [%]: " << llMaxErrorRate << endl;
				cout << "Minimalny wspolczynnik bledu [%]: " << llMinErrorRate << endl;
				cout << "Sredni wspolczynnik bledu [%]: " << avrageErrorRate / reader.iRNumber[pliki] << endl;
				cout << "Maksymalny czas wykonywania algorytmu [ms]: " << llMaxTime / (double)1000000 << endl;
				cout << "Minimalny czas wykonywania algorytmu [ms]: " << llMinTime / (double)1000000 << endl;
				cout << "Sredni czas wykonywania algorytmu [ms]: " << llAvgTimefloat / (double)1000000 << endl;

				cout << endl;
				cout << endl;

				cout << "Done...\n";
			}
		}
	}

	cout << "All done... \nGood luck... \n";
	getchar();
	return 0;
}