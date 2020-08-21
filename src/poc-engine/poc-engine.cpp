#include "poc-engine.h"
#include "rendering/window.h"

void poc::PocEngine::run()
{
	std::cout << "[POC::PocEngine] Starting..." << std::endl;

	auto window = poc::layers::window::Window::createWindow(1024, 768, "PocEngine");

	std::cout << "[POC::PocEngine] Started" << std::endl;

	while (!window.isClosing()) {
		window.update();
	}

	std::cout << "[POC::PocEngine] Stopping..." << std::endl;

	std::cout << "[POC::PocEngine] Stopped" << std::endl;

}
