#include "Graph.h"
#include "Exception.h"
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>

extern std::vector<std::list<Route>> Routes;

Graph::Graph():
	outStream(outputFileName, std::ofstream::out),
	guids(guidFileName, std::ifstream::in)
{
	if (!outStream.is_open()) {
		throw ExceptionError("Can not open output file");
	}
	if (!guids.is_open()) {
		throw ExceptionError("Can not open GUIDs file");
	}
}

Graph::~Graph()
{
	outStream.close();
	guids.close();
}

int Graph::addVertexes(std::vector<std::pair<long double, long double>> verteces)
{
	for (auto vertex : verteces) {
		if ( addVertex(vertex.first, vertex.second) == ErrCode) {
			return ErrCode;
		}
	}
	return OKCode;
}

int Graph::addEdge(Node firstVertex, Node secondVertex)
{
	return OKCode;
}

int Graph::addEdge(uint firstVertexIndex, uint secondVertexIndex)
{
	if (firstVertexIndex < 0 || firstVertexIndex >= m_vertices.size() || secondVertexIndex < 0
		|| secondVertexIndex >= m_vertices.size()) {
		return ErrCode;
		// maybe need a generation of an exception
	}
	if (std::find(m_adjacencyList[firstVertexIndex].begin(), m_adjacencyList[firstVertexIndex].end(), secondVertexIndex) == m_adjacencyList[firstVertexIndex].end()) {
		m_adjacencyList[firstVertexIndex].push_back(secondVertexIndex);
	}
	if (std::find(m_adjacencyList[secondVertexIndex].begin(), m_adjacencyList[secondVertexIndex].end(), firstVertexIndex ) == m_adjacencyList[secondVertexIndex].end()) {
		m_adjacencyList[secondVertexIndex].push_back(firstVertexIndex);
	}
	return OKCode;
}

int Graph::addVertex(long double x, long double y)
{
	Node tempNode(x, y);
	std::vector<Node>::iterator iter = std::find(m_vertices.begin(), m_vertices.end(), tempNode);
	if (iter == m_vertices.end()) {
		m_vertices.push_back(Node(x, y));
		m_adjacencyList.resize(m_adjacencyList.size() + 1);
		return m_vertices.size() - 1;
	}
	else {
		return std::distance(m_vertices.begin(), iter);
	}
}


void Graph::printGraph()
{
	std::cout << "------GRAPH------" << std::endl;
	int i = 0;
	int j = 0;
	std::cout.precision(12);
	for (auto vertex : m_vertices) {
		std::cout << vertex.x << " " << vertex.y << std::endl;
		j = 0;
		while ( j < m_adjacencyList[i].size()) {
			std::cout << "     "<< m_vertices[*std::next(m_adjacencyList[i].begin(), j)].x << " " << m_vertices[*std::next(m_adjacencyList[i].begin(), j)].y << std::endl;
			++j;
		}
		++i;
	}
}

void Graph::DFSUtil(int v, bool visited[]) {
	/*visited[v] = true;

	std::list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i) {
		if (!visited[*i]) {
			DFSUtil(*i, visited);
		}
	}*/
}

long double dist_eee(Node first, Node second) {
	return (pow(pow(second.y - first.y, 2) + pow(second.y - first.y, 2) , 0.5));
}

// Need output genrerator with mini generators
void Graph::simulate(std::vector<Moving_obj> Mov_Objs)
{
	int i = 0;
	int index = 0;
	int adjIndex = 0;
	std::string guide = "";
	Node currentNode;
	int time = 0;
	int timestep = 1000;    // milis
	int distance = 0;
	int time_count = 1;
	for (auto obj : Mov_Objs) {
		time = 12100000;
		obj.moving_time = time;
		while (i != IdsMaxCount) {
			index = rand() % m_vertices.size();
			currentNode = m_vertices[index];
			// generate count of max vertices to go
			int roadMaxLength = rand() % 10 + 1;
			int j_adj = 0;
			if (m_adjacencyList[index].size() != 0) {
				while (j_adj != roadMaxLength) {
					adjIndex = rand() % m_adjacencyList[index].size();
					distance = obj.speed * timestep * time_count;
					std::cout << "distance: " << distance << std::endl;
					std::cout << "dist: " << dist_eee(m_vertices[index], m_vertices[adjIndex]) * 1000000 << std::endl;
					while (distance < dist_eee(m_vertices[index], m_vertices[adjIndex]) * 10000000) {
						// sksec amen varkyan generation anel
						// generate new point
						std::cout << "in while" << std::endl;
						generateNewPossition(obj, distance, index, m_vertices[adjIndex]);

						index = adjIndex;
						
						time += timestep;
						++time_count;
					}
					++j_adj;
				}
			}

			++i;
		}
	}
}

std::vector<std::pair<long double, long double>> Graph::generatePoints(const Node& startPoint, const Node& endPoint)
{
	std::vector<std::pair<long double, long double>> LatLongs;
	long double x1 = startPoint.x;
	long double x2 = endPoint.x;
	long double y1 = startPoint.y;
	long double y2 = endPoint.y;
	
	long double Long = 0; //x
	long double Lat = 0;  //y

	int countOFSteps = 1;

	long double x_distance = abs(endPoint.x - startPoint.x);
	if (startPoint.x < endPoint.x) {
		while (true) {
			Long = ((x2 - x1) * countOFSteps * routeLength) / pow((pow(x2 - x1, 2) + pow(y2 - y1, 2)), 0.5) + x1; //x
			if (Long > x2) {
				break;
			}																		   // Lat calculation formula Y = y1 + ((y2 - y1)/ (x2 - x1)) * (x - x1);
			Lat = startPoint.y + ((endPoint.y - startPoint.y) / (endPoint.x - startPoint.x)) * (Long - startPoint.x);
			LatLongs.push_back(std::pair<long double, long double>(Long, Lat));
			++countOFSteps;
		};
	}
	else if (startPoint.x > endPoint.x) {
		 while (true) {
			Long = x2 + ((x1 - x2) * countOFSteps * routeLength) / pow((pow(x2 - x1, 2) + pow(y2 - y1, 2)), 0.5); //x
			if (Long > x1) {
				break;
			}
			Lat = startPoint.y + ((endPoint.y - startPoint.y) / (endPoint.x - startPoint.x)) * (Long - startPoint.x);
			LatLongs.push_back(std::pair<long double, long double>(Long, Lat));
			++countOFSteps;
		 }
	}
	else {
		//it means startPoint and endPoint are in the same abscis
		Long = startPoint.x; // x is const in this case
		int count = 1;
		while (true) {
			if (startPoint.y < endPoint.y) {
				Lat = startPoint.y + count * routeLength;
			}
			else {
				Lat = endPoint.y + count * routeLength;
			}
			++count;
			if (endPoint.y > startPoint.y) {
				if (Lat > endPoint.y) {
					break;
				}
			}
			else {
				if (Lat > startPoint.y) {
					break;
				}
			}
			LatLongs.push_back(std::pair<long double, long double>(Long, Lat));
		}
	}
	return LatLongs;
}

template<class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, std::pair<long double, long double> value)
{
	for (; first != last; ++first) {
		if (*first.x_coord == value.first && *first.y_coord == value.second) {
			return first;
		}
	}
	return last;
}

void Graph::generateNewPossition(Moving_obj obj, int distance, int startIndex, Node end)
{
	Route route;
	// find new possition
	long double Long;
	long double Lat;
	Node start = m_vertices[startIndex];

	if (start.x < end.x) {
		Long = ((end.x - start.x) * distance) / pow((pow(end.x - start.x, 2) + pow(end.y - start.y, 2)), 0.5) + start.x; //x																		   // Lat calculation formula Y = y1 + ((y2 - y1)/ (x2 - x1)) * (x - x1);
		Lat = start.y + ((end.y - start.y) / (end.x - start.x)) * (Long - start.x);
	}
	else if (start.x > end.x) {
		Long = end.x + ((start.x - end.x) * distance) / pow((pow(end.x - start.x, 2) + pow(end.y - start.y, 2)), 0.5); //x
		Lat = start.y + ((end.y - start.y) / (end.x - start.x)) * (Long - start.x);
	}
	else {
		//it means startPoint and endPoint are in the same abscis
		Long = start.x; // x is const in this case
		if (start.y < end.y) {
			Lat = start.y + distance;
		}
		else {
			Lat = end.y + distance;
		}
	}

	obj.coord_x = Long;
	obj.coord_y = Lat;

	// find in which route is that point

	for (auto route_ptr : Routes[startIndex]) {
		if (route_ptr.startPtr.first < Long && Long < route_ptr.endPtr.second) {
			route = route_ptr;
		}
	}
	
	// generate json
	generateJsonObject(obj, route);
}

