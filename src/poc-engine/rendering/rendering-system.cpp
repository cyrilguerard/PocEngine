#include "rendering-system.h"

namespace poc::systems {

	RenderingSystem::RenderingSystem(const poc::layers::Window& window) {
		graphics = poc::layers::Graphic::createGraphicApi(window, poc::layers::Graphic::Api::VULKAN);
	}

	void RenderingSystem::render() {
		
	}

}