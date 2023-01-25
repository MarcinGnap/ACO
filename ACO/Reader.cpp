#include "Reader.h"
#include <fstream>
#include <iostream>

Reader::Reader() {
}

Reader::~Reader() {
	if (iNOfVertices != 0) {
		for (int i = 0; i < iNOfVertices; i++)
			delete iVertices[i];
		delete[] iVertices;
		iVertices = NULL;
		iNOfVertices = 0;
	}
}

bool Reader::OpenIni() {
	std::ifstream ifFileini("config.ini", std::ios::in);
	if (ifFileini.good())
	{
		ifFileini >> alpha;
		ifFileini >> beta;
		ifFileini >> qval;
		ifFileini >> aco;
		for (int z = 0; z < 1; z++) {

			ifFileini >> fileNames[z];
			ifFileini >> iRNumber[z];
			ifFileini >> iOCost[z];
		}
		//return Reader::OpenAll(sFilename);
		/*
		std::string sMatrixFile = sFilename.substr(sFilename.find_last_of(".") + 1);
		if (sMatrixFile != "txt" && sMatrixFile != "tsp" && sMatrixFile != "atsp")
		{
			sFilename += ".txt";
		}
		sMatrixFile = sFilename.substr(sFilename.find_last_of(".") + 1);
		if (sMatrixFile == "txt") {
			return Reader::OpenTxt(sFilename);
		}
		if (sMatrixFile == "tsp") {
			return Reader::OpenTsp(sFilename);
		}
		if (sMatrixFile == "atsp") {
			return Reader::OpenAtsp(sFilename);
		}
		*/

		ifFileini.close();

		return true;
	}
	return false;
}
/*
int Reader::OpenTxt(std::string & sFilename) {
	std::cout << "Loading...\n";
	std::fstream fFiletxt;
	fFiletxt.open(sFilename, std::ios::in);
	if (!fFiletxt.good())
	{
		this->sFilename = "Brak pliku";
		std::cout << "Nie mozna wczytac pliku...";
		return 0;
	}
	if (iNOfVertices != 0)
	{
		for (int i = 0; i < iNOfVertices; ++i) {
			delete iVertices[i];
		}
		delete[] iVertices;
		iVertices = NULL;
		iNOfVertices = 0;
	}
	std::cout << "Txt file opened...\n";
	this->sFilename = sFilename;
	fFiletxt >> iNOfVertices;
	iVertices = new int *[iNOfVertices];
	int iLine = 0;
	std::cout << "\nNumber of Vertices: " << iNOfVertices << std::endl << std::endl;
	for (int i = 0; i < iNOfVertices; ++i) {
		iVertices[i] = new int[iNOfVertices];
	}
	while (!fFiletxt.eof()) {
		for (int x = 0; x < iNOfVertices; ++x) {
			for (int j = 0; j < iNOfVertices; ++j) {
				fFiletxt >> iVertices[x][j];
			}
			iLine++;
		}
	}
	std::cout << "Instance copied...\n";
	fFiletxt.close();
	if (iLine != iNOfVertices && iLine != iNOfVertices * 2) {
		std::cout << "Niepoprawna ilosc danych...";
		this->sFilename = "Brak pliku";
		for (int i = 0; i < iNOfVertices; ++i) {
			delete iVertices[i];
		}
		delete[] iVertices;
		iVertices = NULL;
		iNOfVertices = 0;
		return 1;
	}
	std::cout << "Wczytanie pliku przebieglo poprawnie\n";
	return 0;
}
int Reader::OpenTsp(std::string & sFilename)
{
	std::ifstream File;
	File.open(sFilename, std::ios::in);
	if (!File.good()) {
		this->sFilename = "Brak wczytanego pliku";
		std::cout << "\nNie mozna wczytac pliku!\n";
		return 0;
	}
	this->sFilename = sFilename;
	if (iNOfVertices != 0) {
		for (int i = 0; i < iNOfVertices; i++)
			delete iVertices[i];
		delete[] iVertices;
		iVertices = NULL;
		iNOfVertices = 0;
	}
	std::string line;
	while (!File.eof()) {
		std::getline(File, line);
		{
			if (!line.compare(0, 6, "TYPE: ")) {
				if (!line.compare(7, 3, "TSP")) {
					std::cout << "\n To nie jest plik TSP\n"; // czy typ pliku to TSP
					return 1;
				}
			}
			else if (!line.compare(0, 7, "TYPE : ")) {
				if (!line.compare(8, 3, "TSP")) {
					std::cout << "\n To nie jest plik TSP\n"; // czy typ pliku to TSP
					return 1;
				}
			}
			if (!line.compare(0, 11, "DIMENSION: ")) {
				std::string number = line.substr(11, 100);
				iNOfVertices = std::stoi(number); //zapisanie ilo�ci wierzcho�k�w
			}
			else if (!line.compare(0, 12, "DIMENSION : ")) {
				std::string number = line.substr(12, 100);
				iNOfVertices = std::stoi(number); //zapisanie ilo�ci wierzcho�k�w
			}
			if (!line.compare(0, 18, "EDGE_WEIGHT_TYPE: ")) {
				if (!line.compare(19, 8, "EXPLICIT")) {
					std::cout << "\n Nieobslugiwany typ danych\n"; // czy zapisano jako wagi kraw�dzi
					return 1;
				}
			}
			else if (!line.compare(0, 19, "EDGE_WEIGHT_TYPE : ")) {
				if (!line.compare(20, 8, "EXPLICIT")) {
					std::cout << "\n Nieobslugiwany typ danych\n"; // czy zapisano jako wagi kraw�dzi
					return 1;
				}
			}
			if (!line.compare(0, 20, "EDGE_WEIGHT_FORMAT: ")) { //Obs�uga r�nego typu zapisu macierzy
				if (!line.compare(20, 11, "FULL_MATRIX")) {
					iVertices = new int*[iNOfVertices];
					for (int i = 0; i < iNOfVertices; i++)
						iVertices[i] = new int[iNOfVertices];
					std::string temp;
					while (!File.eof()) {
						File >> temp;
						if (temp == "EDGE_WEIGHT_SECTION") {
							for (int j = 0; j < iNOfVertices; j++)
								for (int k = 0; k < iNOfVertices; k++)
									File >> iVertices[j][k];
						}
					}
				}
				else if (!line.compare(20, 14, "LOWER_DIAG_ROW")) {
					iVertices = new int*[iNOfVertices];
					for (int i = 0; i < iNOfVertices; i++)
						iVertices[i] = new int[iNOfVertices];
					std::string temp;
					int line = 1;
					while (!File.eof()) {
						File >> temp;
						if (temp == "EDGE_WEIGHT_SECTION") {
							for (int j = 0; j < iNOfVertices; j++) {
								for (int k = 0; k < line; k++) {
									File >> iVertices[j][k];
									if (j != k)
										iVertices[k][j] = iVertices[j][k];
								}
								if (line < iNOfVertices)
									line++;
							}
						}
					}
				}
				else if (!line.compare(20, 14, "UPPER_DIAG_ROW")) {
					iVertices = new int*[iNOfVertices];
					for (int i = 0; i < iNOfVertices; i++)
						iVertices[i] = new int[iNOfVertices];
					std::string temp;
					int line = iNOfVertices - 1;
					while (!File.eof()) {
						File >> temp;
						if (temp == "EDGE_WEIGHT_SECTION") {
							for (int j = iNOfVertices - 1; j >= 0; j--) {
								for (int k = line; k >= 0; k--) {
									File >> iVertices[j][k];
									if (j != k)
										iVertices[k][j] = iVertices[j][k];
								}
								if (line >= 0)
									line--;
							}
						}
					}
				}
				else {
					std::cout << "\nNieobslugiwany format macierzy\n";
					return 1;
				}
			}
			else if (!line.compare(0, 21, "EDGE_WEIGHT_FORMAT : ")) { //Obs�uga r�nego typu zapisu macierzy
				if (!line.compare(21, 11, "FULL_MATRIX")) {
					iVertices = new int*[iNOfVertices];
					for (int i = 0; i < iNOfVertices; i++)
						iVertices[i] = new int[iNOfVertices];
					std::string temp;
					while (!File.eof()) {
						File >> temp;
						if (temp == "EDGE_WEIGHT_SECTION") {
							for (int j = 0; j < iNOfVertices; j++)
								for (int k = 0; k < iNOfVertices; k++)
									File >> iVertices[j][k];
						}
					}
				}
				else if (!line.compare(21, 14, "LOWER_DIAG_ROW")) {
					iVertices = new int*[iNOfVertices];
					for (int i = 0; i < iNOfVertices; i++)
						iVertices[i] = new int[iNOfVertices];
					std::string temp;
					int line = 1;
					while (!File.eof()) {
						File >> temp;
						if (temp == "EDGE_WEIGHT_SECTION") {
							for (int j = 0; j < iNOfVertices; j++) {
								for (int k = 0; k < line; k++) {
									File >> iVertices[j][k];
									if (j != k)
										iVertices[k][j] = iVertices[j][k];
								}
								if (line < iNOfVertices)
									line++;
							}
						}
					}
				}
				else if (!line.compare(21, 14, "UPPER_DIAG_ROW")) {
					iVertices = new int*[iNOfVertices];
					for (int i = 0; i < iNOfVertices; i++)
						iVertices[i] = new int[iNOfVertices];
					std::string temp;
					int line = iNOfVertices - 1;
					while (!File.eof()) {
						File >> temp;
						if (temp == "EDGE_WEIGHT_SECTION") {
							for (int j = iNOfVertices - 1; j >= 0; j--) {
								for (int k = line; k >= 0; k--) {
									File >> iVertices[j][k];
									if (j != k)
										iVertices[k][j] = iVertices[j][k];
								}
								if (line >= 0)
									line--;
							}
						}
					}
				}
				else {
					std::cout << "\nNieobslugiwany format macierzy...\n";
					return 1;
				}
			}
		}
	}
	File.close();
	std::cout << "\nPlik zostal wczytany!\n";
	return 0;
}
int Reader::OpenAtsp(std::string & sFilename)
{
	std::ifstream ifAtspFile;
	ifAtspFile.open(sFilename, std::ios::in);
	if (!ifAtspFile.good()) {
		this->sFilename = "Brak pliku";
		std::cout << "\nNie mozna wczytac pliku...\n";
		return 0;
	}
	this->sFilename = sFilename;
	if (iNOfVertices != 0) {
		for (int i = 0; i < iNOfVertices; i++)
			delete iVertices[i];
		delete[] iVertices;
		iVertices = NULL;
		iNOfVertices = 0;
	}
	std::string line;
	while (!ifAtspFile.eof()) {
		std::getline(ifAtspFile, line);
		{
			if (!line.compare(0, 6, "TYPE: "))
				if (!line.compare(7, 4, "ATSP")) {
					std::cout << "\n To nie jest plik ATSP\n"; // czy typ pliku to ATSP
					return 1;
				}
			if (!line.compare(0, 11, "DIMENSION: ")) {
				std::string number = line.substr(11, 100);
				iNOfVertices = std::stoi(number); //zapisanie ilo�ci wierzcho�k�w
			}
			if (!line.compare(0, 18, "EDGE_WEIGHT_TYPE: "))
				if (!line.compare(19, 8, "EXPLICIT")) {
					std::cout << "\n Nieobslugiwany typ danych\n"; // czy zapisano jako wagi kraw�dzi
					return 1;
				}
			if (!line.compare(0, 20, "EDGE_WEIGHT_FORMAT: ")) { //Obs�uga r�nego typu zapisu macierzy
				if (!line.compare(20, 11, "FULL_MATRIX")) {
					iVertices = new int*[iNOfVertices];
					for (int i = 0; i < iNOfVertices; i++)
						iVertices[i] = new int[iNOfVertices];
					std::string temp;
					while (!ifAtspFile.eof()) {
						ifAtspFile >> temp;
						if (temp == "EDGE_WEIGHT_SECTION") {
							for (int j = 0; j < iNOfVertices; j++)
								for (int k = 0; k < iNOfVertices; k++)
									ifAtspFile >> iVertices[j][k];
						}
					}
				}
				else {
					std::cout << "\nNieobslugiwany format macierzy\n";
					return 1;
				}
			}
		}
	}
	ifAtspFile.close();
	std::cout << "\nPlik zostal wczytany poprawnie...\n";
	return 0;
}
*/
int Reader::OpenAll(std::string & sFilename) {
	std::cout << "Loading...\n";

	std::fstream fFiletxt;
	fFiletxt.open(sFilename, std::ios::in);
	if (!fFiletxt.good())
	{
		this->sFilename = "Brak pliku";
		std::cout << "Nie mozna wczytac pliku...";
		return 0;
	}

	if (iNOfVertices != 0)
	{
		for (int i = 0; i < iNOfVertices; ++i) {
			delete iVertices[i];
		}
		delete[] iVertices;
		iVertices = NULL;
		iNOfVertices = 0;
	}
	std::cout << "Txt file opened...\n";

	this->sFilename = sFilename;
	fFiletxt >> iNOfVertices;
	iVertices = new int *[iNOfVertices];
	int iLine = 0;

	std::cout << "\nNumber of Vertices: " << iNOfVertices << std::endl << std::endl;

	for (int i = 0; i < iNOfVertices; ++i) {
		iVertices[i] = new int[iNOfVertices];
	}

	while (!fFiletxt.eof()) {
		for (int x = 0; x < iNOfVertices; ++x) {
			for (int j = 0; j < iNOfVertices; ++j) {
				fFiletxt >> iVertices[x][j];
			}
			iLine++;
		}
	}

	std::cout << "Instance copied...\n";

	fFiletxt.close();

	if (iLine != iNOfVertices && iLine != iNOfVertices * 2) {
		std::cout << "Niepoprawna ilosc danych...";
		this->sFilename = "Brak pliku";
		for (int i = 0; i < iNOfVertices; ++i) {
			delete iVertices[i];
		}
		delete[] iVertices;
		iVertices = NULL;
		iNOfVertices = 0;
		return 1;
	}

	std::cout << "Wczytanie pliku przebieglo poprawnie\n";
	return 0;
}