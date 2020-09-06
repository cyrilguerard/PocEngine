#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-device.hpp"
#include "vulkan-render-pass.hpp"
#include "vulkan-swapchain.hpp"

namespace poc {

	class VulkanPipeline {
	public:

		VulkanPipeline(const VulkanDevice& device, const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass);

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}