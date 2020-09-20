#include "vulkan-image-view.hpp"

using namespace poc;

namespace poc {

	static vk::UniqueImageView createImageView(
		const vk::Device& device,
		const vk::Image& image,
		const vk::Format& format,
		const vk::ImageAspectFlags& imageAspect) {

		const auto subresourceRange = vk::ImageSubresourceRange()
			.setAspectMask(imageAspect)
			.setBaseMipLevel(0)
			.setLevelCount(1)
			.setBaseArrayLayer(0)
			.setLayerCount(1);

		const auto createInfo = vk::ImageViewCreateInfo()
			.setImage(image)
			.setViewType(vk::ImageViewType::e2D)
			.setFormat(format)
			.setSubresourceRange(subresourceRange);

		return device.createImageViewUnique(createInfo);
	}

	class VulkanImageView::Impl {
	public:

		const vk::UniqueImageView imageView;

		Impl(const vk::Device& device,
			const vk::Image& image,
			const vk::Format& format,
			const vk::ImageAspectFlags& imageAspect) :
			imageView(createImageView(device, image, format, imageAspect)) { }

	};

	VulkanImageView::VulkanImageView(
		const vk::Device& device,
		const vk::Image& image,
		const vk::Format& format,
		const vk::ImageAspectFlags& imageAspect) :
		pimpl(make_unique_pimpl<VulkanImageView::Impl>(device, image, format, imageAspect)) { }

	const vk::ImageView& VulkanImageView::getImageView() const {
		return *pimpl->imageView;
	}

}

