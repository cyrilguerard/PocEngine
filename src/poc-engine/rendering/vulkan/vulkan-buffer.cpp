#include "vulkan-buffer.hpp"

#include "../../core/logger.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanBuffer" };

	static vk::UniqueBuffer createBuffer(
		const vk::Device& device,
		const vk::DeviceSize& size,
		const vk::BufferUsageFlags& usage) {

		assert(device && "device not initialized");

		return device.createBufferUnique(
			vk::BufferCreateInfo()
			.setSize(size)
			.setUsage(usage)
			.setSharingMode(vk::SharingMode::eExclusive)
		);
	}

	static vk::UniqueDeviceMemory allocateDeviceMemory(
		const VulkanPhysicalDevice& physicalDevice,
		const vk::Device& device,
		const vk::Buffer& buffer,
		const vk::MemoryPropertyFlags& memoryPropertyFlags) {

		assert(physicalDevice.getPhysicalDevice() && "physicalDevice not initialized");
		assert(device && "device not initialized");
		assert(buffer && "buffer not initialized");

		vk::MemoryRequirements requirements = device.getBufferMemoryRequirements(buffer);
		uint32_t memoryTypeIndex = physicalDevice.findMemoryTypeIndex(requirements.memoryTypeBits, memoryPropertyFlags);

		auto allocateInfo = vk::MemoryAllocateInfo()
			.setAllocationSize(requirements.size)
			.setMemoryTypeIndex(memoryTypeIndex);
		return device.allocateMemoryUnique(allocateInfo);
	}



	class VulkanBuffer::Impl {
	public:

		Impl(const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const vk::DeviceSize& size,
			const vk::BufferUsageFlags& usage,
			const vk::MemoryPropertyFlags& memoryProperty,
			const void* data) :
			buffer(createBuffer(device.getDevice(), size, usage)),
			memory(allocateDeviceMemory(physicalDevice, device.getDevice(), *buffer, memoryProperty)) {

			device.getDevice().bindBufferMemory(*buffer, *memory, 0);

			if (data) {
				copyToBuffer(device.getDevice(), size, data);
			}

		}

	private:

		void copyToBuffer(const vk::Device& device, const vk::DeviceSize& size, const void* src) {
			void* dst = device.mapMemory(*memory, 0, size);
			memcpy(dst, src, static_cast<size_t>(size));
			device.unmapMemory(*memory);
		}

		vk::UniqueBuffer buffer;
		vk::UniqueDeviceMemory memory;

		friend VulkanBuffer;
	};

	VulkanBuffer::VulkanBuffer(
		const VulkanPhysicalDevice& physicalDevice,
		const VulkanDevice& device,
		const vk::DeviceSize& size,
		const vk::BufferUsageFlags& usage,
		const vk::MemoryPropertyFlags& memoryProperty,
		const void* data) :
		pimpl(make_unique_pimpl<VulkanBuffer::Impl>(physicalDevice, device, size, usage, memoryProperty, data)) { }

	const vk::Buffer& VulkanBuffer::getBuffer() const {
		return *pimpl->buffer;
	}

	VulkanBuffer VulkanBuffer::createDeviceLocalBuffer(
		const VulkanPhysicalDevice& physicalDevice,
		const VulkanDevice& device,
		const VulkanCommandPool& commandPool,
		const vk::DeviceSize& size,
		const vk::BufferUsageFlags& usage,
		const void* data) {

		VulkanBuffer stagingBuffer{
			physicalDevice,
			device,
			size,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			data
		};

		VulkanBuffer localBuffer{
			physicalDevice,
			device,
			size,
			vk::BufferUsageFlagBits::eTransferDst | usage,
			vk::MemoryPropertyFlagBits::eDeviceLocal,
			nullptr
		};

		vk::UniqueCommandBuffer commandBuffer{ commandPool.beginCommandBuffer(device) };
		auto bufferCopy = vk::BufferCopy().setSrcOffset(0).setDstOffset(0).setSize(size);
		commandBuffer->copyBuffer(stagingBuffer.getBuffer(), localBuffer.getBuffer(), 1, &bufferCopy);
		commandPool.endCommandBuffer(device, *commandBuffer);

		return localBuffer;

	}

}

