#include "Exception.h"
#include "Graph.h"
#include "Objects.h"

#include <ctime>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <iostream>



std::vector<std::list<Route>> Routes;

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

void Graph::generateJsonObject( Moving_obj obj, Route route)
{
	outStream << "{";
	outStream << "\"beacon_id\": \"" << obj.id << "\",";
	outStream << "\"beacon_lat\": " << std::to_string(obj.coord_y) << ",";
	outStream << "\"beacon_lon\": " << std::to_string(obj.coord_x) << ",";
	outStream << "\"route_segment_id\": \"" <<  route.id << "\",";
	outStream << "\"timestamp\": " << obj.moving_time << "\",";
	outStream << "\"transport_id\": \"" << obj.minibusNum << "\",";
	outStream << "\"number_of_vehicles\": 1";
	outStream << "\"lat_1\": " << std::to_string(route.startPtr.second) << ",";
	outStream << "\"lon_1\": " << std::to_string(route.startPtr.first) << ",";
	outStream << "\"lat_2\": " << std::to_string(route.endPtr.second) << ",";
	outStream << "\"lon_2\": " << std::to_string(route.endPtr.first);
	outStream << "}" << std::endl;
}

void Generator::reGenerateGraph(Graph & graph)
{
	auto vertices = graph.getVertices();
	Route tempRoute;
	std::pair<long double, long double> prevPoint;
	std::vector<Node>::iterator iter = vertices.begin();
	for (int i = 0; iter != vertices.end(); ++i, ++iter) {
		std::list<idx> adjs = graph.getAdjacencies(i);
		for (auto adj : adjs) {
			prevPoint = std::pair<long double, long double>(vertices[i].x, vertices[i].y);
			std::vector<std::pair<long double, long double>> points = graph.generatePoints(vertices[i], vertices[adj]);
			std::vector<std::pair<long double, long double>>::iterator point_iter = points.begin();
			for (; point_iter != points.end(); ++point_iter) {
				tempRoute = Generator::generateRoute(prevPoint, *point_iter);
				Routes.resize(i+ 1);
				Routes[i].push_back(tempRoute);
				prevPoint = *point_iter;
			}
		}
	}
}

std::vector<Moving_obj> Generator::GenerateMovingObjects(int count)
{
	std::vector<Moving_obj> objs;
	Moving_obj obj;
	for (int i = 0; i < count; ++i) {
		obj.coord_x = 0;
		obj.coord_y = 0;
		obj.moving_time = 0;
		obj.id = boost::uuids::random_generator()();
		obj.speed = midSpeed + rand() % 10;
		// generate minibuse number
		obj.minibusNum = minibuses[rand() % minibuses.size()];
		objs.push_back(obj);
	}
	
	return objs;
}

Route Generator::generateRoute(std::pair<long double, long double> start, std::pair<long double, long double> end)
{
	Route route;
	route.startPtr = start;
	route.endPtr = end;
	route.id = boost::uuids::random_generator()();
	return route;
}