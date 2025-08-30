#include <png++/png.hpp>

#include "ImageLoader.hpp"

namespace bgl::io {

class PNGLoader : public ImageLoader {
    public:
    Image load(const std::filesystem::path& path) override {
        // TODO
    }
};

}  // namespace bgl::io
