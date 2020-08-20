
#include <chrono>
#include <iostream>
#include <thread>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "vulkan/vulkan.hpp"

#include "poc-engine.h"

void poc::PocEngine::start()
{
	std::cout << "[POC::GameEngine] Start engine" << std::endl;
	
	const auto createInfo = vk::InstanceCreateInfo();
	const auto instance = vk::createInstanceUnique(createInfo);
	assert(instance);

	glfwInit();
	const auto window = glfwCreateWindow(800, 600, "Poc Engine", nullptr, nullptr);
	assert(window && "Create Window failed");

	std::this_thread::sleep_for(std::chrono::seconds(3));

	glfwDestroyWindow(window);

}
