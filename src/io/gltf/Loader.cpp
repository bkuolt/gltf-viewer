#include <iostream>
#include <filesystem>
#include <glm/glm.hpp>

#include "Loader.hpp"

#include <filesystem>

// TODO: move in cmake 
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
// #define TINYGLTF_NO_INCLUDE_JSON
// #include <nlohmann/json.hpp>
#include <tiny_gltf.h>





namespace bgl::gltf {

Loader::Loader() {
    _loader.SetImageLoader(ImageLoaderCallback, nullptr); // Set custom image loader
}

Loader::Loader(const std::filesystem::path &path) : Loader(){
    (path);
}

tinygltf::Model Loader::load(const std::filesystem::path &path) {
    std::string error, warning;
    tinygltf::Model model;
    bool successful = _loader.LoadASCIIFromFile(&model, &error, &warning, path);

    if (!warning.empty())
        std::cout << "Warning: " << warning << std::endl;
    if (!error.empty())
        std::cout << "Error: " << error << std::endl;
    if (!successful)
    {
        // TODO: incoporate error and warning messages
        throw std::runtime_error("Failed to load glTF file");
    }

    return model;
}

bool Loader::ImageLoaderCallback(tinygltf::Image *image, const int image_idx,
                                 std::string *err, std::string *warn,
                                 int req_width, int req_height,
                                 const unsigned char *bytes, int size,
                                 void *user_data)
{
    // TODO: PNG
    // TODO: JPG
    // TODO: KTX
    std::cout << "skipping: " << "(" << req_width <<"x"<< req_height << ")"<< std::endl;
    return true;
}

}  // namespace bgl::gltf

namespace {

void printStats(const tinygltf::Model &model) {
    std::cout << "Number of meshes: " << model.meshes.size() << std::endl
              << "Number of materials: " << model.materials.size() << std::endl
              << "Number of nodes: " << model.nodes.size() << std::endl
              << "Number of animations: " << model.animations.size() << std::endl;                                   

    // TODO: print stats
}

} // namespace
