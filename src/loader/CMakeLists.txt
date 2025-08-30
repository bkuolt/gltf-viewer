add_library(libgltf STATIC
    Loader.cpp
    vk/Model.cpp
    vk/Pipeline.cpp
    vk/Shader.cpp
    vk/Context.hpp
    vk/Buffer.hpp
    vk/Image.hpp
)

set (CMAKE_CXX_STANDARD 20)

# sdplog
find_package(spdlog REQUIRED)
target_link_libraries(libgltf PRIVATE spdlog::spdlog)

find_package(Vulkan REQUIRED)
find_package(glslang REQUIRED)

# Vulkan Libs und Include hinzufügen
target_include_directories(libgltf PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(libgltf PRIVATE ${Vulkan_LIBRARIES})

find_package(nlohmann_json REQUIRED)
target_include_directories(libgltf PRIVATE
    ${nlohmann_json_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS}
    ${TinyGLTF_INCLUDE_DIRS}
)

target_link_libraries(libgltf PRIVATE
    nlohmann_json::nlohmann_json
    glslang::glslang glslang::SPIRV
    #libpng::libpng
    #libjpeg::libjpeg
    #zlib::zlib
    #zstd::zstd
)
