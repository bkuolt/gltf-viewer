#include "ImageLoaderRegistry.hpp"
#include <iostream>
#include <fmt/format.h>

namespace bgl::io {

ImageLoaderRegistry::ImageLoaderRegistry() {

    // TODO: register png
    // TODO: register jpg
}

ImageLoaderRegistry& ImageLoaderRegistry::instance() {
    static ImageLoaderRegistry registry;
    return registry;  // TODO: proper implementation
}

std::unique_ptr<ImageLoader> ImageLoaderRegistry::loader(const std::string& extension)
{
    for (const auto& [loader, extensions] : _loaders) {
        if (std::find(extensions.begin(), extensions.end(), extension) != extensions.end()) {
            //TODO
            //return std::make_unique<ImageLoader>(*loader);
        }
    }
    return nullptr;
}

void ImageLoaderRegistry::registerLoader(const std::initializer_list<std::string>& extensions, std::unique_ptr<ImageLoader> loader)
{
    _loaders.insert(std::make_pair(std::move(loader), extensions));
   //std::cout << "Registered image loader for extensions: {}" << fmt::join(extensions, ", ");
}

void ImageLoaderRegistry::unregisterLoader(const std::unique_ptr<ImageLoader>& loader)
{
    const auto extensions = _loaders.at(loader);
    _loaders.erase(loader);
    //std::cout << "Unregistered image loader for extensions: {}" << fmt::join(extensions, ", ");
}

void ImageLoaderRegistry::unregisterLoader(const std::initializer_list<std::string>& extensions)
{
    // TODO
}

}  // namespace bgl::io