#include "vulkan-surface.hpp"

#include "../../core/logger.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanSurface" };

	static vk::UniqueSurfaceKHR createSurface(const VulkanInstance& instance, const Window& window) {
		const VkInstance vkInstance = instance.getInstance();
		assert(vkInstance && "instance not initialized");

		VkSurfaceKHR vkSurface{};
		// expose window as a surface to Vulkan 
		Window::toGraphicApi(window, vkInstance, &vkSurface);
		// wrap the surface to be managed by Vulkan
		return vk::UniqueSurfaceKHR(vkSurface, instance.getInstance());
	}

	class VulkanSurface::Impl {
	public:

		Impl(const VulkanInstance& instance, const Window& window) :
			surface(createSurface(instance, window)) {
			Logger::info(logTag, "Surface created");
		}

	private:
		vk::UniqueSurfaceKHR surface;

		friend VulkanSurface;
	};

	VulkanSurface::VulkanSurface(const VulkanInstance& instance, const Window& window) :
		pimpl(make_unique_pimpl<VulkanSurface::Impl>(instance, window)) { }

	const vk::SurfaceKHR VulkanSurface::getSurface() const {
		return *pimpl->surface;
	}

}

