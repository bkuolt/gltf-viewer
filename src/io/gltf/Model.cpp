#include "./../image/ImageLoader.hpp"
#include "Loader.hpp"

#include <future>
#include <iostream>
#include <tiny_gltf.h>

#include <format>
//#include <print>

void load_images(tinygltf::Model& model);
void load_geometry(tinygltf::Model& model);






///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/* ----------------------------- Texture -------------------------- */

vk::UniqueSampler create_sampler(const tinygltf::Sampler& sampler) {
    return {};  // TODO
}

vk::UniqueImage create_image(const tinygltf::Image& image) {
    return {};  // TODO
}

void create_texture(const tinygltf::Model& model, const tinygltf::Texture& texture) {
    const auto sampler { model.samplers[texture.sampler] };
    const auto image { model.images[texture.source] };
    const auto bufferView { model.bufferViews[image.bufferView] };

    create_sampler(sampler);
    create_image(image);
    // TODO: support embedded images

    // return texture
}
/* -----------------------------  -------------------------- */

vk::UniqueBuffer create_buffer(tinygltf::Buffer& buffer) {
    return {};  // TODO
}

/* ----------------------------- Mesh -------------------------- */

void create_index_buffer() {

}

void create_vertex_buffer() {

}

void create_mesh(tinygltf::Mesh mesh, tinygltf::Model model) {

    for (auto& primitive : mesh.primitives) {
        tinygltf::Accessor accessor = model.accessors[primitive.indices];
        tinygltf::BufferView bufferView = model.bufferViews[accessor.bufferView];
        auto indexBuffer = model.buffers[bufferView.buffer];
        // TODO in eigene Funktion packen

        primitive.mode;

        std::vector<tinygltf::BufferView> attributes(primitive.attributes.size());
        // TODO: for each attribute
            // TODO: handle primitive.attributes;
            // TODO: primitive.material;
            // TODO: vertex buffer
    }

    // TODO: identify index buffers
    // TODO: identify vertex buffers
}

/* ------------------------------------------------------------------------ */ 

void load_geometry(tinygltf::Model& model) {
    std::format("Model has {} meshes in {}", model.meshes.size(), model.buffers.size());


    for (auto& buffer : model.buffers) {
        create_buffer(buffer);
    }

    model.meshes.size();
    for (const auto& mesh : model.meshes) {
     //   std::cout << "Mesh: " << mesh.name << " (" << mesh.primitives       
    }

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

void load_images(tinygltf::Model& model) {
    //std::vector<Texture> textures;
    
    for (size_t i = 0; i < model.images.size(); ++i) {
        create_image(model.images[i]);
    }
}

/*******************************************************************************************/ */
class Model {};

Model loadModel() {
#if 0    
    auto imageLoad =  std::async(std::launch::async, &load_images, std::ref(model));
    auto geometryLoad = std::async(std::launch::async, &load_geometry, std::ref(model));

    // TODO: in der Zwischenzeit

    imageLoad.get();
    geometryLoad.get();
#endif
    return {};
}