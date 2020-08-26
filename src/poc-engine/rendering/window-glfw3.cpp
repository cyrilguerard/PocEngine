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

		WindowGlfw3(int width, int height, const std::string& title) {
			initGlfw();
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

		void initGlfw() {
			glfwSetErrorCallback(errorCall);
			if (!glfwInit()) {
				LOG("[POC::WindowGlfw3] Failed to initialize GLFW")
				throw std::runtime_error("Failed to initialize GLFW");
			}
		}

		void initGlfwWindow(int width, int height, const std::string& title) {
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

		// deleted
		WindowGlfw3(const WindowGlfw3& other) = delete;
		WindowGlfw3& operator=(const WindowGlfw3& other) = delete;
		WindowGlfw3(WindowGlfw3&& other) noexcept = delete;
		WindowGlfw3& operator=(WindowGlfw3&& other) noexcept = delete;

		// friends
		friend void Window::exposeToGraphicApi(const VkInstance& instance, VkSurfaceKHR* surface) const;
	};

	std::unique_ptr<Window> Window::openWindow(int width, int height, const std::string& title) {
		assert(!created && "Window already created");
		auto window = std::unique_ptr<WindowGlfw3>{ new WindowGlfw3(width, height, title) };
		created = true;
		return window;
	}

	template<>
	void Window::exposeToGraphicApi(const VkInstance& instance, VkSurfaceKHR* surface) const
	{
		if (!glfwVulkanSupported()) {
			LOG("[POC::WindowGlfw3] Vulkan not supported")
			throw std::runtime_error("Vulkan not supported by Glfw3");
		}

		auto window = dynamic_cast<const WindowGlfw3*>(this);

		VkResult result = glfwCreateWindowSurface(instance, window->window, nullptr, surface);
		if (result != VK_SUCCESS) {
			LOG("[POC::WindowGlfw3] Window surface creation failed (Glfw3/Vulkan)")
			throw std::runtime_error("Window surface creation failed (Glfw3/Vulkan)");
		}

	}

#endif // POC_LAYERS_WINDOW_USE_GLFW3

}

