#pragma once

#include <string>

#include "../core/scene.hpp"
#include "../plateform/window.hpp"

namespace poc {

	class GraphicApi {
	public:

		enum class Type {
			//OPENGL,
			//DIRECTX,
			VULKAN
		};

		virtual void render(const Scene& scene) = 0;
		virtual ~GraphicApi() {}

		static std::unique_ptr<GraphicApi> make(const Window& window, Type type);

	};

}
