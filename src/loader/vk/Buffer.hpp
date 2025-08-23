#ifndef BGL_VK_BUFFER_HPP
#define BGL_VK_BUFFER_HPP
#include <vulkan/vulkan.hpp>

class Buffer {
    public:
        Buffer(const vk::Device& device,
               const vk::PhysicalDevice& physicalDevice,
               vk::BufferUsageFlags usage,
               vk::DeviceSize bufferSize);
        virtual ~Buffer() noexcept;
        
        void upload(void* data);

    private:
        vk::DeviceMemory getDeviceMemory(vk::MemoryPropertyFlags memoryFlags);
        uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

        vk::DeviceSize _bufferSize;
        vk::Buffer _buffer;
        vk::DeviceMemory _memory;
        const vk::Device& _device;
};

class VertexBuffer : public Buffer {
    public:
        VertexBuffer(vk::Device device, vk::PhysicalDevice physicalDevice, vk::DeviceSize size)
            : Buffer(device, physicalDevice, vk::BufferUsageFlagBits::eVertexBuffer, size)
        {}
};

class IndexBuffer : public Buffer {
    public:
        IndexBuffer(vk::Device device, vk::PhysicalDevice physicalDevice, vk::DeviceSize size)
            : Buffer(device, physicalDevice, vk::BufferUsageFlagBits::eIndexBuffer, size)
        {}
};

#endif // BGL_VK_BUFFER_HPP
