#include "vulkan-image.hpp"

#include "../../core/logger.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanImage" };

	static vk::UniqueImage createImage(
		const vk::Device& device,
		const vk::Format& format,
		const uint32_t width,
		const uint32_t height,
		const vk::ImageTiling& tiling,
		const vk::ImageUsageFlags& usage) {

		assert(device && "device not initialized");

		const auto extent = vk::Extent3D{ width, height, 1 };

		const auto createInfo = vk::ImageCreateInfo()
			.setImageType(vk::ImageType::e2D)
			.setFormat(format)
			.setExtent(extent)
			.setMipLevels(1)
			.setArrayLayers(1)
			.setSamples(vk::SampleCountFlagBits::e1)
			.setTiling(tiling)
			.setUsage(usage)
			.setSharingMode(vk::SharingMode::eExclusive)
			.setInitialLayout(vk::ImageLayout::eUndefined);

		return device.createImageUnique(createInfo);
	}

	static vk::UniqueDeviceMemory allocateImageMemory(
		const VulkanPhysicalDevice& physicalDevice,
		const vk::Device& device,
		const vk::Image& image,
		const vk::MemoryPropertyFlags& memoryProperties) {

		assert(physicalDevice.getPhysicalDevice() && "physicalDevice not initialized");
		assert(device && "device not initialized");

		const vk::MemoryRequirements requirements = device.getImageMemoryRequirements(image);
		const uint32_t memoryTypeIndex = physicalDevice.findMemoryTypeIndex(requirements.memoryTypeBits, memoryProperties);

		const auto allocateInfo = vk::MemoryAllocateInfo()
			.setAllocationSize(requirements.size)
			.setMemoryTypeIndex(memoryTypeIndex);

		vk::UniqueDeviceMemory memory{ device.allocateMemoryUnique(allocateInfo) };
		device.bindImageMemory(image, *memory, { 0 });
		return memory;
	}

	static void executeTransitionCommand(
		const VulkanCommandPool& commandPool,
		const VulkanDevice& device,
		const vk::PipelineStageFlags srcStageMask,
		const vk::PipelineStageFlags dstStageMask,
		const vk::ImageMemoryBarrier& barrier) {

		const vk::UniqueCommandBuffer commandBuffer{ commandPool.beginCommandBuffer(device) };
		commandBuffer->pipelineBarrier(srcStageMask, dstStageMask, {}, 0, nullptr, 0, nullptr, 1, &barrier);
		commandPool.endCommandBuffer(device, *commandBuffer);

	}

	static void transitionToImageLayout(
		const VulkanCommandPool& commandPool,
		const VulkanDevice& device,
		const vk::Image& image,
		const vk::ImageLayout& imageLayout) {

		assert(device.getDevice() && "device not initialized");

		const auto imgSubResource = vk::ImageSubresourceRange()
			.setBaseMipLevel(0)
			.setLevelCount(1)
			.setBaseArrayLayer(0)
			.setLayerCount(1);

		auto barrier = vk::ImageMemoryBarrier()
			.setOldLayout(vk::ImageLayout::eUndefined)
			.setNewLayout(imageLayout)
			.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
			.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
			.setImage(image)
			.setSubresourceRange(imgSubResource);

		switch (imageLayout)
		{
		case vk::ImageLayout::eDepthStencilAttachmentOptimal:
		{
			barrier.setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite);
			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
			executeTransitionCommand(commandPool, device,
				vk::PipelineStageFlagBits::eTopOfPipe,
				vk::PipelineStageFlagBits::eEarlyFragmentTests,
				barrier);
			break;
		}
		default:
		{
			Logger::error(logTag, "Unsupported image transition: " + vk::to_string(imageLayout));
			throw std::runtime_error("Unsupported image transition: " + vk::to_string(imageLayout));
		}
		}
	}

	class VulkanImage::Impl {
	public:

		const vk::Format format;
		const vk::UniqueImage image;
		const vk::UniqueDeviceMemory imageMemory;

		Impl(
			const VulkanCommandPool& commandPool,
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const vk::Format& format,
			const uint32_t width,
			const uint32_t height,
			const vk::ImageTiling& tiling,
			const vk::ImageUsageFlags& usage,
			const vk::MemoryPropertyFlags& memoryProperties,
			const vk::ImageLayout& imageLayout) :
			format(format),
			image(createImage(device.getDevice(), format, width, height, tiling, usage)),
			imageMemory(allocateImageMemory(physicalDevice, device.getDevice(), *image, memoryProperties)) {

			transitionToImageLayout(commandPool, device, *image, imageLayout);

		}

	};

	VulkanImage::VulkanImage(
		const VulkanCommandPool& commandPool,
		const VulkanPhysicalDevice& physicalDevice,
		const VulkanDevice& device,
		const vk::Format& format,
		const uint32_t width,
		const uint32_t height,
		const vk::ImageTiling& tiling,
		const vk::ImageUsageFlags& usage,
		const vk::MemoryPropertyFlags& memoryProperties,
		const vk::ImageLayout& imageLayout) :
		pimpl(make_unique_pimpl<VulkanImage::Impl>(
			commandPool, physicalDevice, device, format, width, height, tiling, usage, memoryProperties, imageLayout)) { }

	const vk::Image VulkanImage::getImage() const {
		return *pimpl->image;
	}

	const vk::Format VulkanImage::getFormat() const {
		return pimpl->format;
	}

}

