//--------------------------
// TODO: move in cmake 

// tinygltf config
#define TINYGLTF_IMPLEMENTATION

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE

#define TINYGLTF_NO_INCLUDE_JSON
#include <nlohmann/json.hpp>



#include <tiny_gltf.h>
namespace gltf = tinygltf; 
//--------------------------

#include <iostream>
#include <filesystem>
#include <glm/glm.hpp>

namespace {

// Stub für ImageLoader Callback
bool CustomImageLoader(gltf::Image *image, const int image_idx,
                       std::string *err, std::string *warn,
                       int req_width, int req_height,
                       const unsigned char *bytes, int size,
                       void *user_data)
{
    // TODO
    std::cout << "skipping: " << size << std::endl;
    return true;
}

void printStats(const gltf::Model &model) {
    std::cout << "Number of meshes: " << model.meshes.size() << std::endl
              << "Number of materials: " << model.materials.size() << std::endl
              << "Number of nodes: " << model.nodes.size() << std::endl
              << "Number of animations: " << model.animations.size() << std::endl;                                   


// TODO: print stats

}

void Process(const gltf::Model& model) {
    printStats(model);

    // TODO:
    // TODO:
    // TODO:
}
// ---------------------------

gltf::TinyGLTF& GetLoader() {
    static tinygltf::TinyGLTF loader;
    loader.SetImageLoader(CustomImageLoader, nullptr); // Set custom image loader
    return loader;
}

gltf::Model GetModel(const std::filesystem::path &path) {
    auto loader = GetLoader();

    gltf::Model model;
    std::string err, warn;
    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);

    if (!warn.empty())
        std::cout << "Warn: " << warn << std::endl;
    if (!err.empty())
        std::cout << "Err: " << err << std::endl;
    if (!ret)
    {
        std::cout << "Failed to load glTF file" << std::endl;
        // return 1;
    }

    return model;
}

}  // namespace


namespace bgl::gltf {

void LoadModel(const std::filesystem::path &path)
{
    auto model = GetModel(path);
    Process(std::move(model));
    std::cout << "Loaded glTF file successfully!" << std::endl;
}

}  // namespace bgl::gltf
