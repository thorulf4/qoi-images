#include <cstdint>
#include <ranges>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <array>
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

void check_magic_bytes(uint8_t const*& data){
    if(std::memcmp(data, "qoif", 4) != 0)
        throw std::logic_error{"Magic bytes didn't match"};

    data += 4;
}

// template<typename T>
// concept Simple = requires {
//     requires alignof(T) == 1;
// };

template<typename T>
T read(uint8_t const*& data);

template<typename T>
requires (alignof(T) == 1)
T read(uint8_t const*& data){
    uint8_t const* location = data;
    data += sizeof(T);
    return *reinterpret_cast<const T*>(location);
}

template<>
Header read<Header>(uint8_t const*& data){
    Header header{};
    std::memcpy(&header, data, 10);
    data += 10;
    return header;
}

struct RGB{
    uint8_t r,g,b;

    int hash() const {
        return (r*3 + g*5 + b * 7) % 64;
    }
};
struct RGBA : public RGB{
    uint8_t a{255};
    using RGB::operator=;

    int hash() const {
        return (r*3 + g*5 + b * 7 + a*11) % 64;
    }
};

template<typename PixelType>
class Decoder{
    std::array<PixelType, 64> array{};
    std::vector<PixelType> pixels{};
    PixelType last_pixel{};
    uint32_t pixels_left;

public:
    Decoder(const Header& header){
        pixels_left = header.height * header.width;
        pixels.reserve(pixels_left);
    }

    void add_pixel(const PixelType& pixel){
        pixels.emplace_back(pixel);
        array[pixel.hash()] = pixel;
    }

    void decode(const uint8_t* data){
        for(; pixels_left > 0; --pixels_left){
            uint8_t flag = *data++;

            if(flag == 254){
                // QOI_OP_RGB
                last_pixel = read<RGB>(data);
                add_pixel(last_pixel);
            }else if(flag == 255){
                //QOI_OP_RGBA
                last_pixel = read<RGBA>(data);
                add_pixel(last_pixel);
            }else if(flag < 64){
                // QOI_OP_INDEX
                last_pixel
            }else if(flag < 128){
                // QOI_OP_DIFF
            }else if(flag < 192){
                // QOI_OP_LUMA
            }else{
                // QOI_OP_RUN
            }
        }
    }
};

void decode(const uint8_t* data){
    auto header = read<Header>(data);
    Decoder<RGBA> decoder{header};
    decoder.decode(data);
}

