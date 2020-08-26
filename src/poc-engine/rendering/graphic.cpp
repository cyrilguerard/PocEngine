#include <assert.h>

#include "graphic.h"
#include "graphic-vulkan.h"

namespace poc::layers {

	bool Graphic::created = false;

	Graphic::Graphic(Api api) : api(api) {
		created = true;
	}

	Graphic::~Graphic() {
		created = false; 
	}

	Graphic::Api Graphic::getApi() const {
		return api;
	}

	std::unique_ptr<Graphic> Graphic::createGraphicApi(const Window& window, Api api) {
		assert(!created && "Graphic API already created");
		switch (api) {
		case Api::VULKAN:
			return std::unique_ptr<Graphic> { new GraphicVulkan(window) };
		default:
			assert(0 && "Unsupported Graphic API");
		}
		return nullptr;
	}

	std::string Graphic::toString(Api api) {
		switch (api) {
		case Api::VULKAN:
			return "VULKAN";
		default:
			return "UNKNOWN";
		}
	}

}