#include <iostream>
#include <fstream>

#include "Exception.h"
#include "Graph.h"

int main() {
	try {
		Graph YerevanMap;
		// reads input from file and generates graph of that.
		Generator::generateGraphFromInput(inputFileName, YerevanMap);
		//YerevanMap.printGraph();
		YerevanMap.generateObjects();
	}
	catch (ExceptionError exc) {
		std::cout << "EXCEPTION: " << exc.message() << std::endl;
	}
	return 0;
}