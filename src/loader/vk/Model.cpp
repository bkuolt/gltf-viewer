
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