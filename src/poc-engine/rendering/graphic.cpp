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

	Graphic* Graphic::createGraphicApi(Api api) {
		assert(!created && "Graphic API already created");
		switch (api) {
		case Api::VULKAN:
			return new GraphicVulkan();
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