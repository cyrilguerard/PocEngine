#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-device.hpp"

namespace poc {

	class VulkanImageView {
	public:

		explicit VulkanImageView(const vk::Device& device, const vk::Image& image, const vk::Format& format);
		const vk::ImageView& getImageView() const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}