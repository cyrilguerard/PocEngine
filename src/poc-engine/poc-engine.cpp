#include "poc-engine.hpp"

#include "core/logger.hpp"
#include "plateform/window.hpp"
#include "rendering/graphic-api.hpp"
#include "rendering/rendering-system.hpp"

#include <functional>

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::PocEngine" };

	class PocEngineImpl : public PocEngine {
	public:

		void loadScene(const Scene s) override {
			scene = s;
		}

		void run() override
		{
			Logger::info(logTag, "Starting...");

			const auto window = Window::openWindow(1280, 720, "PocEngine");
			window->setResizeCallback(std::bind(&PocEngineImpl::onResize, this, std::placeholders::_1, std::placeholders::_2));

			const auto renderingSystem = RenderingSystem::make(*window, GraphicApi::Type::VULKAN);

			Logger::info(logTag, "Started");

			while (!window->isClosing()) {
				window->update();
				renderingSystem->render(*window.get(), scene);
			}

			Logger::info(logTag, "Stopping...");

			Logger::info(logTag, "Stopped");

		}

		void onResize(int width, int height) {
			Logger::info(logTag, std::to_string(width) + " " + std::to_string(height));
		}

	private:
		Scene scene;

	};

	std::unique_ptr<PocEngine> PocEngine::make() {
		return std::make_unique<PocEngineImpl>();
	}

}
