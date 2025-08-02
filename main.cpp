#include <glm/glm.hpp>
#include <iostream>
#include <filesystem>


void LoadModel(const std::filesystem::path& path);

int main(int argc, char** argv) {

    const auto path = argv[1];
    LoadModel(path);

    return 0;
}