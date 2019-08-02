#include "Graph.h"
#include "Exception.h"
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>

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
	outStream << "[" << std::endl;
}

Graph::~Graph()
{
	outStream << "]" << std::endl;
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


const std::vector<std::string> minibuses = {
	"8",
	"26",
	"63",
	"5",
	"52A",
	"18"
};


// Need output genrerator with mini generators
void Graph::generateObjects()
{
	int i = 0;
	int index = 0;
	int adjIndex = 0;
	std::string guide = "";
	Node currentNode;
	std::vector<std::pair<long double, long double>> pointsOnRoad;
	while (i != IdsMaxCount) {
		index = rand() % m_vertices.size();
		currentNode = m_vertices[index];
		//Generate Guid
		std::getline(guids, guide);

		//GUID guid;
		//HRESULT hCreateGuid = CoCreateGuid(&guid);
		//OLECHAR* guidString;
		//StringFromCLSID(guid, &guidString);
		// ensure memory is freed
		//::CoTaskMemFree(guidString);
		
		 // generate minibuse number
		std::string minibusNum = minibuses[rand() % minibuses.size()];
		// generate count of max vertices to go
		int roadMaxLength = rand() % 10 + 1;
		int j_adj = 0;
		if (m_adjacencyList[index].size() != 0) {
			while (j_adj != roadMaxLength) {
				adjIndex = rand() % m_adjacencyList[index].size();
				pointsOnRoad = generatePoints(m_vertices[index], m_vertices[adjIndex]);
				for (auto point : pointsOnRoad) {
					generateJsonObject(guide, minibusNum, point.first, point.second);
				}
				index = adjIndex;
				++j_adj;
			}
		}

		++i;
	}
}

std::vector<std::pair<long double, long double>> Graph::generatePoints(const Node& startPoint, const Node& endPoint)
{
	std::vector<std::pair<long double, long double>> LatLongs;
	long double Long = 0;  //x
	long double Lat = 0;   //y
	long double x_distance = abs(endPoint.x - startPoint.x);
	long double step = x_distance / cutsCount;
	if (startPoint.x < endPoint.x) {
		for (int i = 0; i < cutsCount; ++i) {
			Long = startPoint.x + i * step;
			// Lat calculation formula Y = y1 + ((y2 - y1)/ (x2 - x1)) * (x - x1);
			Lat = startPoint.y + ((endPoint.y - startPoint.y) / (endPoint.x - startPoint.x)) * (Long - startPoint.x);
			LatLongs.push_back(std::pair<long double, long double>(Long, Lat));
		}
	}
	else if (startPoint.x > endPoint.x) {
		for (int i = 0; i < cutsCount; ++i) {
			Long = startPoint.x - i * step;
			Lat = startPoint.y + ((endPoint.y - startPoint.y) / (endPoint.x - startPoint.x)) * (Long - startPoint.x);
			LatLongs.push_back(std::pair<long double, long double>(Long, Lat));
		}
	}
	else {
		//it means startPoint and endPoint are in the same abscis
		Long = startPoint.x; // x is const in this case
		for (int i = 0; i < cutsCount; ++i) {
			if (startPoint.y < endPoint.y) {
				Lat = startPoint.y + i * step;
			}
			else {
				Lat = endPoint.y + i * step;
			}
			LatLongs.push_back(std::pair<long double, long double>(Long, Lat));
		}
	}
	return LatLongs;
}