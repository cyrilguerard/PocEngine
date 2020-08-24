#include "rendering-system.h"

namespace poc::systems {

	poc::layers::Window* RenderingSystem::openWindow(int width, int height, std::string title) {
		window = poc::layers::Window::createWindow(width, height, title, graphics->getApi());
		return window;
	}

	RenderingSystem::RenderingSystem() {
		graphics = poc::layers::Graphic::createGraphicApi(poc::layers::Graphic::Api::VULKAN);
	}

	RenderingSystem::~RenderingSystem() {
		delete graphics;
		if (window) delete window;
	}

	void RenderingSystem::render() {
		
	}

}