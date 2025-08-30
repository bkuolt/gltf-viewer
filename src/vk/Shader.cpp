#include "Shader.hpp"

#include <vulkan/vulkan.hpp>
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <iostream>

vk::ShaderModule CreateShaderModule(
    const vk::Device& device,
    const std::vector<uint32_t>& spirvCode)
{
    vk::ShaderModuleCreateInfo shaderModuleInfo{};
    shaderModuleInfo.codeSize = spirvCode.size() * sizeof(uint32_t);
    shaderModuleInfo.pCode    = spirvCode.data();

    return device.createShaderModule(shaderModuleInfo);
}

vk::ShaderModule CreateShaderModule(
    const vk::Device& device,
    const std::string& shaderSource,
    EShLanguage stage)
{
    auto code = CompileGLSLtoSPIRV(shaderSource, EShLanguage::EShLangVertex);  // TODO: fix
    return CreateShaderModule(device, code);
}

std::vector<uint32_t> CompileGLSLtoSPIRV(const std::string& source, EShLanguage stage) {
    glslang::InitializeProcess();

    glslang::TShader shader(stage);
    const char* src = source.c_str();
    shader.setStrings(&src, 1);

    shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_2);
    shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_5);

    TBuiltInResource resources = {};
    //TODO: InitDefaultResources(resources); // musst du definieren oder kopieren

    if (!shader.parse(&resources, 100, false, EShMsgDefault))
        throw std::runtime_error(shader.getInfoLog());

    glslang::TProgram program;
    program.addShader(&shader);
    if (!program.link(EShMsgDefault))
        throw std::runtime_error(program.getInfoLog());

    std::vector<uint32_t> spirv;
    glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);

    glslang::FinalizeProcess();
    return spirv;
}

/* ---------------------------------------------------------------------- */

VkDescriptorSet createDescriptorSet(vk::Device device) {
    // 4. Descriptor Set Layout erstellen (wie vorher gezeigt)
    vk::DescriptorSetLayoutBinding uboBinding{};
    uboBinding.binding = 0;
    uboBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
    uboBinding.descriptorCount = 1;
    uboBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

    vk::DescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboBinding;

    vk::UniqueDescriptorSetLayout descriptorSetLayout = device.createDescriptorSetLayoutUnique(layoutInfo);
    std::cout << "Descriptor Set Layout created\n";

    return {};  // TODO: descriptorSetLayout.get() !?
}


VkPipelineLayout createPipelineLayout() {
    return {};  // TODO
}

// TODO: VkDescriptorSetLayout
// TODO: createPipelineLayout
// TODO: vk::DescriptorSetLayoutBinding

