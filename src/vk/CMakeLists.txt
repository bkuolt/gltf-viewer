set (CMAKE_CXX_STANDARD 20)

add_library(vk STATIC
    Buffer.cpp
    Context.cpp
    Pipeline.cpp
    Shader.cpp
)

# sdplog
find_package(spdlog REQUIRED)

# Vulkan
find_package(glslang REQUIRED)
find_package(Vulkan REQUIRED)
target_include_directories(vk PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(io PRIVATE ${Vulkan_LIBRARIES})

# GLM
target_include_directories(vk PRIVATE
    ${glm_INCLUDE_DIRS}
)

target_link_libraries(vk PRIVATE
    glslang::glslang glslang::SPIRV
)
