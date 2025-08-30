#include "ImageLoader.hpp"
#include "ImageLoaderRegistry.hpp"

namespace bgl::io {

namespace {

std::vector<std::byte> download(const Url& url) {

    throw std::runtime_error("not implemented");  // TODO
}

}  // namespace

vk::UniqueImage LoadImage(const std::filesystem::path& path) {
    auto loader { ImageLoaderRegistry::instance().loader(path.extension()) };
    
    const auto imageData { loader->load(path) };
    return {};  // TODO: refactor so that a bgl::Image is returned and uploaded to the gpu
}

vk::UniqueImage LoadImage(const std::vector<std::byte>& data, const std::string& extension) {
    auto loader { ImageLoaderRegistry::instance().loader(extension) };
    return loader->load(data);
}

vk::UniqueImage LoadImage(const Url& url) {
    const auto buffer { download(url) };
    const auto extension { "" };  // TODO: extract from url
    return LoadImage(buffer, extension);
}

}  // namespace bgl::io