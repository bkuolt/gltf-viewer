#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>

#include <spdlog/spdlog.h>
using log = spdlog;

namespace
{

    // --- Callback Funktion für Validation Layer ---
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    vk::DebugUtilsMessengerCreateInfoEXT createDebugMessengerCreateInfo() {
        vk::DebugUtilsMessengerCreateInfoEXT& createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;

        return createInfo;    
    }

    vk::ApplicationInfo createApplicationInfo() {
        vk::ApplicationInfo appInfo{};
        appInfo.pApplicationName = "BGL Vulkan App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "BGL Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;
        return appInfo;
    }

    vk::InstanceCreateInfo createInstanceCreateInfo(vk::ApplicationInfo& appInfo) {
        const bool enableValidationLayers { true };

        const std::vector<const char*> extensions {
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        };

        vk::InstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
            log::info("Enabled extensions: {}", std::string::join(extensions, ", "));

        if (enableValidationLayers) {
            const std::vector<const char*> validationLayers {
                "VK_LAYER_KHRONOS_validation"
            };

            auto debugCreateInfo = createDebugMessengerCreateInfo();
            instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
            instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

            log::info("Enabled validation layers: {}", std::string::join(validationLayers, ", "));
        } else {
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
        }
    }

    vk::UniqueInstance createInstance()
    {
        auto appInfo = createApplicationInfo();
        auto instanceCreateInfo = createInstanceCreateInfo(appInfo);
        vk::UniqueInstance instance = vk::createInstanceUnique(instanceCreateInfo);
        log::info("Vulkan instance created successfully");
        return instance;
    }

    /* ------------------------------------------------------------------------- */

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

    /* ------------------------------------------------------------------------- */

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
            _instance = createInstance();
            _physicalDevice = getPhysicalDevice(_instance);
            _graphicsQueueFamilyIndex = findGraphicsQueueFamily(_physicalDevice);
            _device = createLogicalDevice(_graphicsQueueFamilyIndex);
        }

    private:
        vk::UniqueInstance _instance;
        vk::PhysicalDevice _physicalDevice;
        vk::UniqueDevice _device;
        uint32_t _graphicsQueueFamilyIndex;
}