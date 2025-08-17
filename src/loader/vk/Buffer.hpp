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

// Beispiel-Vertex-Daten
std::array<Vertex, 3> vertices = {{
    {{0.0f, -0.5f, 0.0f}, {1.f, 0.f, 0.f}},
    {{0.5f, 0.5f, 0.0f},  {0.f, 1.f, 0.f}},
    {{-0.5f, 0.5f, 0.0f}, {0.f, 0.f, 1.f}},
}};

// -----------------------------------------------------

uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}

vk::DeviceMemory getDeviceMemory(vk::Device device,
                                 vk::Buffer buffer, 
                                 vk::MemoryPropertyFlags memoryFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent) {

    // 2. Speicherbedarf abfragen
    vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements(buffer);
    uint32_t memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, memoryFlags);

    // 4. Speicher allokieren
    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = memoryTypeIndex;
    return device.allocateMemory(allocInfo);
}    

int createBufferExample(vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eVertexBuffer) {
    vk::Device device /* dein Vulkan Device */;
    vk::PhysicalDevice physicalDevice /* dein Physical Device */;
    vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    // Buffer erstellen
    vk::BufferCreateInfo bufferInfo{};
    bufferInfo.size = bufferSize;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;
    vk::Buffer vertexBuffer = device.createBuffer(bufferInfo);

    // Buffer und Speicher verbinden
    auto memory = getDeviceMemory(device, vertexBuffer);
    device.bindBufferMemory(vertexBuffer, memory, 0);

    // Daten in Buffer kopieren (map, memcpy, unmap)
    void* data = device.mapMemory(memory, 0, bufferSize);
    std::memcpy(data, vertices.data(), (size_t)bufferSize);
    device.unmapMemory(memory);

    // --- Clean up (später, wenn Buffer nicht mehr gebraucht wird) ---
    // device.destroyBuffer(vertexBuffer);
    // device.freeMemory(vertexBufferMemory);
}


// index
// vertex