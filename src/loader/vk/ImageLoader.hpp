#include <png++/png.hpp>
#include <cstdint>

extern "C" {
#include <jpeglib.h>
}
  Image loadJPEG_to_RGBA8(const std::string& path);

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


inline std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return (char)std::tolower(c); });
    return s;
}
inline bool endsWith(const std::string& s, const std::string& suf) {
    return s.size() >= suf.size() && std::equal(suf.rbegin(), suf.rend(), s.rbegin());
}

    ////////
Image loadImageRGBA8(const std::string& path) {
    const std::string p = toLower(path);
    if (endsWith(p, ".png"))  return loadPNG_RGBA8(path);
    if (endsWith(p, ".jpg") || endsWith(p, ".jpeg") || endsWith(p, ".jfif")) return loadJPEG_to_RGBA8(path);
    throw std::runtime_error("Unbekanntes Bildformat (unterstützt: .png, .jpg, .jpeg, .jfif): " + path);
}



  Image loadJPEG_to_RGBA8(const std::string& path) {
        // libjpeg liest nach RGB (kein Alpha) -> wir konvertieren zu RGBA mit A=255
        FILE* fp = std::fopen(path.c_str(), "rb");
        if (!fp) throw std::runtime_error("Konnte JPEG-Datei nicht öffnen: " + path);

        jpeg_decompress_struct cinfo{};
        jpeg_error_mgr jerr{};
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);

        jpeg_stdio_src(&cinfo, fp);
        if (jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK) {
            jpeg_destroy_decompress(&cinfo);
            std::fclose(fp);
            throw std::runtime_error("Ungültiger JPEG-Header: " + path);
        }

        // Farbraum auf RGB zwingen
        cinfo.out_color_space = JCS_RGB;
        jpeg_start_decompress(&cinfo);

        const uint32_t width  = static_cast<uint32_t>(cinfo.output_width);
        const uint32_t height = static_cast<uint32_t>(cinfo.output_height);
        const int comps = cinfo.output_components; // sollte 3 sein (RGB)

        if (comps != 3) {
            // libjpeg kann z.B. 1 (Graustufen) liefern – behandeln wir trotzdem generisch
        }

        // Zeilenpuffer für libjpeg (RGB)
        const size_t rowStrideRGB = static_cast<size_t>(width) * comps;
        std::vector<uint8_t> rowBuf(rowStrideRGB);
        JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, rowStrideRGB, 1);

        LoadedImageRGBA8 out;
        out.width = width;
        out.height = height;
        out.pixels.resize(static_cast<size_t>(width) * height * 4);

        size_t y = 0;
        while (cinfo.output_scanline < cinfo.output_height) {
            jpeg_read_scanlines(&cinfo, buffer, 1);
            std::memcpy(rowBuf.data(), buffer[0], rowStrideRGB);

            uint8_t* dst = out.pixels.data() + static_cast<size_t>(y) * width * 4;
            const uint8_t* src = rowBuf.data();
            for (uint32_t x = 0; x < width; ++x) {
                uint8_t r, g, b;
                if (comps == 3) { r = *src++; g = *src++; b = *src++; }
                else if (comps == 1) { r = g = b = *src++; } // Grayscale → RGB
                else {
                    // Unerwartet (z.B. CMYK). In der Praxis: cinfo.out_color_space setzen & erneut versuchen.
                    jpeg_finish_decompress(&cinfo);
                    jpeg_destroy_decompress(&cinfo);
                    std::fclose(fp);
                    throw std::runtime_error("Nicht unterstützte JPEG-Komponenten: " + std::to_string(comps));
                }
                *dst++ = r; *dst++ = g; *dst++ = b; *dst++ = 255;
            }
            ++y;
        }

        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        std::fclose(fp);
        return out;
    }