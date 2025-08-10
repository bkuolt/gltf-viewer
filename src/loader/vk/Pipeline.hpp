#ifndef BGL_VK_PIPELINE_HPP
#define BGL_VK_PIPELINE_HPP

#include <vulkan/vulkan.hpp>

VkPipeline CreatePipeline(
    VkDevice device,
    VkExtent2D extent,
    VkRenderPass renderPass,
    VkPipelineLayout pipelineLayout,
    VkShaderModule vertShaderModule,
    VkShaderModule fragShaderModule);

#endif  // BGL_VK_PIPELINE_HPP