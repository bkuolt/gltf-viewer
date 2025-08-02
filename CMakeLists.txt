cmake_minimum_required(VERSION 3.21)
project(gltf_viewer LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake)
include(${CMAKE_BINARY_DIR}/Release/generators/conandeps_legacy.cmake)

set(CMAKE_BUILD_TYPE Release)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

find_package(glm REQUIRED)
# find_package(tinygltf REQUIRED)

add_executable(gltf_viewer src/main.cpp)
target_compile_features(gltf_viewer PRIVATE cxx_std_17)

#-------------------------------------
# define libgltf
#-------------------------------------
add_library(libgltf STATIC
    src/loader/Loader.cpp
)

find_package(nlohmann_json REQUIRED)
target_include_directories(libgltf PRIVATE
    ${nlohmann_json_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS}
    ${TinyGLTF_INCLUDE_DIRS}
)

target_link_libraries(libgltf PRIVATE
    nlohmann_json::nlohmann_json
    #libpng::libpng
    #libjpeg::libjpeg
    #zlib::zlib
    #zstd::zstd
)

##-------------------------------------
# use libgltf
#-------------------------------------
target_link_libraries(gltf_viewer PRIVATE libgltf nlohmann_json::nlohmann_json )
target_include_directories(gltf_viewer PRIVATE)

