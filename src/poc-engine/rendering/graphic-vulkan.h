#pragma once

#include "graphic.h"
#include "../platform.h"

namespace poc::layers {

	class GraphicVulkan : public Graphic {

	private:

		static const std::string tag;

		GraphicVulkan(const Window& window);

		vk::DynamicLoader loader;
		vk::UniqueInstance instance;
		vk::UniqueDebugUtilsMessengerEXT debugMessenger;
		vk::UniqueSurfaceKHR surface;

		vk::PhysicalDevice physicalDevice;
		vk::UniqueDevice device;
		vk::Queue graphicQueue;
		vk::Queue presentQueue;

		void initializeDispatcher();
		void initializeInstance();
		void initializeDebugMessenger();
		void initializeSurface(const Window& window);
		void initializePhysicalDevice();
		void initializeDevice();

		//deleted
		GraphicVulkan(const GraphicVulkan& other) = delete;
		GraphicVulkan& operator=(const GraphicVulkan& other) = delete;
		GraphicVulkan(GraphicVulkan&& other) noexcept = delete;
		GraphicVulkan& operator=(GraphicVulkan&& other) noexcept = delete;

		// friends
		friend Graphic;
	};

}