#ifndef BGL_GLT_LOADER_HPP
#define BGL_GLT_LOADER_HPP

#include <filesystem>

namespace bgl::gltf {

void LoadModel(const std::filesystem::path& path);

}  // namespace bgl::gltf

#endif // BGL_GLT_LOADER_HPP