#ifndef BGL_IMAGE_LOADER_HPP
#define BGL_IMAGE_LOADER_HPP

#include <vulkan/vulkan.hpp>
#include <filesystem>

namespace bgl::io {

using Url = std::string;

// RGBA8
struct Image {
    uint32_t width = 0;
    uint32_t height = 0;
    std::vector<uint8_t> pixels;
};

class ImageLoader {
 public:
    ImageLoader(const std::filesystem::path& path) {}

    virtual Image load(const std::filesystem::path& path) = 0;
    virtual Image load(const std::vector<std::byte> data) = 0;
};

vk::UniqueImage LoadImage(const Url& url);
vk::UniqueImage LoadImage(const std::filesystem::path& path);
vk::UniqueImage LoadImage(const std::vector<std::byte>& data, const std::string& extension);

}  // namespace bgl::io

#endif // BGL_IMAGE_LOADER_HPP
