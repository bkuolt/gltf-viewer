#include <png++/png.hpp>

// RGBA8
struct Image {
    uint32_t width = 0;
    uint32_t height = 0;
    std::vector<uint8_t> pixels;
};

Image loadImageRGBA8(const std::string& path) {
    const std::string p = toLower(path);
    if (endsWith(p, ".png"))  return loadPNG_RGBA8(path);
    if (endsWith(p, ".jpg") || endsWith(p, ".jpeg") || endsWith(p, ".jfif")) return loadJPEG_to_RGBA8(path);
    throw std::runtime_error("Unbekanntes Bildformat (unterstützt: .png, .jpg, .jpeg, .jfif): " + path);
}

Image loadPNG_RGBA8(const std::string& path) {
        png::image<png::rgba_pixel> img(path); // png++ wirft exceptions bei Fehlern
        Image out;
        out.width  = static_cast<uint32_t>(img.get_width());
        out.height = static_cast<uint32_t>(img.get_height());
        out.pixels.resize(static_cast<size_t>(out.width) * out.height * 4);

        // Zeilenweise kopieren
        for (uint32_t y = 0; y < out.height; ++y) {
            auto const& row = img.get_row(y);
            uint8_t* dst = out.pixels.data() + static_cast<size_t>(y) * out.width * 4;
            for (uint32_t x = 0; x < out.width; ++x) {
                const png::rgba_pixel& p = row[x];
                *dst++ = p.red;
                *dst++ = p.green;
                *dst++ = p.blue;
                *dst++ = p.alpha;
            }
        }
        return out;
    }
