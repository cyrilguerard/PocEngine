#pragma once

#include "../plateform/window.hpp"
#include "../core/scene.hpp"
#include "graphic-api.hpp"

namespace poc {

	class RenderingSystem {
	public:

		virtual void render(const Window& window, const Scene& scene) = 0;
		virtual ~RenderingSystem() {};

		static std::unique_ptr<RenderingSystem> make(const Window& window, GraphicApi::Type type);

	};

}