#include "vulkan-render.hpp"

#include <vector>

#include "../../core/logger.hpp"
#include "vulkan-pipeline.hpp"
#include "vulkan-render-pass.hpp"
#include "vulkan-swapchain.hpp"


using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanRender" };

	static std::vector <vk::UniqueFramebuffer> createFrameBuffers(
		const vk::Device& device,
		const VulkanSwapchain& swapchain,
		const vk::RenderPass& renderPass) {

		std::vector <vk::UniqueFramebuffer> frameBuffers;
		frameBuffers.reserve(swapchain.getNumberOfImages());

		for (uint32_t i = 0; i < swapchain.getNumberOfImages(); i++) {

			const auto createInfo = vk::FramebufferCreateInfo()
				.setRenderPass(renderPass)
				.setAttachmentCount(1)
				.setPAttachments(&swapchain.getImageViews()[i].getImageView())
				.setWidth(swapchain.getExtent().width)
				.setHeight(swapchain.getExtent().height)
				.setLayers(1);

			frameBuffers.push_back(device.createFramebufferUnique(createInfo));
		}

		return frameBuffers;
	}

	class VulkanRender::Impl {
	public:

		Impl(const Window& window,
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const VulkanSurface& surface,
			const VulkanCommandPool& commandPool) :
			swapchain(VulkanSwapchain(window, physicalDevice, device, surface)),
			renderPass(VulkanRenderPass(device, swapchain)),
			pipeline(VulkanPipeline(device, swapchain, renderPass)),
			maxBufferingFrames(swapchain.getNumberOfImages()),
			frameBuffers(createFrameBuffers(device.getDevice(), swapchain, renderPass.getRenderPass())),
			commandBuffers(commandPool.createCommandBuffers(device, maxBufferingFrames)),
			imageFences(maxBufferingFrames),
			frameFences(device.createFences(maxBufferingFrames)),
			imageAcquisitionSemaphores(device.createSemaphores(maxBufferingFrames)),
			graphicCompletedSemaphores(device.createSemaphores(maxBufferingFrames)) {

			Logger::info(logTag, "Vulkan render initialized");
		}

		void render(const VulkanDevice& device) {

			const vk::Fence frameFence{ *frameFences[currentFrame] };
			const vk::Semaphore imageSemaphore{ *imageAcquisitionSemaphores[currentFrame] };
			const vk::Semaphore graphicSemaphore{ *graphicCompletedSemaphores[currentFrame] };

			// ensure the number of frames is not exceeding the number of images in the swapchain
			device.getDevice().waitForFences(1, &frameFence, VK_TRUE, UINT64_MAX);

			const auto [result, currentImage] = device.getDevice().acquireNextImageKHR(swapchain.getSwapchain(), UINT64_MAX, imageSemaphore, nullptr);

			// ensure the same image is not used in parallel
			const vk::Fence imageFence = { imageFences[currentImage] };
			if (imageFence) {
				device.getDevice().waitForFences(1, &imageFence, VK_TRUE, UINT64_MAX);
			}
			imageFences[currentImage] = frameFence;
			device.getDevice().resetFences(1, &frameFence);

			const vk::Framebuffer frameBuffer{ *frameBuffers[currentFrame] };
			const vk::CommandBuffer commandbuffer{ *commandBuffers[currentFrame] };

			commandbuffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);

			const auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
			commandbuffer.begin(beginInfo);

			vk::ClearValue clearValues{ std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f } };
			const auto renderPassBeginInfo = vk::RenderPassBeginInfo()
				.setRenderPass(renderPass.getRenderPass())
				.setFramebuffer(frameBuffer)
				.setRenderArea({ { 0 , 0 }, swapchain.getExtent() })
				.setClearValueCount(1)
				.setPClearValues(&clearValues);
			commandbuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

			commandbuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.getPipeline());
			commandbuffer.draw(3, 1, 0, 0);

			commandbuffer.endRenderPass();
			commandbuffer.end();

			const vk::PipelineStageFlags stage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
			const auto submitInfo = vk::SubmitInfo()
				.setWaitSemaphoreCount(1)
				.setPWaitSemaphores(&imageSemaphore)
				.setPWaitDstStageMask(&stage)
				.setCommandBufferCount(1)
				.setPCommandBuffers(&commandbuffer)
				.setSignalSemaphoreCount(1)
				.setPSignalSemaphores(&graphicSemaphore);

			device.getGraphicsQueue().submit(1, &submitInfo, frameFence);

			const auto presentInfo = vk::PresentInfoKHR()
				.setWaitSemaphoreCount(1)
				.setPWaitSemaphores(&graphicSemaphore)
				.setSwapchainCount(1)
				.setPSwapchains(&swapchain.getSwapchain())
				.setPImageIndices(&currentImage);

			device.getPresentationQueue().presentKHR(presentInfo);
			device.getPresentationQueue().waitIdle(); //TODO: do better no need to wait here, need to do in object destruction

			currentFrame = (currentFrame + 1) % maxBufferingFrames;
		}



	private:

		const VulkanSwapchain swapchain;
		const VulkanRenderPass renderPass;
		const VulkanPipeline pipeline;

		uint32_t currentFrame{ 0 };
		const uint32_t maxBufferingFrames;

		const std::vector <vk::UniqueFramebuffer> frameBuffers;
		const std::vector <vk::UniqueCommandBuffer> commandBuffers;

		std::vector<vk::Fence> imageFences;
		const std::vector<vk::UniqueFence> frameFences;
		const std::vector<vk::UniqueSemaphore> imageAcquisitionSemaphores;
		const std::vector<vk::UniqueSemaphore> graphicCompletedSemaphores;

	};

	VulkanRender::VulkanRender(
		const Window& window,
		const VulkanPhysicalDevice& physicalDevice,
		const VulkanDevice& device,
		const VulkanSurface& surface,
		const VulkanCommandPool& commandPool) :
		pimpl(make_unique_pimpl<VulkanRender::Impl>(window, physicalDevice, device, surface, commandPool)) { }

	void VulkanRender::render(const VulkanDevice& device) {
		pimpl->render(device);
	}
}


