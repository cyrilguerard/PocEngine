#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-device.hpp"
#include "vulkan-render-pass.hpp"
#include "vulkan-swapchain.hpp"

namespace poc {

	class VulkanPipeline {
	public:

		explicit VulkanPipeline(const VulkanDevice& device, const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass);
		const vk::Pipeline& getPipeline() const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}