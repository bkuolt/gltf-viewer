cmake_minimum_required(VERSION 3.21)
project(gltf_viewer LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake)
include(${CMAKE_BINARY_DIR}/Release/generators/conandeps_legacy.cmake)

set(CMAKE_BUILD_TYPE Release)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

find_package(glm REQUIRED)

add_executable(gltf_viewer src/main.cpp)
target_compile_features(gltf_viewer PRIVATE cxx_std_17)

add_subdirectory(src/loader)  # cmake for libgltf 

target_link_libraries(gltf_viewer PRIVATE libgltf nlohmann_json::nlohmann_json )
target_include_directories(gltf_viewer PRIVATE)

