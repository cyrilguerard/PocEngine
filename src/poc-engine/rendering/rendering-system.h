#pragma once

#include "graphic.h"
#include "window.h"

namespace poc::systems {

	class RenderingSystem {

	public:

		RenderingSystem(const poc::layers::Window& window);

		void render();

	private:

		std::unique_ptr<poc::layers::Graphic> graphics;

	};

}