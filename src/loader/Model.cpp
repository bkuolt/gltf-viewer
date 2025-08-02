
#include <vulkan/vulkan.hpp>  // Vulkan-Hpp Header



struct VulkanBuffer {
    VkBuffer buffer;
    VkDeviceMemory memory;
    VkDeviceSize size;
};

struct VulkanImage {
    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
    VkSampler sampler;
};

struct GLTFMeshGPU {
    VulkanBuffer vertexBuffer;
    VulkanBuffer indexBuffer;
    uint32_t indexCount;
};

struct GLTFMaterialGPU {
    VulkanImage baseColorTexture;
    VulkanImage normalTexture;
    VulkanImage metallicRoughnessTexture;
};

struct GLTFModelGPU {
    std::vector<GLTFMeshGPU> meshes;
    std::vector<GLTFMaterialGPU> materials;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
};


void CreateImage() {

    vk::Image image;
    vk::DeviceMemory memory;
    vk::ImageView view;
    vk::Sampler sampler;

    // cretae image
    vkCreateImage(nullptr, nullptr, nullptr, nullptr);  // &image, &memory, &view, &sampler);

    // TODO
}