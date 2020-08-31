#include "rendering-system.hpp"

#include <cassert>

using namespace poc;

namespace poc {

	class RenderingSystemImpl : public RenderingSystem {
	public:

		RenderingSystemImpl(const Window& window, GraphicApi::Type type) :
			graphicApi(GraphicApi::make(window, type)) {
		}

		void render() override {}

	private:

		std::unique_ptr<GraphicApi> graphicApi;

	};

	std::unique_ptr<RenderingSystem> RenderingSystem::make(const Window& window, GraphicApi::Type type) {
		return std::make_unique<RenderingSystemImpl>(window, type);
	}

}