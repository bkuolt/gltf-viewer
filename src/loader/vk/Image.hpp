#include <vulkan/vulkan.hpp>

namespace bgl {

uint32_t findMemoryType(const vk::PhysicalDevice& physDev,
                        uint32_t typeFilter,
                        vk::MemoryPropertyFlags properties)
{
    vk::PhysicalDeviceMemoryProperties memProps = physDev.getMemoryProperties();

    for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memProps.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Kein passender Speichertyp gefunden!");
}


class Image {
 public:
 // TODO: add context
    Image(vk::Device device, vk::PhysicalDevice physDevice, uint32_t width, uint32_t height, vk::Format format)
        : _device(device), _format(format), _width(width), _height(height), _format(format)
    {
        _image = createImage(device, width, height, format);
        _memory = allocateMemory(_image, device, physDevice);
        device.bindImageMemory(_image, _memory, 0);
    }

    void upload(vk::Queue queue, uint32_t queueFamilyIndex,
                uint32_t width, uint32_t height, const std::vector<uint8_t>& pixels)
    {
        const vk::DeviceSize imageSize = static_cast<vk::DeviceSize>(width) * height * 4;

        vk::Buffer staging;            // TODO
        vk::DeviceMemory stagingMem;   // TODO
        _device.bindBufferMemory(staging, stagingMem, 0);

        // copy data to staging buffer
        void* p = _device.mapMemory(stagingMem, 0, imageSize);
        std::memcpy(p, pixels.data(), static_cast<size_t>(imageSize));
        _device.unmapMemory(stagingMem);

        // 2) Command Pool temporär
        vk::CommandPoolCreateInfo pci{};
        pci.flags = vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        pci.queueFamilyIndex = queueFamilyIndex;
        vk::CommandPool pool = dev.createCommandPool(pci);
        // 3) Commands: Transitions + Copy
        vk::CommandBuffer cmd = beginOneTime(dev, pool);

        // UNDEFINED -> TRANSFER_DST_OPTIMAL (falls dein Image schon in UNDEFINED ist)
        transition(cmd, _image, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, 1);


        // Copy Buffer -> Image
        vk::BufferImageCopy region {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = vk::Extent3D {0,0,0};
        region.imageExtent = vk::Extent3D {width, height, 1};

        cmd.copyBufferToImage(staging, _image,
                              vk::ImageLayout::eTransferDstOptimal,
                              1, &region);

        // TRANSFER_DST_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL
        transition(cmd, _image, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, 1);
        endOneTime(dev, queue, pool, cmd);

        // 4) Aufräumen
        dev.destroyCommandPool(pool);
        dev.destroyBuffer(staging);
        dev.freeMemory(stagingMem);
    }


    vk::ImageView createImageView() {
        return {};  // TODO
    }

 private:
    vk::Image createImage(vk::Device &device, uint32_t width, uint32_t height, vk::Format format) {
        vk::ImageCreateInfo imgInfo{};
        imgInfo.imageType   = vk::ImageType::e2D;
        imgInfo.extent      = vk::Extent3D{width, height, 1};
        imgInfo.mipLevels   = 1;
        imgInfo.arrayLayers = 1;
        imgInfo.format      = format;             // vk::Format, z.B. eR8G8B8A8Unorm
        imgInfo.tiling      = vk::ImageTiling::eOptimal;
        imgInfo.initialLayout = vk::ImageLayout::eUndefined;
        imgInfo.usage       = vk::ImageUsageFlagBits::eTransferDst |
                            vk::ImageUsageFlagBits::eSampled;
        imgInfo.sharingMode = vk::SharingMode::eExclusive;
        imgInfo.samples     = vk::SampleCountFlagBits::e1;

        return device.createImage(imgInfo);
    }

    vk::DeviceMemory allocateMemory(const vk::Image &image, vk::Device &device, vk::PhysicalDevice &physDevice) {
        vk::MemoryRequirements memReq = device.getImageMemoryRequirements(image);

        // 4. Passenden Memory-Type finden (wir wollen Device-Local)
        uint32_t memTypeIndex = findMemoryType(
            physDevice,
            memReq.memoryTypeBits,
            vk::MemoryPropertyFlagBits::eDeviceLocal
        );

        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memReq.size;
        allocInfo.memoryTypeIndex = memTypeIndex;
        return device.allocateMemory(allocInfo);
    }



    vk::Device _device;
    vk::PhysicalDevice _physDevice;
    uint32_t _width;
    uint32_t _height;
    vk::Format _format;
    vk::DeviceMemory _memory;


    vk::Image _image{nullptr};
};

// *******************************************************************************

// Helpers: einmalig irgendwo definieren
inline uint32_t findMemoryType(vk::PhysicalDevice phys, uint32_t typeBits, vk::MemoryPropertyFlags props){
    auto mp = phys.getMemoryProperties();
    for(uint32_t i=0;i<mp.memoryTypeCount;++i)
        if((typeBits&(1u<<i)) && (mp.memoryTypes[i].propertyFlags & props) == props) return i;
    throw std::runtime_error("No memory type");
}
inline vk::CommandBuffer beginOneTime(vk::Device dev, vk::CommandPool pool){
    vk::CommandBufferAllocateInfo ai{pool, vk::CommandBufferLevel::ePrimary, 1};
    auto cmd = dev.allocateCommandBuffers(ai).front();
    vk::CommandBufferBeginInfo bi{vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
    cmd.begin(bi); return cmd;
}
inline void endOneTime(vk::Device dev, vk::Queue q, vk::CommandPool pool, vk::CommandBuffer cmd){
    cmd.end();
    vk::Fence f = dev.createFence({});
    vk::SubmitInfo si{}; si.commandBufferCount=1; si.pCommandBuffers=&cmd;
    q.submit(si, f); dev.waitForFences(f, true, UINT64_MAX);
    dev.destroyFence(f); dev.freeCommandBuffers(pool, cmd);
}
inline void transition(vk::CommandBuffer cmd, vk::Image img,
                       vk::ImageLayout oldL, vk::ImageLayout newL, uint32_t mipLevels=1){
    vk::ImageMemoryBarrier b{};
    b.oldLayout=oldL; b.newLayout=newL;
    b.srcQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;
    b.dstQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;
    b.image=img;
    b.subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, mipLevels, 0, 1};

    vk::PipelineStageFlags src{}, dst{};
    if(oldL==vk::ImageLayout::eUndefined && newL==vk::ImageLayout::eTransferDstOptimal){
        b.srcAccessMask={};
        b.dstAccessMask=vk::AccessFlagBits::eTransferWrite;
        src=vk::PipelineStageFlagBits::eTopOfPipe; dst=vk::PipelineStageFlagBits::eTransfer;
    } else if(oldL==vk::ImageLayout::eTransferDstOptimal && newL==vk::ImageLayout::eShaderReadOnlyOptimal){
        b.srcAccessMask=vk::AccessFlagBits::eTransferWrite;
        b.dstAccessMask=vk::AccessFlagBits::eShaderRead;
        src=vk::PipelineStageFlagBits::eTransfer;  dst=vk::PipelineStageFlagBits::eFragmentShader;
    } else {
        throw std::runtime_error("Unsupported layout transition");
    }
    cmd.pipelineBarrier(src, dst, {}, nullptr, nullptr, b);
}


} // namespace bgl