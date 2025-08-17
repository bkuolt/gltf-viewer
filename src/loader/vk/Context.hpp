#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>

#include <spdlog/spdlog.h>
using log = spdlog;

namespace
{
    vk::UniqueInstance createInstance()
    {
        vk::ApplicationInfo appInfo{};
        appInfo.pApplicationName = "BGL Vulkan App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "BGL Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        vk::InstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.pApplicationInfo = &appInfo;

        vk::UniqueInstance instance = vk::createInstanceUnique(instanceCreateInfo);
        log::info("Vulkan instance created successfully");

        return instance;
    }

    vk::UniqueDevice createLogicalDevice(uint32_t graphicsQueueFamilyIndex) {
        const float queuePriority = 1.0f;
        vk::DeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        vk::DeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

        vk::UniqueDevice device = physicalDevice.createDeviceUnique(deviceCreateInfo);
        log::info("Logical device created successfully");

        return device;
    }
    
    PhysicalDevice getPhysicalDevice(vk::UniqueInstance& instance) {
        std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
        if (physicalDevices.empty()) {
            throw std::runtime_error("No physical devices found");
        }

        log::info("found physical devices:");
        for (const auto& device : physicalDevices) {
            log::info(device.getProperties().deviceName);
        }

        vk::PhysicalDevice physicalDevice = physicalDevices[0];
        log::info("Using GPU: {}", physicalDevice.getProperties().deviceName);

        return physicalDevice;
    }

    uint32_t findGraphicsQueueFamily(vk::PhysicalDevice& physicalDevice) {
        auto queueFamilies = physicalDevice.getQueueFamilyProperties();
        uint32_t graphicsQueueFamilyIndex = UINT32_MAX;

        for (uint32_t i = 0; i < queueFamilies.size(); i++) {
            if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
                graphicsQueueFamilyIndex = i;
                break;
            }
        }

        if (graphicsQueueFamilyIndex == UINT32_MAX) {
            std::cerr << "Keine Graphics Queue Family gefunden!\n";
            return -1;
        }
    }
}

class Context {
{
    public:
        Context() {
            auto instance = createInstance();
            auto physicalDevice = getPhysicalDevice(instance);
            auto graphicsQueueFamilyIndex = findGraphicsQueueFamily(physicalDevice);
            auto device = createLogicalDevice(graphicsQueueFamilyIndex);
        }
}