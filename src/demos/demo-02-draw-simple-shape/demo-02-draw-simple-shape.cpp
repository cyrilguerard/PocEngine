#include <exception>
#include <iostream>

#include "demo-02-draw-simple-shape.h"

int main() {

	try {
		auto engine = poc::PocEngine();
		engine.run();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}