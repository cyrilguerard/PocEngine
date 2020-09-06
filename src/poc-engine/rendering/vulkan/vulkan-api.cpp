#include "vulkan-api.hpp"

#include "../../core/logger.hpp"
#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"
#include "vulkan-instance.hpp"
#include "vulkan-pipeline.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-render-pass.hpp"
#include "vulkan-surface.hpp"
#include "vulkan-swapchain.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanApi" };

	class VulkanApi::Impl {
	public:

		Impl(const Window& window) :
			instance(),
			surface(instance, window),
			physicalDevice(instance, surface),
			device(physicalDevice, surface),
			swapchain(device, physicalDevice, surface, window),
			commandPool(device),
			renderPass(device, swapchain),
			pipeline(device, swapchain, renderPass) {

			Logger::info(logTag, "Vulkan API fully initialized");

			semaphore = device.getDevice().createSemaphoreUnique(vk::SemaphoreCreateInfo());

		}

		void render() {

			const auto result{ device.getDevice().acquireNextImageKHR(swapchain.getSwapchain(), UINT64_MAX, *semaphore, nullptr) };

			const uint32_t currentImageIndex = result.value;

			Logger::info(logTag, std::to_string(currentImageIndex));

			const auto presentInfo = vk::PresentInfoKHR()
				.setSwapchainCount(1)
				.setPWaitSemaphores(&(*semaphore))
				.setSwapchainCount(1)
				.setPSwapchains(&swapchain.getSwapchain())
				.setPImageIndices(&currentImageIndex);

			device.getPresentationQueue().presentKHR(presentInfo);

		}

	private:
		VulkanInstance instance;
		VulkanSurface surface;
		VulkanPhysicalDevice physicalDevice;
		VulkanDevice device;
		VulkanSwapchain swapchain;
		VulkanCommandPool commandPool;
		VulkanRenderPass renderPass;
		VulkanPipeline pipeline;

		vk::UniqueSemaphore semaphore;
	};

	VulkanApi::VulkanApi(const Window& window) :
		pimpl(make_unique_pimpl<VulkanApi::Impl>(window)) {};

	void VulkanApi::render() {
		pimpl->render();
	};

}

