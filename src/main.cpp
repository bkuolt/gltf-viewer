#include "loader/Loader.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_gltf_file>" << std::endl;
        return EXIT_FAILURE;
    }

    const auto path = argv[1];
    bgl::gltf::LoadModel(path);
    return EXIT_SUCCESS;
}