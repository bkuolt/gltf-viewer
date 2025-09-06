#include "io/image/ImageLoader.hpp"
#include "io/gltf/Loader.hpp"

#include <future>
#include <iostream>
void load_images(tinygltf::Model& model);
void load_geometry(tinygltf::Model& model);

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_gltf_file>" << std::endl;
        return EXIT_FAILURE;
    }

    const auto path = argv[1];
    bgl::gltf::Loader loader;
    auto model = loader.load(path);


    std::async(std::launch::async, &load_images, std::ref(model));
    std::async(std::launch::async, &load_geometry, std::ref(model)); 
    // TODO: get images
    // TODO: get meshes

    return EXIT_SUCCESS;
}


void load_images(tinygltf::Model& model) {
    for (const auto& image : model.images) {
        std::cout << "Image: " << image.uri << " (" << image.width << "x" << image.height << ")" << std::endl;

        image.uri;
        image.width;
        image.height;
        image.component;
        image.bits;
        image.pixel_type;
        image.bufferView;
        image.image; // std::vector<unsigned char>

        // TODO: create vulkan image

    }
}

void load_geometry(tinygltf::Model& model) {

    model.buffers.size();

    tinygltf::Buffer& buffer = model.buffers[0];
    buffer.data.size();
    buffer.uri;
    buffer.name;
    buffer.data; // std::vector<unsigned char>
    buffer.extras;
    buffer.extensions;

    // allocate vkMemory
    // create vkBuffers

    model.meshes.size();
    for (const auto& mesh : model.meshes) {
        mesh.name;
        mesh.primitives.size();
        for (const auto& primitive : mesh.primitives) {
            primitive.attributes.size();
            primitive.indices;
            primitive.mode;
            primitive.material;
        }
    }

}