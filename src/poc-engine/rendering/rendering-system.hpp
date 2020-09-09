#pragma once

#include "graphic-api.hpp"
#include "../plateform/window.hpp"

namespace poc {

	class RenderingSystem {
	public:

		virtual void render() = 0;
		virtual ~RenderingSystem() {};

		static std::unique_ptr<RenderingSystem> make(const Window& window, GraphicApi::Type type);

	};

}