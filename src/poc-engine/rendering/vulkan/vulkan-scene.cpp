#include "vulkan-scene.hpp"

namespace poc {

	static VulkanBuffer createVertexBuffer(
		const VulkanPhysicalDevice& physicalDevice,
		const VulkanDevice& device,
		const VulkanCommandPool& commandPool,
		const Scene& scene) {

		const auto size = vk::DeviceSize(sizeof(Vertex) * scene.getVertexCount());
		assert(size > 0);

		return VulkanBuffer::createDeviceLocalBuffer(
			physicalDevice,
			device,
			commandPool,
			size,
			vk::BufferUsageFlagBits::eVertexBuffer,
			scene.getVertexes().data());
	}

	class VulkanScene::Impl {
	public:

		Impl(const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const VulkanCommandPool& commandPool,
			const Scene& scene) :
			vertexCount(scene.getVertexCount()),
			vertexBuffer(createVertexBuffer(physicalDevice, device, commandPool, scene)) {

		}

		uint32_t vertexCount;
		VulkanBuffer vertexBuffer;

	};

	VulkanScene::VulkanScene(
		const VulkanPhysicalDevice& physicalDevice,
		const VulkanDevice& device,
		const VulkanCommandPool& commandPool,
		const Scene& scene) :
		pimpl(make_unique_pimpl<VulkanScene::Impl>(physicalDevice, device, commandPool, scene)) {}

	uint32_t VulkanScene::getVertexCount() const {
		return pimpl->vertexCount;
	}

	const VulkanBuffer& VulkanScene::getVertexBuffer() const {
		return pimpl->vertexBuffer;
	}

}