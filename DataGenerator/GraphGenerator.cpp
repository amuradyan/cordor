#include "Exception.h"
#include "Graph.h"

#include <ctime>
#include <fstream>
#include <cstdlib>
#include <iostream>

void Generator::generateGraphFromInput(std::string filename, Graph & graph) {
	std::ifstream inputFile(inputFileName, std::ifstream::in);
	if (!inputFile.is_open()) {
		throw ExceptionError("Can not open input file");
	}
	int lines_count = 0;
	std::string line = "";
	double long x_coord = 0;
	double long y_coord = 0;
	std::string delimitor = ",";
	int del_index = 0;
	int prev_index = -1;
	int index = 0;
	while (std::getline(inputFile, line)) {
		if (line.length() != 0) {
			del_index = line.find(delimitor);
			x_coord = std::stold(line.substr(0, del_index));
			y_coord = std::stold(line.substr(del_index + 1, line.length()));

			index = graph.addVertex(x_coord, y_coord);

			if (prev_index != -1) {
				graph.addEdge(prev_index, index);
			}

			prev_index = index;
		}
		else {
			prev_index = -1;
		}
	}
	inputFile.close();
}

void Graph::generateJsonObject(std::string guid, std::string miniBusNum, long double Long, long double Lat)
{
	outStream << " {" << std::endl;
	outStream << "  \"GUID\": \"" <<  guid << "\"," << std::endl;
	outStream << "  \"MiniBus\": \"" << miniBusNum << "\"," << std::endl;
	outStream << "  \"Long\": " << std::to_string(Long) << "," << std::endl;
	outStream << "  \"Lat\": " << std::to_string(Lat) << "," << std::endl;
	outStream << "  \"Time\": " << std::time(0) << std::endl;
	outStream << " }," << std::endl;
}
