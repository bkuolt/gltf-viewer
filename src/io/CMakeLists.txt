set (CMAKE_CXX_STANDARD 20)

add_library(io STATIC
    image/ImageLoader.cpp
    image/ImageLoaderRegistry.cpp
    gltf/Loader.cpp
)

target_include_directories(io
    PUBLIC
        ImageLoader.hpp
        ImageLoaderRegistry.hpp
        gltf/
)

# sdplog
find_package(spdlog REQUIRED)
target_link_libraries(io PRIVATE spdlog::spdlog)

# Vulkan
find_package(glslang REQUIRED)
find_package(Vulkan REQUIRED)
target_include_directories(io PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(io PRIVATE ${Vulkan_LIBRARIES})


find_package(TinyGLTF REQUIRED)


# GLTF
find_package(nlohmann_json REQUIRED)
target_include_directories(io PRIVATE
    ${nlohmann_json_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS}
    ${TinyGLTF_INCLUDE_DIRS}
)

#fmt
find_package(fmt)

target_link_libraries(io PRIVATE
    nlohmann_json::nlohmann_json
    glslang::glslang
    fmt::fmt
    TinyGLTF::TinyGLTF
    # TODO: fix libpng::libpng
    # TODO: fix libjpeg::libjpeg
    # TODO: fix zlib::zlib
    # TODO: fix zstd::zstd
)
