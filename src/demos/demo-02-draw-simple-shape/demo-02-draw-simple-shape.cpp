#include "demo-02-draw-simple-shape.hpp"

#include <exception>
#include <iostream>

int main() {

	try {
		auto engine = poc::PocEngine::make();
		engine->run();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}