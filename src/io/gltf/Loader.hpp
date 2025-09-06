#ifndef BGL_GLT_LOADER_HPP
#define BGL_GLT_LOADER_HPP

#include <filesystem>

// TODO: move in cmake 
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#include <tiny_gltf.h>
//sudo apt install libtinygltf-dev

namespace bgl::gltf {

class Loader {
public:
    Loader();
    Loader(const std::filesystem::path &path);

    tinygltf::Model load(const std::filesystem::path &path);

private:
    static bool ImageLoaderCallback(tinygltf::Image *image, const int image_idx,
                                    std::string *err, std::string *warn,
                                    int req_width, int req_height,
                                    const unsigned char *bytes, int size,
                                    void *user_data);
                                
    tinygltf::TinyGLTF _loader;
};



}  // namespace bgl::gltf

#endif // BGL_GLT_LOADER_HPP