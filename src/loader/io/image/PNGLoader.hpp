#include <png++/png.hpp>

#include "ImageLoader.hpp"

namespace bgl::io {

class PNGLoader : public ImageLoader {
    public:
    Image load(const std::filesystem::path& path) override {

        png::image<png::rgba_pixel> image(path); // png++ wirft exceptions bei Fehlern
        Image imageData;
        imageData.width  = static_cast<uint32_t>(image.get_width());
        imageData.height = static_cast<uint32_t>(image.get_height());
        imageData.pixels.resize(static_cast<size_t>(imageData.width) * imageData.height * 4);

        for (uint32_t y = 0; y < imageData.height; ++y) {
            auto const& row = image.get_row(y);

            uint8_t* dst = imageData.pixels.data() + static_cast<size_t>(y) * imageData.width * 4;
            for (uint32_t x = 0; x < imageData.width; ++x) {
                const png::rgba_pixel& p = row[x];
                *dst++ = p.red;
                *dst++ = p.green;
                *dst++ = p.blue;
                *dst++ = p.alpha;
            }
        }

        return imageData;
    }
};

}  // namespace bgl::io