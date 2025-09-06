#include "io/image/ImageLoader.hpp"
#include "io/gltf/Loader.hpp"

#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_gltf_file>" << std::endl;
        return EXIT_FAILURE;
    }

    const auto path = argv[1];
    bgl::gltf::Loader loader;
    auto model = loader.ĺoad(path);

    // TODO: get images
    // TODO: get meshes

    return EXIT_SUCCESS;
}