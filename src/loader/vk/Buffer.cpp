#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan.hpp>
#include <array>
#include <cstring> // für memcpy

#include "Buffer.hpp"


Buffer::Buffer(const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        vk::BufferUsageFlags usage,
        vk::DeviceSize bufferSize)
    : _device(device)
{
    vk::BufferCreateInfo bufferInfo{};
    bufferInfo.size = bufferSize;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;
    _buffer = device.createBuffer(bufferInfo);

    auto memory = getDeviceMemory(device, _buffer);
    device.bindBufferMemory(_buffer, memory, 0);
}

Buffer::~Buffer() noexcept {
    _device.destroyBuffer(_buffer);
    _device.freeMemory(_memory);
}

void Buffer::upload(void* data) {
    void* mappedMemory = _device.mapMemory(_memory, 0, _bufferSize);
    std::memcpy(mappedMemory, data, (size_t)_bufferSize);
    _device.unmapMemory(_memory); 
}

vk::DeviceMemory Buffer::getDeviceMemory(vk::MemoryPropertyFlags memoryFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent) {
    vk::MemoryRequirements memRequirements = _device.getBufferMemoryRequirements(_buffer);
    uint32_t memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, memoryFlags);

    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = memoryTypeIndex;
    return _device.allocateMemory(allocInfo);
}

uint32_t Buffer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
    vk::PhysicalDeviceMemoryProperties memProperties = _physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("Failed to find suitable memory type!");
}


// = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent