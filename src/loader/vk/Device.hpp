#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>

int createvkInstance() {
    // 1. Vulkan Instanz erstellen
    vk::ApplicationInfo appInfo{};
    appInfo.pApplicationName = "My Vulkan App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    vk::InstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.pApplicationInfo = &appInfo;

    vk::UniqueInstance instance = vk::createInstanceUnique(instanceCreateInfo);
    std::cout << "Vulkan instance created\n";

    // 2. Physical Device auswählen (erste GPU)
    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
    if (physicalDevices.empty()) {
        std::cerr << "Keine GPU gefunden!\n";
        return -1;
    }
    vk::PhysicalDevice physicalDevice = physicalDevices[0];
    std::cout << "Using GPU: " << physicalDevice.getProperties().deviceName << "\n";

    // 3. Logical Device und Queue erstellen
    // Finde eine Queue Family, die Graphics unterstützt
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

    float queuePriority = 1.0f;
    vk::DeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    vk::DeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

    vk::UniqueDevice device = physicalDevice.createDeviceUnique(deviceCreateInfo);
    std::cout << "Logical device created\n";

    // 4. Descriptor Set Layout erstellen (wie vorher gezeigt)
    vk::DescriptorSetLayoutBinding uboBinding{};
    uboBinding.binding = 0;
    uboBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
    uboBinding.descriptorCount = 1;
    uboBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

    vk::DescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboBinding;

    vk::UniqueDescriptorSetLayout descriptorSetLayout = device->createDescriptorSetLayoutUnique(layoutInfo);
    std::cout << "Descriptor Set Layout created\n";

    // Clean up automatically durch UniqueHandles

    return 0;
}