#include "vulkan-image-view.hpp"

#include "../../core/logger.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanImageView" };

	static vk::UniqueImageView createImageView(const vk::Device& device, const vk::Image& image, const vk::Format& format) {

		const auto subresourceRange = vk::ImageSubresourceRange()
			.setAspectMask(vk::ImageAspectFlagBits::eColor)
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

		Impl(const vk::Device& device, const vk::Image& image, const vk::Format& format)
			: imageView(createImageView(device, image, format)) { }

	private:
		vk::UniqueImageView imageView;

		friend VulkanImageView;
	};

	VulkanImageView::VulkanImageView(const vk::Device& device, const vk::Image& image, const vk::Format& format) :
		pimpl(make_unique_pimpl<VulkanImageView::Impl>(device, image, format)) { }

	const vk::ImageView& VulkanImageView::getImageView() const {
		return *pimpl->imageView;
	}
}

