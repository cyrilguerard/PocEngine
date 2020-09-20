#include "vulkan-render.hpp"

#include <vector>

#include "../../core/logger.hpp"
#include "vulkan-image.hpp"
#include "vulkan-pipeline.hpp"
#include "vulkan-render-pass.hpp"
#include "vulkan-swapchain.hpp"


using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanRender" };

	static VulkanImage createDepthImage(
		const VulkanCommandPool& commandPool,
		const VulkanPhysicalDevice& physicalDevice,
		const VulkanDevice& device,
		const VulkanSwapchain& swapchain) {

		const auto [width, height] = swapchain.getExtent();

		return VulkanImage(
			commandPool,
			physicalDevice,
			device,
			physicalDevice.getDepthFormat(),
			width,
			height,
			vk::ImageTiling::eOptimal,
			vk::ImageUsageFlagBits::eDepthStencilAttachment,
			vk::MemoryPropertyFlagBits::eDeviceLocal,
			vk::ImageLayout::eDepthStencilAttachmentOptimal);
	}

	static VulkanImageView createDepthImageView(
		const vk::Device& device,
		const VulkanImage& depthImage) {
		return VulkanImageView(device, depthImage.getImage(), depthImage.getFormat(), vk::ImageAspectFlagBits::eDepth);
	}

	static std::vector <vk::UniqueFramebuffer> createFrameBuffers(
		const vk::Device& device,
		const vk::RenderPass& renderPass,
		const VulkanSwapchain& swapchain,
		const vk::ImageView& imageView) {

		std::vector <vk::UniqueFramebuffer> frameBuffers;
		frameBuffers.reserve(swapchain.getNumberOfImages());

		for (uint32_t i = 0; i < swapchain.getNumberOfImages(); i++) {

			std::array<vk::ImageView, 2> imageViews = {
				swapchain.getImageViews()[i].getImageView(),
				imageView
			};

			const auto createInfo = vk::FramebufferCreateInfo()
				.setRenderPass(renderPass)
				.setAttachmentCount(static_cast<uint32_t>(imageViews.size()))
				.setPAttachments(imageViews.data())
				.setWidth(swapchain.getExtent().width)
				.setHeight(swapchain.getExtent().height)
				.setLayers(1);

			frameBuffers.push_back(device.createFramebufferUnique(createInfo));
		}

		return frameBuffers;
	}

	class VulkanRender::Impl {
	public:

		const VulkanSwapchain swapchain;
		const VulkanRenderPass renderPass;
		const VulkanPipeline pipeline;

		uint32_t currentFrame{ 0 };
		const uint32_t maxBufferingFrames;

		const VulkanImage depthImage;
		const VulkanImageView depthImageView;

		const std::vector <vk::UniqueFramebuffer> frameBuffers;
		const std::vector <vk::UniqueCommandBuffer> commandBuffers;

		std::vector<vk::Fence> imageFences;
		const std::vector<vk::UniqueFence> frameFences;
		const std::vector<vk::UniqueSemaphore> imageAcquisitionSemaphores;
		const std::vector<vk::UniqueSemaphore> graphicCompletedSemaphores;

		Impl(const Window& window,
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const VulkanSurface& surface,
			const VulkanCommandPool& commandPool) :
			swapchain(VulkanSwapchain(window, physicalDevice, device, surface)),
			renderPass(VulkanRenderPass(physicalDevice, device, swapchain)),
			pipeline(VulkanPipeline(device, swapchain, renderPass)),
			maxBufferingFrames(swapchain.getNumberOfImages()),
			depthImage(createDepthImage(commandPool, physicalDevice, device, swapchain)),
			depthImageView(createDepthImageView(device.getDevice(), depthImage)),
			frameBuffers(createFrameBuffers(device.getDevice(), renderPass.getRenderPass(), swapchain, depthImageView.getImageView())),
			commandBuffers(commandPool.createCommandBuffers(device, maxBufferingFrames)),
			imageFences(maxBufferingFrames),
			frameFences(device.createFences(maxBufferingFrames)),
			imageAcquisitionSemaphores(device.createSemaphores(maxBufferingFrames)),
			graphicCompletedSemaphores(device.createSemaphores(maxBufferingFrames)) {

			Logger::info(logTag, "Vulkan render initialized");
		}

		void render(const VulkanDevice& device, const VulkanScene& scene) {

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

			std::array<vk::ClearValue, 2> clearValues{
				vk::ClearColorValue{std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f }},
				vk::ClearDepthStencilValue{ 1.0f, 0 }
			};

			const auto renderPassBeginInfo = vk::RenderPassBeginInfo()
				.setRenderPass(renderPass.getRenderPass())
				.setFramebuffer(frameBuffer)
				.setRenderArea({ { 0 , 0 }, swapchain.getExtent() })
				.setClearValueCount(static_cast<uint32_t>(clearValues.size()))
				.setPClearValues(clearValues.data());

			commandbuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
			commandbuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.getPipeline());

			vk::DeviceSize offsets{ 0 };
			commandbuffer.bindVertexBuffers(0, 1, &scene.getVertexBuffer().getBuffer(), &offsets);
			commandbuffer.draw(scene.getVertexCount(), 1, 0, 0);

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

	};

	VulkanRender::VulkanRender(
		const Window& window,
		const VulkanPhysicalDevice& physicalDevice,
		const VulkanDevice& device,
		const VulkanSurface& surface,
		const VulkanCommandPool& commandPool) :
		pimpl(make_unique_pimpl<VulkanRender::Impl>(window, physicalDevice, device, surface, commandPool)) { }

	void VulkanRender::render(const VulkanDevice& device, const VulkanScene& scene) {
		pimpl->render(device, scene);
	}
}


