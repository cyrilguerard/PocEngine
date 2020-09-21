#include "vulkan-graphic-api.hpp"

#include "../../core/logger.hpp"
#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"
#include "vulkan-instance.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-render.hpp"
#include "vulkan-surface.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanGraphicApi" };

	class VulkanGraphicApi::Impl {
	public:

		const VulkanInstance instance;
		const VulkanSurface surface;
		const VulkanPhysicalDevice physicalDevice;
		const VulkanDevice device;
		const VulkanCommandPool commandPool;
		const VulkanRender vRender;

		Impl(const Window& window) :
			instance(),
			surface(instance, window),
			physicalDevice(instance, surface),
			device(physicalDevice, surface),
			commandPool(device),
			vRender(VulkanRender(window, physicalDevice, device, surface, commandPool)) {

			Logger::info(logTag, "Vulkan API fully initialized");
		}

		void render(const Scene& scene) {
			if (!scene.isEmpty()) {
				vRender.render(device, VulkanScene(physicalDevice, device, commandPool, scene));
			}
		}

	};

	VulkanGraphicApi::VulkanGraphicApi(const Window& window) :
		pimpl(make_unique_pimpl<VulkanGraphicApi::Impl>(window)) {};

	void VulkanGraphicApi::render(const Scene& scene) {
		pimpl->render(scene);
	};

}

