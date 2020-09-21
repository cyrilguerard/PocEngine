#include "vulkan-render-pass.hpp"

#include "../../core/logger.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanRenderPass" };

	static vk::UniqueRenderPass createRenderPass(
		const VulkanPhysicalDevice& physicalDevice,
		const vk::Device& device,
		const VulkanSwapchain& swapchain) {

		assert(physicalDevice.getPhysicalDevice() && "physicalDevice not initialized");
		assert(device && "device not initialized");
		assert(swapchain.getSwapchain() && "swapchain not initialized");

		const auto colorAttachment = vk::AttachmentDescription()
			.setFormat(swapchain.getFormat())
			.setSamples(physicalDevice.getMaxSampleCount())
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eStore)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);

		const auto colorAttachmentRef = vk::AttachmentReference()
			.setAttachment(0)
			.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		const auto depthStencilAttachment = vk::AttachmentDescription()
			.setFormat(physicalDevice.getDepthFormat())
			.setSamples(physicalDevice.getMaxSampleCount())
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

		const auto depthStencilAttachmentRef = vk::AttachmentReference()
			.setAttachment(1)
			.setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

		const auto resolveAttachment = vk::AttachmentDescription()
			.setFormat(swapchain.getFormat())
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStoreOp(vk::AttachmentStoreOp::eStore)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

		const auto resolveAttachmentRef = vk::AttachmentReference()
			.setAttachment(2)
			.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		const auto subpass = vk::SubpassDescription()
			.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
			.setColorAttachmentCount(1)
			.setPColorAttachments(&colorAttachmentRef)
			.setPDepthStencilAttachment(&depthStencilAttachmentRef)
			.setPResolveAttachments(&resolveAttachmentRef);

		const auto dependency = vk::SubpassDependency()
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setSrcAccessMask({})
			.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

		std::array<vk::AttachmentDescription, 3> attachments{ colorAttachment, depthStencilAttachment, resolveAttachment };

		const auto createInfo = vk::RenderPassCreateInfo()
			.setAttachmentCount(static_cast<uint32_t>(attachments.size()))
			.setPAttachments(attachments.data())
			.setSubpassCount(1)
			.setPSubpasses(&subpass)
			.setDependencyCount(1)
			.setPDependencies(&dependency);

		return device.createRenderPassUnique(createInfo);
	}

	class VulkanRenderPass::Impl {
	public:

		const vk::UniqueRenderPass renderPass;

		Impl(
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const VulkanSwapchain& swapchain) :
			renderPass(createRenderPass(physicalDevice, device.getDevice(), swapchain)) {

			Logger::info(logTag, "Render pass created");
		}

	};

	VulkanRenderPass::VulkanRenderPass(
		const VulkanPhysicalDevice& physicalDevice,
		const VulkanDevice& device,
		const VulkanSwapchain& swapchain) :
		pimpl(make_unique_pimpl<VulkanRenderPass::Impl>(physicalDevice, device, swapchain)) { }

	const vk::RenderPass& VulkanRenderPass::getRenderPass() const {
		return *pimpl->renderPass;
	}

}

