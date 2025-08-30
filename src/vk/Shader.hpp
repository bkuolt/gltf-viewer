#ifndef BGL_VK_SHADER_HPP
#define BGL_VK_SHADER_HPP

#include <vulkan/vulkan.hpp>
#include <glslang/Public/ShaderLang.h>
//#include <SPIRV/GlslangToSpv.h>

vk::ShaderModule  CreateShaderModule(
    const vk::Device& device,
    const std::vector<uint32_t>& spirvCode);

std::vector<uint32_t> CompileGLSLtoSPIRV(const std::string& source, EShLanguage stage);

#endif // BGL_VK_SHADER_HPP
