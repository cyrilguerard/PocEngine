#include "window.h"

namespace poc::layers {

	bool Window::created = false;

	Window::Window() {
		created = true;
	}

	Window::~Window() {
		created = false; 
	}

}