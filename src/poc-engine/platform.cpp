#include "platform.h"

#if defined( POC_LAYERS_WINDOW_USE_GLFW3 )

void registerWindowVulkanExtensions(std::vector<const char*> extensions) {
	uint32_t count = 0;
	const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&count);
	for (uint32_t i = 0; i < count; i++) {
		extensions.push_back(requiredExtensions[i]);
	}
}

#endif
