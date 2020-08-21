#include <cassert>

#include "window.h"

namespace poc::layers::window {

	bool Window::created = false;

	Window Window::createWindow(int width, int height, std::string title) {
		assert(!created && "Window already created");
		created = true;
		return Window(width, height, title);
	}

}