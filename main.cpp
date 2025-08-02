#include <glm/glm.hpp>
#include<nlohmann/json.hpp>
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_JSON
#define TINYGLTF_USE_CPP14

#include <tiny_gltf.h>


#include <iostream>

int main() {
    // Beispiel: GLM Vektor
    glm::vec3 v(1.0f, 2.0f, 3.0f);
    std::cout << "GLM Vector: " << v.x << ", " << v.y << ", " << v.z << std::endl;

    // Beispiel: tinygltf Model laden
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err, warn;
    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, "model.gltf");
    if (!warn.empty()) std::cout << "Warn: " << warn << std::endl;
    if (!err.empty()) std::cout << "Err: " << err << std::endl;
    if (!ret) {
        std::cout << "Failed to load glTF file" << std::endl;
        return 1;
    }
    std::cout << "Loaded glTF file successfully!" << std::endl;
    return 0;
}