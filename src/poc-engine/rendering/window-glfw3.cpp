#include <assert.h>

#include "../debug.h"
#include "../platform.h"
#include "window.h"

namespace poc::layers {

#ifdef POC_LAYERS_WINDOW_USE_GLFW3

	void errorCall(int errorCode, const char* description) {
		LOG("[POC::Glfw3] " << description)
	}

	class WindowGlfw3 : public Window {

	public:

		WindowGlfw3(int width, int height, std::string title, Graphic::Api api) {
			initGlfw(api);
			initGlfwWindow(width, height, title);
			displayWindow(width, height);
		}

		virtual ~WindowGlfw3() {
			glfwDestroyWindow(window);
			glfwTerminate();
		}

		virtual bool isClosing() {
			return glfwWindowShouldClose(window);
		}

		virtual void update() {
			glfwPollEvents();
		}

	private:

		GLFWwindow* window = nullptr;

		void initGlfw(Graphic::Api api) {
			
			glfwSetErrorCallback(errorCall);
			if (!glfwInit()) {
				LOG("[POC::WindowGlfw3] Failed to initialize GLFW")
				throw std::runtime_error("Failed to initialize GLFW");
			}

			if (api != Graphic::Api::VULKAN || !glfwVulkanSupported()) {
				LOG("[POC::WindowGlfw3] " << Graphic::toString(api) << " not supported")
				throw std::runtime_error(Graphic::toString(api) + " not supported by Glfw3");
			}

		}

		void initGlfwWindow(int width, int height, std::string title) {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
			window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
			if (!window) {
				throw std::runtime_error("Failed to create GLFW window");
			}
		}

		void displayWindow(int width, int height) {
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			int xpos = (mode->width - width) / 2;
			int ypos = (mode->height - height) / 2;
			glfwSetWindowPos(window, xpos, ypos);
			glfwShowWindow(window);
		}

	};

	Window* Window::createWindow(int width, int height, std::string title, Graphic::Api api) {
		assert(!created && "Window already created");
		auto window = new WindowGlfw3(width, height, title, api);
		created = true;
		return window;
	}

#endif // POC_LAYERS_WINDOW_USE_GLFW3

}

