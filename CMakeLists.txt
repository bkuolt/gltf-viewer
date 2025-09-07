cmake_minimum_required(VERSION 3.21)
project(gltf_viewer LANGUAGES CXX)


# Removed redundant add_executable


include(build/conan/build/Release/generators/conan_toolchain.cmake)
include(build/conan/build/Release/generators/conandeps_legacy.cmake)

set(CMAKE_BUILD_TYPE Release)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

find_package(glm REQUIRED)

add_executable(gltf_viewer src/main.cpp)
target_compile_features(gltf_viewer PRIVATE cxx_std_23)


set_target_properties(gltf_viewer PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_subdirectory(src/io)
add_subdirectory(src/vk)

target_link_libraries(gltf_viewer PRIVATE io vk)
target_include_directories(gltf_viewer PRIVATE)

