#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-instance.hpp"
#include "vulkan-surface.hpp"

namespace poc {

	class VulkanPhysicalDevice {
	public:

		explicit VulkanPhysicalDevice(const VulkanInstance& instance, const VulkanSurface& surface);

		const vk::PhysicalDevice& getPhysicalDevice() const;
		const vk::Format& getDepthFormat() const;

		const uint32_t findMemoryTypeIndex(uint32_t type, vk::MemoryPropertyFlags properties) const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};


}