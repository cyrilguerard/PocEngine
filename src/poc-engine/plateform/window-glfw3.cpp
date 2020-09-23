#include "window.hpp"

#include "../core/logger.hpp"
#include "../plateform/platform.hpp"

using namespace poc;

namespace poc {

#ifdef POC_LAYERS_WINDOW_USE_GLFW3

	constexpr char logTag[]{ "POC::WindowGlfw3" };

	void errorCall(int, const char* description) {
		Logger::error("POC::Glfw3", description);
	}



	class WindowGlfw3 : public Window {
	public:

		WindowGlfw3(int width, int height, const std::string& title) {
			initGlfw();
			initGlfwWindow(width, height, title);
			displayWindow(width, height);
		}

		~WindowGlfw3() {
			glfwDestroyWindow(window);
			glfwTerminate();
		}

		virtual Size getDrawableSurfaceSize() const override {
			int width = 0;
			int height = 0;
			glfwGetFramebufferSize(window, &width, &height);
			return Size{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		}

		virtual void setResizeCallback(OnResizeCallback callback) override {
			onResizeCallback = callback;
		}

		virtual bool isClosing() const override {
			return glfwWindowShouldClose(window);
		}

		virtual void update() override {
			glfwPollEvents();
		}

		virtual void waitWhileMinimized() const override {
			Size size{};
			while (size.width == 0 || size.height == 0) {
				size = this->getDrawableSurfaceSize();
				glfwWaitEvents();
			}
		}

		// deleted
		WindowGlfw3(const WindowGlfw3& other) = delete;
		WindowGlfw3& operator=(const WindowGlfw3& other) = delete;
		WindowGlfw3(WindowGlfw3&& other) noexcept = delete;
		WindowGlfw3& operator=(WindowGlfw3&& other) noexcept = delete;

	private:

		GLFWwindow* window = nullptr;
		OnResizeCallback onResizeCallback;

		void initGlfw() {
			glfwSetErrorCallback(errorCall);
			if (!glfwInit()) {
				Logger::error(logTag, "Failed to initialize GLFW");
				throw std::runtime_error("Failed to initialize GLFW");
			}
		}

		void initGlfwWindow(int width, int height, const std::string& title) {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
			window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
			if (!window) {
				Logger::error(logTag, "Failed to create GLFW window");
				throw std::runtime_error("Failed to create GLFW window");
			}
			glfwSetWindowUserPointer(window, this);
			glfwSetWindowSizeCallback(window, onResize);
		}

		static void onResize(GLFWwindow* window, int width, int height) {
			const WindowGlfw3* w = reinterpret_cast<WindowGlfw3*>(glfwGetWindowUserPointer(window));
			if (w->onResizeCallback) {
				(w->onResizeCallback)(width, height);
			}
		}

		void displayWindow(int width, int height) {
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			int xpos = (mode->width - width) / 2;
			int ypos = (mode->height - height) / 2;
			glfwSetWindowPos(window, xpos, ypos);
			glfwShowWindow(window);
		}

		friend void Window::toGraphicApi(const Window& window, const VkInstance& instance, VkSurfaceKHR* surface);
	};

	std::unique_ptr<Window> Window::openWindow(int width, int height, const std::string& title) {
		return std::unique_ptr<WindowGlfw3>{ new WindowGlfw3(width, height, title) };
	}

	template<>
	static void Window::toGraphicApi(const Window& window, const VkInstance& instance, VkSurfaceKHR* surface) {
		if (!glfwVulkanSupported()) {
			Logger::error(logTag, "Vulkan not supported by Glfw3");
			throw std::runtime_error("Vulkan not supported by Glfw3");
		}

		auto windowGlfw3 = dynamic_cast<const WindowGlfw3*>(&(window));

		VkResult result = glfwCreateWindowSurface(instance, windowGlfw3->window, nullptr, surface);
		if (result != VK_SUCCESS) {
			Logger::error(logTag, "Window surface creation failed (Glfw3/Vulkan)");
			throw std::runtime_error("Window surface creation failed (Glfw3/Vulkan)");
		}
	}



#endif // POC_LAYERS_WINDOW_USE_GLFW3

}

