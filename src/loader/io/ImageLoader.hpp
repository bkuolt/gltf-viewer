#ifndef BGL_IMAGE_LOADER_HPP
#define BGL_IMAGE_LOADER_HPP

#include <vulkan/vulkan.hpp>
#include <filesystem>

namespace bgl::io {

using Url = std::string;

class ImageLoader {
 public:
    virtual vk::UniqueImage load(const std::filesystem::path& path) = 0;
    virtual vk::UniqueImage load(const std::vector<std::byte> data) = 0;
};

vk::UniqueImage LoadImage(const Url& url);
vk::UniqueImage LoadImage(const std::filesystem::path& path);
vk::UniqueImage LoadImage(const std::vector<std::byte>& data, const std::string& extension);

}  // namespace bgl::io

#endif // BGL_IMAGE_LOADER_HPP
