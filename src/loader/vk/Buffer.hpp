#include <vulkan/vulkan.hpp>

// TODO

VkBuffer createIndexBuffer() {
    return {};  // TODO
}

VkBuffer createVertexBuffer() {
    return {};  // TODO
}


#include <vulkan/vulkan.hpp>
#include <array>
#include <cstring> // für memcpy

struct Vertex {
    float pos[3];
    float color[3];
};


int createBufferExample() {
    // Beispiel-Vertex-Daten
    std::array<Vertex, 3> vertices = {{
        {{0.0f, -0.5f, 0.0f}, {1.f, 0.f, 0.f}},
        {{0.5f, 0.5f, 0.0f},  {0.f, 1.f, 0.f}},
        {{-0.5f, 0.5f, 0.0f}, {0.f, 0.f, 1.f}},
    }};
    
    vk::Device device = /* dein Vulkan Device */;
    vk::PhysicalDevice physicalDevice = /* dein Physical Device */;

    vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    // Lambda zum Finden eines Memory Types
    auto findMemoryType = [&](uint32_t typeFilter, vk::MemoryPropertyFlags properties) -> uint32_t {
        vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        throw std::runtime_error("Failed to find suitable memory type!");
    };

    // 1. Buffer erstellen
    vk::BufferCreateInfo bufferInfo{};
    bufferInfo.size = bufferSize;
    bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    vk::Buffer vertexBuffer = device.createBuffer(bufferInfo);

    // 2. Speicherbedarf abfragen
    vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements(vertexBuffer);

    // 3. Memory Type finden
    uint32_t memoryTypeIndex = findMemoryType(
        memRequirements.memoryTypeBits,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    // 4. Speicher allokieren
    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = memoryTypeIndex;

    vk::DeviceMemory vertexBufferMemory = device.allocateMemory(allocInfo);

    // 5. Buffer und Speicher verbinden
    device.bindBufferMemory(vertexBuffer, vertexBufferMemory, 0);

    // 6. Daten in Buffer kopieren (map, memcpy, unmap)
    void* data = device.mapMemory(vertexBufferMemory, 0, bufferSize);
    std::memcpy(data, vertices.data(), (size_t)bufferSize);
    device.unmapMemory(vertexBufferMemory);

    // vertexBuffer kannst du jetzt beim Rendern binden:
    // vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, offsets);

    // --- Clean up (später, wenn Buffer nicht mehr gebraucht wird) ---
    // device.destroyBuffer(vertexBuffer);
    // device.freeMemory(vertexBufferMemory);
}
