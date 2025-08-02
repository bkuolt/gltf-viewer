cmake_minimum_required(VERSION 3.21)
project(MyGltfViewer LANGUAGES CXX)


set(CMAKE_BUILD_TYPE Release)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")


add_executable(MyGltfViewer
main.cpp
)
find_package(glm REQUIRED)

# find_package(tinygltf REQUIRED)

# Create wrapper target for tinygltf

# Conan generiert Toolchain + Abhängigkeiten in build-Verzeichnis
include(${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake)
include(${CMAKE_BINARY_DIR}/Release/generators/conandeps_legacy.cmake)

message("!!!!!!!!!!!!!!!!!")
message(${TinyGLTF_INCLUDE_DIR})

find_package(nlohmann_json REQUIRED)
target_include_directories(MyGltfViewer PRIVATE ${nlohmann_json_INCLUDE_DIRS})




# Optional: Wenn tinygltf.cc nicht automatisch dabei ist (häufig der Fall!)
# dann holen wir uns den Pfad aus Conan:
get_target_property(TinyGLTF_INCLUDE_DIR TinyGLTF::TinyGLTF INTERFACE_INCLUDE_DIRECTORIES)

# Suche nach tiny_gltf.cc (ja, dirty hacky, aber funktioniert)
file(GLOB TinyGLTF_CC "${TinyGLTF_INCLUDE_DIR}/tiny_gltf.cc")

# Wenn gefunden, hinzufügen:
if (TinyGLTF_CC)
    message(STATUS "Found tiny_gltf.cc: ${TinyGLTF_CC}")
    target_sources(MyGltfViewer PRIVATE ${TinyGLTF_CC})
endif()














#target_link_libraries(MyGltfViewer PRIVATE TinyGLTF:TinyGLTF)

target_include_directories(MyGltfViewer PRIVATE
    ${glm_INCLUDE_DIRS} ${TinyGLTF_INCLUDE_DIRS}
)


target_compile_features(MyGltfViewer PRIVATE cxx_std_17)
