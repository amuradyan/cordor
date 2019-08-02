#include <iostream>
#include <fstream>

#include "Exception.h"
#include "Graph.h"

int main() {
	try {
		Graph YerevanMap;
		// reads input from file and generates graph of that.
		Generator::generateGraphFromInput(inputFileName, YerevanMap);
		Generator::reGenerateGraph(YerevanMap);
		std::vector<Moving_obj> objects = Generator::GenerateMovingObjects(5);
		//YerevanMap.printGraph();
		YerevanMap.simulate(objects);
	}
	catch (ExceptionError exc) {
		std::cout << "EXCEPTION: " << exc.message() << std::endl;
	}
	return 0;
}