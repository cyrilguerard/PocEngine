#include "vulkan-swapchain.hpp"

#include <algorithm>

#include "../../core/logger.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanSwapchain" };

	static vk::SurfaceFormatKHR selectSurfaceFormat(const vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR& surface) {
		assert(physicalDevice && "physicalDevice not initialized");
		assert(surface && "surface not initialized");

		const std::vector<vk::SurfaceFormatKHR> formats = physicalDevice.getSurfaceFormatsKHR(surface);
		assert(!formats.empty() && "no supported formats");

		const auto it = std::find_if(formats.cbegin(), formats.cend(),
			[](const auto& format) {
				return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
			});
		if (it != formats.cend()) {
			return *it;
		}

		// else take the fist one
		return formats[0];
	}

	static uint32_t computeMinImageCount(const vk::SurfaceCapabilitiesKHR& capabilities) {

		// use triple buffering by default
		uint32_t minImageCount = 3;

		if (minImageCount < capabilities.minImageCount) {
			// take min count + 1 to avoid waiting the driver
			minImageCount = capabilities.minImageCount + 1;
		}

		if (capabilities.maxImageCount > 0 && minImageCount > capabilities.maxImageCount) {
			minImageCount = capabilities.maxImageCount;
		}

		return minImageCount;
	}

	static vk::Extent2D computeImageExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const Window& window) {

		// if defined
		if (capabilities.currentExtent.width != 0xFFFFFFFF) {
			return capabilities.currentExtent;
		}

		const auto [width, height] = window.getDrawableSurfaceSize();

		vk::Extent2D extent;
		extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, width));
		extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, height));
		return extent;
	}

	static vk::PresentModeKHR selectPresentMode(const vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR& surface) {
		const auto presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
		if (std::any_of(presentModes.cbegin(), presentModes.cend(),
			[](const auto pm) { return pm == vk::PresentModeKHR::eMailbox; })) {
			// not tearing and more efficient than FIFO, use if available
			return vk::PresentModeKHR::eMailbox;
		};

		// required to be supported by the GPU
		return vk::PresentModeKHR::eFifo;
	}

	static vk::UniqueSwapchainKHR createSwapchain(
		const VulkanDevice& device,
		const vk::PhysicalDevice physicalDevice,
		const vk::SurfaceKHR& surface,
		const vk::SurfaceFormatKHR& imageFormat,
		const Window& window) {

		assert(device.getDevice() && "device not initialized");
		assert(physicalDevice && "physicalDevice not initialized");
		assert(surface && "surface not initialized");

		const auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
		const auto minImageCount = computeMinImageCount(surfaceCapabilities);
		const auto imageExtent = computeImageExtent(surfaceCapabilities, window);
		const auto presentMode = selectPresentMode(physicalDevice, surface);

		auto createInfo = vk::SwapchainCreateInfoKHR()
			.setSurface(surface)
			.setMinImageCount(minImageCount)
			.setImageFormat(imageFormat.format)
			.setImageColorSpace(imageFormat.colorSpace)
			.setImageExtent(imageExtent)
			.setImageArrayLayers(1)
			.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
			.setImageSharingMode(vk::SharingMode::eExclusive)
			.setPreTransform(surfaceCapabilities.currentTransform)
			.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
			.setPresentMode(presentMode)
			.setClipped(VK_TRUE);

		if (device.hasDistinctPresentationQueue()) {
			std::array<uint32_t, 2> queueFamilyIndices{
				device.getGraphicsQueueIndex(),
				device.getPresentationQueueIndex() };

			createInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
			createInfo.setQueueFamilyIndexCount(static_cast<uint32_t>(queueFamilyIndices.size()));
			createInfo.setPQueueFamilyIndices(queueFamilyIndices.data());
		}

		return device.getDevice().createSwapchainKHRUnique(createInfo);
	}

	static std::vector<vk::Image> createImages(const vk::Device& device, const vk::SwapchainKHR& swapchain) {
		return device.getSwapchainImagesKHR(swapchain);
	}

	class VulkanSwapchain::Impl {
	public:

		Impl(
			const VulkanDevice& device,
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanSurface& surface,
			const Window& window) :
			imageFormat(selectSurfaceFormat(physicalDevice.getPhysicalDevice(), surface.getSurface())),
			swapchain(createSwapchain(device, physicalDevice.getPhysicalDevice(), surface.getSurface(), imageFormat, window)),
			images(createImages(device.getDevice(), *swapchain)) {

			Logger::info(logTag, "SwapChain created");
		}

	private:
		vk::SurfaceFormatKHR imageFormat;
		vk::UniqueSwapchainKHR swapchain;

		std::vector<vk::Image> images;

		friend VulkanSwapchain;
	};

	VulkanSwapchain::VulkanSwapchain(
		const VulkanDevice& device,
		const VulkanPhysicalDevice& physicalDevice,
		const VulkanSurface& surface,
		const Window& window) :
		pimpl(make_unique_pimpl<VulkanSwapchain::Impl>(device, physicalDevice, surface, window)) { }

	const vk::SwapchainKHR& VulkanSwapchain::getSwapchain() const {
		return *pimpl->swapchain;
	}

	const vk::Format& VulkanSwapchain::getFormat() const {
		return pimpl->imageFormat.format;
	}
}

