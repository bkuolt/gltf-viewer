
#include <vulkan/vulkan.hpp> // Vulkan-Hpp Header

struct VulkanBuffer
{
    VkBuffer buffer;
    VkDeviceMemory memory;
    VkDeviceSize size;
};

struct VulkanImage
{
    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
    VkSampler sampler;
};

struct GLTFMeshGPU
{
    VulkanBuffer vertexBuffer;
    VulkanBuffer indexBuffer;
    uint32_t indexCount;
};

struct GLTFMaterialGPU
{
    VulkanImage baseColorTexture;
    VulkanImage normalTexture;
    VulkanImage metallicRoughnessTexture;
};

struct GLTFModelGPU
{
    std::vector<GLTFMeshGPU> meshes;
    std::vector<GLTFMaterialGPU> materials;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
};


// ------------------------------------

VkBuffer createIndexBuffer() {
    return {};  // TODO
}

VkBuffer createVertexBuffer() {
    return {};  // TODO
}

VkCommandBuffer createCommandBuffer() {
    return {};  // TODO
}

VkDescriptorSet createDescriptorSet() {
    return {};  // TODO
}

VkPipeline createPipeline(
    VkDevice device,
    VkExtent2D extent,
    VkRenderPass renderPass,
    VkPipelineLayout pipelineLayout,
    VkShaderModule vertShaderModule,
    VkShaderModule fragShaderModule)
{
    // Shader-Stages
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    // Vertex Input (leerer Dummy wenn keine Vertexdaten)
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    // Input Assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Viewport + Scissor
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) extent.width;
    viewport.height = (float) extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Depth/Stencil
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    // Color Blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    // Dynamic States (optional)
    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;

    // Pipeline Info
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    // Erstellen
    VkPipeline graphicsPipeline;
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    return graphicsPipeline;
}
  
/**
 * @brief Draws a GLTF model using Vulkan.
 */
void Draw(
    VkViewport viewport,
    VkRect2D scissor,
    VkCommandBuffer cmdBuffer,
    VkPipeline pipeline,
    VkPipelineLayout pipelineLayout,
    VkRenderPass renderPass,
    VkRenderPassBeginInfo renderPassInfo,
    VkBuffer vertexBuffer,
    VkBuffer indexBuffer,
    uint32_t indexCount,
    VkDescriptorSet descriptorSet)
{

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    beginInfo.pInheritanceInfo = nullptr;

    // 1. Beginne das Kommando
    vkBeginCommandBuffer(cmdBuffer, &beginInfo);

    // 2. Render Pass starten
    vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // 3. Viewport und Scissor setzen (falls dynamisch)
    vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);
    vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

    // 4. Pipeline binden
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    // 5. Vertex Buffer binden
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vertexBuffer, offsets);

    // 6. Index Buffer binden (wenn du `vkCmdDrawIndexed` nutzt)
    vkCmdBindIndexBuffer(cmdBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    // 7. Descriptor Sets binden (UBOs, Texturen, etc.)
    vkCmdBindDescriptorSets(
        cmdBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipelineLayout,
        0, // firstSet
        1, &descriptorSet,
        0, nullptr // dynamic offsets, falls nötig
    );

    // 8. Draw Call ausführen
    vkCmdDrawIndexed(cmdBuffer, indexCount, 1, 0, 0, 0);

    // 9. Render Pass beenden
    vkCmdEndRenderPass(cmdBuffer);

    // 10. Command Buffer beenden
    vkEndCommandBuffer(cmdBuffer);
}