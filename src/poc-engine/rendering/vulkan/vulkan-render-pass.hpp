#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-device.hpp"
#include "vulkan-swapchain.hpp"

namespace poc {

	class VulkanRenderPass {
	public:

		VulkanRenderPass(const VulkanDevice& device, const VulkanSwapchain& swapchain);

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}