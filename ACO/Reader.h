#pragma once

#include <string>

class Reader
{
public:
	int iRNumber[1];
	int iOCost[1];
	std::string fileNames[1];
	std::string sFilename;

	int iNOfVertices;
	int **iVertices;
	int *bestPath;

	Reader();
	~Reader();
	bool OpenIni();
	/*
	int OpenTxt(std::string & sFilename);
	int OpenTsp(std::string & sFilename);
	int OpenAtsp(std::string & sFilename);
	*/
	int OpenAll(std::string & sFilename);
};