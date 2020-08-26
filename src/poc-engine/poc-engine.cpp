#include <iostream>

#include "poc-engine.h"

#include "rendering/window.h"
#include "rendering/rendering-system.h"

using namespace poc;

void poc::PocEngine::run()
{
	std::cout << "[POC::PocEngine] Starting..." << std::endl;
	
	auto window = layers::Window::openWindow(1024, 768, "PocEngine");
	auto renderingSystem = systems::RenderingSystem(*window.get());

	std::cout << "[POC::PocEngine] Started" << std::endl;

	while (!window->isClosing()) {
		window->update();
		renderingSystem.render();
	}

	std::cout << "[POC::PocEngine] Stopping..." << std::endl;

	std::cout << "[POC::PocEngine] Stopped" << std::endl;

}
