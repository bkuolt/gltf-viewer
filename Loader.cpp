#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_JSON
#define TINYGLTF_USE_CPP14

#include <nlohmann/json.hpp>
#include <tiny_gltf.h>

#include <iostream>

// Stub für ImageLoader Callback
bool CustomImageLoader(tinygltf::Image *image, const int image_idx,
                       std::string *err, std::string *warn,
                       int req_width, int req_height,
                       const unsigned char *bytes, int size,
                       void *user_data)
{
    // TODO
    std::cout << "skipping: " << size << std::endl;
    return true;
}

void LoadModel(const std::filesystem::path &path)
{
    static tinygltf::TinyGLTF loader;
    loader.SetImageLoader(CustomImageLoader, nullptr); // Set custom image loader

    /////////////

    tinygltf::Model model;
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

    std::cout << "Loaded glTF file successfully!" << std::endl;
    // return model;
    Process(std::move(model));
}

///////


void Process( tinygltf::Model model) {

    model.

}