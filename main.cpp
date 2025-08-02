#include <glm/glm.hpp>

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_JSON
#define TINYGLTF_USE_CPP14

#include<nlohmann/json.hpp>
#include <tiny_gltf.h>


#include <iostream>

// Stub für ImageLoader Callback
bool CustomImageLoader(tinygltf::Image* image, const int image_idx,
                       std::string* err, std::string* warn,
                       int req_width, int req_height,
                       const unsigned char* bytes, int size,
                       void* user_data) {
    // Hier könnte Bilddaten verarbeitet werden
    // Für den Stub einfach true zurückgeben
    std::cout << "skipping: " << bytes << std::endl;
    return true;
}

int main() {
    // Beispiel: GLM Vektor
    glm::vec3 v(1.0f, 2.0f, 3.0f);
    std::cout << "GLM Vector: " << v.x << ", " << v.y << ", " << v.z << std::endl;

    // Beispiel: tinygltf Model laden
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err, warn;

const auto path = "/home/bastian/Desktop/glTF-Sample-Models/2.0/Sponza/glTF/Sponza.gltf";



/////////////

loader.SetImageLoader(CustomImageLoader, nullptr); // Set custom image loader

/////////////



    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    if (!warn.empty()) std::cout << "Warn: " << warn << std::endl;
    if (!err.empty()) std::cout << "Err: " << err << std::endl;
    if (!ret) {
        std::cout << "Failed to load glTF file" << std::endl;
        return 1;
    }

    std::cout << "Loaded glTF file successfully!" << std::endl;











    return 0;
}