#include "io/image/ImageLoader.hpp"
#include "io/gltf/Loader.hpp"

#include <future>
#include <iostream>
#include <optional>


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_gltf_file>" << std::endl;
        return EXIT_FAILURE;
    }

    // TODO: optionmal

    const auto path = argv[1];
    bgl::gltf::Loader loader;
    auto model = loader.load(path);

    return EXIT_SUCCESS;
}

