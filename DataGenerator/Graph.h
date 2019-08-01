#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <list>
#include <utility>
#include <fstream>
#include <time.h>

#include "Definitions.h"

// Structure for saving vertics
struct Node {
	Node() : x(0), y(0) {};
	Node(long double x_val, long double y_val) : x(x_val), y(y_val) {};
	long double x;
	long double y;
	bool operator==(const Node& second) {
		return (x == second.x && y == second.y) ? true : false;
	}
};

class Graph {
public:
	//constr/ desctr
	Graph();
	~Graph();
	int addVertexes(std::vector<std::pair<long double, long double>> verteces);
	
	//returns index of the added vertex
	int addVertex(long double x, long double y);
	int addEdge(Node firstVertex, Node secondVertex);
	int addEdge(uint firstVertexIndex, uint secondVertexIndex);

	int getVertecesCount() const { return m_vertices.size(); };
	Node getVertex(int index) const { return m_vertices[index]; };
	// need to be removed or visualized later 
	void printGraph();

	//generators
	void generateObjects();
	std::vector<std::pair<long double, long double>> generatePoints(const Node& first, const Node& second);
	void generateJsonObject(std::string guid, std::string miniBusnum, long double Long, long double Lat);
private:
	void DFSUtil(int v, bool visited[]);  // A function used by DFS
private:
	std::vector<Node> m_vertices;
	std::vector<std::list<index>> m_adjacencyList;
	std::ofstream outStream;
	std::ifstream guids;
};

namespace Generator {
	void generateGraphFromInput(std::string filename, Graph & graph);
	void generateObjects(const Graph& graph);
};

#endif

