#include "vulkan-command-pool.hpp"

#include "../../core/logger.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanCommandPool" };

	static vk::UniqueCommandPool createCommandPool(const VulkanDevice& device) {
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

		std::vector<vk::UniqueCommandBuffer> createCommandBuffers(const vk::Device& device, const uint32_t count) {
			assert(device && "device not initialized");

			const auto allocateInfo = vk::CommandBufferAllocateInfo()
				.setCommandPool(*commandPool)
				.setLevel(vk::CommandBufferLevel::ePrimary)
				.setCommandBufferCount(count);
			return device.allocateCommandBuffersUnique(allocateInfo);
		}

		vk::UniqueCommandBuffer beginCommandBuffer(const VulkanDevice& device) {
			assert(device.getDevice() && "device not initialized");

			auto allocateInfo = vk::CommandBufferAllocateInfo()
				.setCommandPool(*commandPool)
				.setLevel(vk::CommandBufferLevel::ePrimary)
				.setCommandBufferCount(1);

			vk::UniqueCommandBuffer commandBuffer{
				device.getDevice().allocateCommandBuffers(allocateInfo)[0],
				{ device.getDevice(), *commandPool }
			};

			auto beginInfo = vk::CommandBufferBeginInfo()
				.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

			commandBuffer->begin(beginInfo);
			return commandBuffer;
		}

		void endCommandBuffer(const VulkanDevice& device, const vk::CommandBuffer& commandBuffer) {
			assert(device.getDevice() && "device not initialized");
			assert(commandBuffer && "commandBuffer not initialized");

			commandBuffer.end();

			const auto submitInfo = vk::SubmitInfo()
				.setCommandBufferCount(1)
				.setPCommandBuffers(&commandBuffer);

			device.getGraphicsQueue().submit(1, &submitInfo, vk::Fence{});
			device.getGraphicsQueue().waitIdle();
		}

	private:
		vk::UniqueCommandPool commandPool;

	};

	VulkanCommandPool::VulkanCommandPool(const VulkanDevice& device) :
		pimpl(make_unique_pimpl<VulkanCommandPool::Impl>(device)) { }

	std::vector<vk::UniqueCommandBuffer> VulkanCommandPool::createCommandBuffers(const VulkanDevice& device, const uint32_t count) const {
		return pimpl->createCommandBuffers(device.getDevice(), count);
	}

	vk::UniqueCommandBuffer VulkanCommandPool::beginCommandBuffer(const VulkanDevice& device) const {
		return pimpl->beginCommandBuffer(device);
	}

	void VulkanCommandPool::endCommandBuffer(const VulkanDevice& device, const vk::CommandBuffer& commandBuffer) const {
		pimpl->endCommandBuffer(device, commandBuffer);
	}

}

