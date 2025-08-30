#include "ImageLoaderRegistry.hpp"
#include <spdlog/spdlog.h>
using log = spdlog;

namespace bgl::io {


ImageLoaderRegistry& ImageLoaderRegistry::instance() {
    static ImageLoaderRegistry registry;
    return registry;  // TODO: proper implementation
}

std::unique_ptr<ImageLoader> ImageLoaderRegistry::loader(const std::string& extension)
{
    return _loaders.find(extension);  // TODO: proper implementation
}

void ImageLoaderRegistry::registerLoader(const std::initializer_list<std::string>& extensions, std::unique_ptr<ImageLoader> loader)
{
    _loaders.insert(std::move(loader), extensions);
    log::info("Registered image loader for extensions: {}", fmt::join(extensions, ", "));
}

void ImageLoaderRegistry::unregisterLoader(const std::unique_ptr<ImageLoader>& loader)
{
    const auto extensions = _loaders.at(loader);
    _loaders.erase(loader);
    log::info("Unregistered image loader for extensions: {}", fmt::join(extensions, ", "));
}

void ImageLoaderRegistry::unregisterLoader(const std::initializer_list<std::string>& extensions)
{
    // TODO
}

}  // namespace bgl::io