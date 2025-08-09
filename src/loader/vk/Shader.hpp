#include <vulkan/vulkan.hpp>

#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>

/*
PipelineShaderStageCreateInfo
    vkShaderModule (2x)

vkPipelineLayout
    vk::DescriptorSetLayoutBinding
    vkDescriptorSetLayout  
*/

std::vector<uint32_t> compileGLSLtoSPIRV(const std::string& source, EShLanguage stage);

vk::ShaderModule  CreateShaderModule(
    const vk::Device& device,
    const std::vector<uint32_t>& spirvCode)
{
    vk::ShaderModuleCreateInfo shaderModuleInfo{};
    shaderModuleInfo.codeSize = spirvCode.size() * sizeof(uint32_t);
    shaderModuleInfo.pCode    = spirvCode.data();

    return device.createShaderModule(shaderModuleInfo);
}

vk::ShaderModule  CreateShaderModule(
    const vk::Device& device,
    const std::string& shaderSource,
    EShLanguage stage = EShLanguage::EShLangVertex)
{
    auto code = compileGLSLtoSPIRV(shaderSource);
    return CreateShaderModule(device, code);
}

std::array<vk::PipelineShaderStageCreateInfo, 2> CreatePipelineShaderStageCreateInfo(
    vk::ShaderModule vertShaderModule,
    vk::ShaderModule fragShaderModule)
{
    vk::PipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.stage  = vk::ShaderStageFlagBits::eVertex;
    vertShaderStageInfo.module = vertShaderModule; // dein Vertex-Shader-Module
    vertShaderStageInfo.pName  = "main"; // Entry Point im Shader

    vk::PipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.stage  = vk::ShaderStageFlagBits::eFragment;
    fragShaderStageInfo.module = fragShaderModule; // dein Fragment-Shader-Module
    fragShaderStageInfo.pName  = "main";

    // Später in der Pipeline-Erstellung:
    return { vertShaderStageInfo, fragShaderStageInfo };
}

std::vector<uint32_t> compileGLSLtoSPIRV(const std::string& source, EShLanguage stage) {
    glslang::InitializeProcess();

    glslang::TShader shader(stage);
    const char* src = source.c_str();
    shader.setStrings(&src, 1);

    shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_2);
    shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_5);

    TBuiltInResource resources = {};
    InitDefaultResources(resources); // musst du definieren oder kopieren

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