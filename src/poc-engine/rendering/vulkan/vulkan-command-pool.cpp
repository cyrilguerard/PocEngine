#include "vulkan-command-pool.hpp"

#include "../../core/logger.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanCommandPool" };

	vk::UniqueCommandPool createCommandPool(const VulkanDevice& device) {
		assert(device.getDevice() && "device not initialized");

		const auto createInfo = vk::CommandPoolCreateInfo()
			.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
			.setQueueFamilyIndex(device.getGraphicsQueueIndex());
		return device.getDevice().createCommandPoolUnique(createInfo);
	}

	class VulkanCommandPool::Impl {
	public:

		Impl(const VulkanDevice& device) :
			commandPool(createCommandPool(device)) {

			Logger::info(logTag, "Command pool created");
		}

	private:
		vk::UniqueCommandPool commandPool;

	};

	VulkanCommandPool::VulkanCommandPool(const VulkanDevice& device) :
		pimpl(make_unique_pimpl<VulkanCommandPool::Impl>(device)) { }

}

