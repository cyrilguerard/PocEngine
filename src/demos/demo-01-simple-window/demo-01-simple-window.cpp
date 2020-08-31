#include "demo-01-simple-window.hpp"

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