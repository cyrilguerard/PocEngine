#pragma once

#define POC_LAYERS_WINDOW_USE_GLFW3

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include "vulkan/vulkan.hpp"

#if defined( POC_LAYERS_WINDOW_USE_GLFW3 )

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#else

#error Platform not yet supported

#endif

#include <vector>

void registerWindowVulkanExtensions(std::vector<const char*>& extensions);
