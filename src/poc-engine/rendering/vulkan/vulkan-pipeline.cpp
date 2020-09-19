#include "vulkan-pipeline.hpp"

#include "../../core/logger.hpp"
#include "../vertex.hpp"

#include "shaders/vulkan-shader-fragment.hpp"
#include "shaders/vulkan-shader-vertex.hpp"


using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanPipeline" };

	static vk::UniqueShaderModule createShaderModule(
		const vk::Device& device,
		const unsigned char* code,
		const size_t codeSize) {

		const auto createInfo = vk::ShaderModuleCreateInfo()
			.setCodeSize(codeSize)
			.setPCode(reinterpret_cast<const uint32_t*>(code));
		return device.createShaderModuleUnique(createInfo);
	}

	static vk::UniquePipelineLayout createPipelineLayout(const vk::Device& device) {
		const auto createInfo = vk::PipelineLayoutCreateInfo()
			.setSetLayoutCount(0)
			.setPSetLayouts(nullptr)
			.setPushConstantRangeCount(0)
			.setPPushConstantRanges(nullptr)
			.setSetLayoutCount(0)
			.setPSetLayouts(nullptr);
		return device.createPipelineLayoutUnique(createInfo);
	}

	static vk::UniquePipeline createPipeline(
		const vk::Device& device,
		const VulkanSwapchain& swapchain,
		const vk::RenderPass& renderPass,
		const vk::PipelineLayout& layout) {

		assert(device && "device not initialized");
		assert(swapchain.getSwapchain() && "swapchain not initialized");
		assert(renderPass && "renderPass not initialized");
		assert(layout && "layout not initialized");

		const auto vertexModule = createShaderModule(device, gShaderVertex, gShaderVertexLength);
		const auto vertexShader = vk::PipelineShaderStageCreateInfo()
			.setStage(vk::ShaderStageFlagBits::eVertex)
			.setModule(*vertexModule)
			.setPName("main");

		const auto fragmentModule = createShaderModule(device, gShaderFragment, gShaderFragmentLength);
		const auto fragmentShader = vk::PipelineShaderStageCreateInfo()
			.setStage(vk::ShaderStageFlagBits::eFragment)
			.setModule(*fragmentModule)
			.setPName("main");

		const std::vector<vk::PipelineShaderStageCreateInfo> shaderInfos{ vertexShader, fragmentShader };

		const auto vertexBindingDesc = vk::VertexInputBindingDescription()
			.setBinding(0)
			.setStride(sizeof(Vertex))
			.setInputRate(vk::VertexInputRate::eVertex);

		const std::array<vk::VertexInputAttributeDescription, 2> vertexAttributeDescs = {

			vk::VertexInputAttributeDescription()
			.setBinding(0)
			.setLocation(0)
			.setFormat(vk::Format::eR32G32B32Sfloat)
			.setOffset(offsetof(Vertex, position)),

			vk::VertexInputAttributeDescription()
			.setBinding(0)
			.setLocation(1)
			.setFormat(vk::Format::eR32G32B32Sfloat)
			.setOffset(offsetof(Vertex, color))

		};

		const auto vertexInputState = vk::PipelineVertexInputStateCreateInfo()
			.setVertexBindingDescriptionCount(1)
			.setPVertexBindingDescriptions(&vertexBindingDesc)
			.setVertexAttributeDescriptionCount(static_cast<uint32_t>(vertexAttributeDescs.size()))
			.setPVertexAttributeDescriptions(vertexAttributeDescs.data());

		const auto inputAssemblyState = vk::PipelineInputAssemblyStateCreateInfo()
			.setTopology(vk::PrimitiveTopology::eTriangleList)
			.setPrimitiveRestartEnable(VK_FALSE);

		const auto extent = swapchain.getExtent();
		const auto viewport = vk::Viewport()
			.setX(0)
			.setY(0)
			.setWidth(static_cast<float>(extent.width))
			.setHeight(static_cast<float>(extent.height))
			.setMinDepth(0.0f)
			.setMaxDepth(1.0f);

		const auto scissor = vk::Rect2D()
			.setOffset({ 0, 0 })
			.setExtent(extent);

		const auto viewportState = vk::PipelineViewportStateCreateInfo()
			.setViewportCount(1)
			.setPViewports(&viewport)
			.setScissorCount(1)
			.setPScissors(&scissor);

		const auto rasterizationState = vk::PipelineRasterizationStateCreateInfo()
			.setDepthClampEnable(VK_FALSE)
			.setRasterizerDiscardEnable(VK_FALSE)
			.setPolygonMode(vk::PolygonMode::eFill)
			.setCullMode(vk::CullModeFlagBits::eBack)
			.setFrontFace(vk::FrontFace::eCounterClockwise)
			.setDepthBiasEnable(VK_FALSE)
			.setLineWidth(1.0f);

		const auto multisampleState = vk::PipelineMultisampleStateCreateInfo()
			.setSampleShadingEnable(VK_FALSE);

		const auto colorBlendAttachment = vk::PipelineColorBlendAttachmentState()
			.setBlendEnable(VK_FALSE)
			.setColorWriteMask(
				vk::ColorComponentFlagBits::eR |
				vk::ColorComponentFlagBits::eG |
				vk::ColorComponentFlagBits::eB |
				vk::ColorComponentFlagBits::eA);

		const auto colorBlendState = vk::PipelineColorBlendStateCreateInfo()
			.setLogicOpEnable(VK_FALSE)
			.setAttachmentCount(1)
			.setPAttachments(&colorBlendAttachment);

		const auto createInfo = vk::GraphicsPipelineCreateInfo()
			.setStageCount(static_cast<uint32_t>(shaderInfos.size()))
			.setPStages(shaderInfos.data())
			.setPVertexInputState(&vertexInputState)
			.setPInputAssemblyState(&inputAssemblyState)
			.setPTessellationState(nullptr)
			.setPViewportState(&viewportState)
			.setPRasterizationState(&rasterizationState)
			.setPMultisampleState(&multisampleState)
			.setPDepthStencilState(nullptr)
			.setPColorBlendState(&colorBlendState)
			.setPDynamicState(nullptr)
			.setLayout(layout)
			.setRenderPass(renderPass)
			.setSubpass(0)
			.setBasePipelineHandle(nullptr);

		return device.createGraphicsPipelineUnique(nullptr, createInfo);
	}

	class VulkanPipeline::Impl {
	public:

		Impl(const VulkanDevice& device, const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass) :
			pipelineLayout(createPipelineLayout(device.getDevice())),
			pipeline(createPipeline(device.getDevice(), swapchain, renderPass.getRenderPass(), *pipelineLayout)) {

			Logger::info(logTag, "Pipeline created");
		}

	private:
		vk::UniquePipelineLayout pipelineLayout;
		vk::UniquePipeline pipeline;

		friend VulkanPipeline;
	};

	VulkanPipeline::VulkanPipeline(const VulkanDevice& device, const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass) :
		pimpl(make_unique_pimpl<VulkanPipeline::Impl>(device, swapchain, renderPass)) { }

	const vk::Pipeline& VulkanPipeline::getPipeline() const {
		return *pimpl->pipeline;
	}

}

