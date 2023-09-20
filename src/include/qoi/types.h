#pragma once
#include <cstdint>

namespace qoi{

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
    static constexpr ChannelType channel = ChannelType::RGB;
    bool operator==(const RGB&) const = default;
    int hash() const {
        return (r * 3 + g * 5 + b * 7) % 64;
    }
};
struct RGBA : RGB{
    uint8_t a{255};
    static constexpr ChannelType channel = ChannelType::RGBA;
    bool operator==(const RGBA&) const = default;
    using RGB::operator=;
    int hash() const{
        return (r * 3 + g * 5 + b * 7 + a * 11) % 64;
    }
};

};