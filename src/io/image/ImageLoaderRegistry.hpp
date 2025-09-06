#include <vulkan/vulkan.hpp>
#include "ImageLoader.hpp"
#include <filesystem>
#include <map>

class ImageLoader;

namespace bgl::io {

class ImageLoaderRegistry {
 public:
    static ImageLoaderRegistry& instance();

    std::unique_ptr<ImageLoader> loader(const std::string& extension);
    void registerLoader(const std::initializer_list<std::string>& extensions, std::unique_ptr<ImageLoader> loader);

    void unregisterLoader(const std::initializer_list<std::string>& extensions);
    void unregisterLoader(const std::unique_ptr<ImageLoader>& loader);

 private:
   ImageLoaderRegistry();

    std::map<std::unique_ptr<ImageLoader>, std::initializer_list<std::string>> _loaders;
};

}