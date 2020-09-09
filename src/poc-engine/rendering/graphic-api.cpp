#include "graphic-api.hpp"

#include <cassert>

#include "vulkan/vulkan-graphic-api.hpp"

using namespace poc;

namespace poc {

	std::unique_ptr<GraphicApi> GraphicApi::make(const Window& window, GraphicApi::Type type) {
		switch (type) {
		case GraphicApi::Type::VULKAN:
			return std::make_unique<VulkanGraphicApi>(window);
		default:
			assert(0 && "Unsupported Graphic API");
		}
		return nullptr;
	}

}