#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <list>
#include <utility>
#include <fstream>
#include <time.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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

// Need to be moved to Object.h header file

struct Point {
	long double x;
	long double y;
	Point() { x = 0; y = 0; };
	Point(long double c_x, long double c_y) : x(c_x), y(c_y) {};
};

struct Moving_obj {
	boost::uuids::uuid id;
	long double coord_x;
	long double coord_y;
	int speed;
	long moving_time;
	std::string route_id;    // maybe needs to be removed
	std::string minibusNum;
};

struct  Route {
	boost::uuids::uuid  id;
	std::pair<long double, long double> startPtr;
	std::pair<long double, long double> endPtr;
	Route operator=(Route second) {
		id = second.id;
		startPtr = second.startPtr;
		endPtr = second.endPtr;
		return *this;
	}
};

const std::vector<std::string> minibuses = {
	"8",
	"26",
	"63",
	"5",
	"52A",
	"18"
};

// end of Object.h

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
	std::vector<Node>& getVertices() { return m_vertices; };
	std::list<idx> getAdjacencies(int index) { return m_adjacencyList[index]; };
	// need to be removed or visualized later 
	void printGraph();

	//generators
	void simulate(std::vector<Moving_obj> Mov_Objs);
	std::vector<std::pair<long double, long double>> generatePoints(const Node& first, const Node& second);
	void generateNewPossition(Moving_obj obj, int distance, int startIndex, Node end);
	void generateJsonObject(Moving_obj obj, Route route);
private:
	void DFSUtil(int v, bool visited[]);  // A function used by DFS
private:
	std::vector<Node> m_vertices;
	std::vector<std::list<idx>> m_adjacencyList;
	std::ofstream outStream;
};

namespace Generator {
	void generateGraphFromInput(std::string filename, Graph & graph);
	void reGenerateGraph(Graph& graph);
	void generateObjects(const Graph& graph);
	std::vector<Moving_obj> GenerateMovingObjects(int count);

	std::vector<Point> generatePoints(Node start, Node end);
	Route generateRoute(std::pair<long double, long double> start, std::pair<long double, long double> end);
};

#endif

