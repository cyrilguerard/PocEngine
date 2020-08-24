#pragma once

#include "graphic.h"
#include "../platform.h"

namespace poc::layers {

	class GraphicVulkan : public Graphic {

	private:

		friend Graphic;

		GraphicVulkan();

		vk::DynamicLoader loader;
		vk::UniqueInstance instance;
		vk::UniqueDebugUtilsMessengerEXT debugMessenger;

		void initializeDispatcher();
		void initializeInstance();
		void initializeDebugMessenger();

	};

}