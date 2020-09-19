#include "demo-03-depth-test.hpp"

#include <exception>
#include <iostream>

int main() {

	try {

		std::vector<poc::Vertex> vertices = {

			// red triangle in foreground
			poc::Vertex { glm::vec3(-0.5, -1.0, 0.0), glm::vec3(1.0, 0.0, 0.0) },
			poc::Vertex { glm::vec3(-1.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0) },
			poc::Vertex { glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0) },

			// green triangle in middleground
			poc::Vertex { glm::vec3(0.0, -0.5, 0.5), glm::vec3(0.0, 1.0, 0.0) },
			poc::Vertex { glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.0, 1.0, 0.0) },
			poc::Vertex { glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 1.0, 0.0) },

			// blue triangle in background
			poc::Vertex { glm::vec3(0.5, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0) },
			poc::Vertex { glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0) },
			poc::Vertex { glm::vec3(1.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0) },
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