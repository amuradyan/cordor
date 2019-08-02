#include "Exception.h"
#include "Graph.h"

#include <ctime>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


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

void Graph::generateJsonObject(std::string beacon_id, std::string transport_id, long double lon, long double lat)
{
	boost::uuids::uuid route_segment_id = boost::uuids::random_generator()();
	outStream << " {" << std::endl;
	outStream << "  \"beacon_id\": \"" <<  beacon_id << "\"," << std::endl;
	outStream << "  \"route_segment_id\": \"" <<  route_segment_id << "\"," << std::endl;
	outStream << "  \"transport_id\": \"" << transport_id << "\"," << std::endl;
	outStream << "  \"lon\": " << std::to_string(lon) << "," << std::endl;
	outStream << "  \"lat\": " << std::to_string(lat) << "," << std::endl;
	outStream << "  \"timestamp\": " << std::time(0) << std::endl;
	outStream << " }," << std::endl;
}
