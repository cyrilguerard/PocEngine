#include "vulkan-render-pass.hpp"

#include "../../core/logger.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanRenderPass" };

	static vk::UniqueRenderPass createRenderPass(const vk::Device& device, const VulkanSwapchain& swapchain) {
		assert(device && "device not initialized");
		assert(swapchain.getSwapchain() && "swapchain not initialized");

		const auto colorAttachment = vk::AttachmentDescription()
			.setFormat(swapchain.getFormat())
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eStore)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

		const auto colorAttachmentRef = vk::AttachmentReference()
			.setAttachment(0)
			.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		const auto subpass = vk::SubpassDescription()
			.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
			.setColorAttachmentCount(1)
			.setPColorAttachments(&colorAttachmentRef);

		const auto dependency = vk::SubpassDependency()
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setSrcAccessMask({})
			.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

		std::array<vk::AttachmentDescription, 1> attachments{ colorAttachment };

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

		Impl(const VulkanDevice& device, const VulkanSwapchain& swapchain) :
			renderPass(createRenderPass(device.getDevice(), swapchain)) {

			Logger::info(logTag, "Render pass created");
		}

	private:
		vk::UniqueRenderPass renderPass;

		friend VulkanRenderPass;
	};

	VulkanRenderPass::VulkanRenderPass(const VulkanDevice& device, const VulkanSwapchain& swapchain) :
		pimpl(make_unique_pimpl<VulkanRenderPass::Impl>(device, swapchain)) { }

	const vk::RenderPass& VulkanRenderPass::getRenderPass() const {
		return *pimpl->renderPass;
	}

}

