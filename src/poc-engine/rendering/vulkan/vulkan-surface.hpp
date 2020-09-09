#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "../../plateform/window.hpp"
#include "vulkan-instance.hpp"

namespace poc {

	class VulkanSurface {
	public:

		explicit VulkanSurface(const VulkanInstance& instance, const Window& window);
		const vk::SurfaceKHR getSurface() const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}