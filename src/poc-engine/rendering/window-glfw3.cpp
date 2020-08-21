#include "window.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <iostream>

namespace poc::layers::window {

	Window::Window(int width, int height, std::string title) {
		if (!glfwInit()) {
			std::exit(-1);
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int xpos = (mode->width - width) / 2;
		int ypos = (mode->height - height) / 2;
		glfwSetWindowPos(window, xpos, ypos);

		glfwShowWindow(window);

		handler = window;
	}

	Window::~Window() {
		glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(handler));
		glfwTerminate();
	}

	bool Window::isClosing() {
		return glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(handler));
	}

	void Window::update() {
		glfwPollEvents();
	}

}
