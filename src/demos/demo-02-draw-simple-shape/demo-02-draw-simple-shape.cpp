#include "demo-02-draw-simple-shape.hpp"

#include <exception>
#include <iostream>

int main() {

	try {

		std::vector<poc::Vertex> vertices = {
			poc::Vertex { glm::vec3(0.0, -0.5, 0.0) },
			poc::Vertex { glm::vec3(-0.5, 0.5, 0.0)},
			poc::Vertex { glm::vec3(0.5, 0.5, 0.0)}
		};

		auto scene = poc::Scene();
		scene.addMesh(poc::Mesh(std::move(vertices)));

		auto engine = poc::PocEngine::make();
		engine->loadScene(std::move(scene));
		engine->run();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}