#pragma once
#include <cstdint>
#include <vector>

enum class ChannelType : uint8_t {
    RGB = 3,
    RGBA = 4
};

enum class ColorType : uint8_t{
    SRGB = 0, // https://en.wikipedia.org/wiki/SRGB linear alpha
    RGB = 1 // all channels linear
};

struct Header{
    uint32_t width;
    uint32_t height;
    ChannelType channels;
    ColorType colorspace;
};

struct RGB{
    uint8_t r,g,b;
    int hash() const;
};
struct RGBA : RGB{
    uint8_t a{255};
    using RGB::operator=;
    int hash() const;
};

struct Image{
    Header header;
    std::vector<RGBA> pixels;
};

Image decode(const uint8_t* data);