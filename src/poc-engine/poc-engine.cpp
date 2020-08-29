#include <iostream>

#include "poc-engine.h"

#include "core/logger.hpp"
#include "rendering/window.h"
#include "rendering/rendering-system.h"

using namespace poc;

const std::string PocEngine::tag = "POC::PocEngine";

void poc::PocEngine::run()
{
	Logger::info(tag, "Starting...");
	
	auto window = layers::Window::openWindow(1024, 768, "PocEngine");
	auto renderingSystem = systems::RenderingSystem(*window.get());

	Logger::info(tag, "Started");

	while (!window->isClosing()) {
		window->update();
		renderingSystem.render();
	}

	Logger::info(tag, "Stopping...");

	Logger::info(tag, "Stopped");

}
