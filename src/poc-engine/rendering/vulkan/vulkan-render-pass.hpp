#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-device.hpp"
#include "vulkan-swapchain.hpp"

namespace poc {

	class VulkanRenderPass {
	public:

		explicit VulkanRenderPass(const VulkanDevice& device, const VulkanSwapchain& swapchain);
		const vk::RenderPass& getRenderPass() const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}