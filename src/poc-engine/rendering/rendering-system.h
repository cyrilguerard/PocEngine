#pragma once

#include "graphic.h"
#include "window.h"

namespace poc::systems {

	class RenderingSystem {

	public:

		RenderingSystem();
		~RenderingSystem();
		poc::layers::Window* openWindow(int width, int height, std::string title);
		void render();

	private:

		poc::layers::Window* window = nullptr;
		poc::layers::Graphic* graphics = nullptr;

	};

}