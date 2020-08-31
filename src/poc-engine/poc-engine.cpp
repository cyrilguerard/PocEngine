#include <iostream>

#include "poc-engine.hpp"

#include "core/logger.hpp"
#include "plateform/window.hpp"
#include "rendering/graphic-api.hpp"
#include "rendering/rendering-system.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::PocEngine" };

	class PocEngineImpl : public PocEngine {

		void run() override
		{
			Logger::info(logTag, "Starting...");

			auto window = Window::openWindow(1024, 768, "PocEngine");
			auto renderingSystem = RenderingSystem::make(*window, GraphicApi::Type::VULKAN);

			Logger::info(logTag, "Started");

			while (!window->isClosing()) {
				window->update();
				renderingSystem->render();
			}

			Logger::info(logTag, "Stopping...");

			Logger::info(logTag, "Stopped");

		}

	};

	std::unique_ptr<PocEngine> PocEngine::make() {
		return std::make_unique<PocEngineImpl>();
	}

}
