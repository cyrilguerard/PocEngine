#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-instance.hpp"
#include "vulkan-surface.hpp"

namespace poc {

	class VulkanPhysicalDevice {
	public:

		explicit VulkanPhysicalDevice(const VulkanInstance& instance, const VulkanSurface& surface);
		const vk::PhysicalDevice getPhysicalDevice() const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};


}