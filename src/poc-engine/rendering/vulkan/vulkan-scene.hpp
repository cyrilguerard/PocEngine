#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../core/scene.hpp"
#include "../../plateform/platform.hpp"

#include "vulkan-buffer.hpp"

namespace poc {

	class VulkanScene {
	public:

		explicit VulkanScene(
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const VulkanCommandPool& commandPool,
			const Scene& scene);

		uint32_t getVertexCount() const;
		const VulkanBuffer& getVertexBuffer() const;


	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;

	};

}