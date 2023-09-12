#include <qoi/decoder.h>

#include <cstdint>
#include <ranges>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <array>
#include <vector>

void check_magic_bytes(uint8_t const*& data){
    if(std::memcmp(data, "qoif", 4) != 0)
        throw std::logic_error{"Magic bytes didn't match"};
    data += 4;
}

template<typename T>
T read(uint8_t const*& data);

template<typename T>
requires (alignof(T) == 1 || alignof(T) == sizeof(T))
T read(uint8_t const*& data){
    uint8_t const* location = data;
    data += sizeof(T);
    return *reinterpret_cast<const T*>(location);
}

template<>
Header read<Header>(uint8_t const*& data){
    Header header{};
    check_magic_bytes(data);
    header.width = __builtin_bswap32(read<uint32_t>(data));
    header.height = __builtin_bswap32(read<uint32_t>(data));
    header.channels = read<ChannelType>(data);
    header.colorspace = read<ColorType>(data);
    return header;
}

#include <iostream>

template<typename PixelType>
class Decoder{
    std::array<PixelType, 64> array{};
    PixelType last_pixel{};
    int64_t pixels_left;

public:
    std::vector<PixelType> pixels{};
    Decoder(const Header& header){
        last_pixel = RGB{0,0,0};
        pixels_left = header.height * header.width;
        pixels.reserve(pixels_left);
    }

    template<typename T>
    void add_pixel(const T& pixel){
        last_pixel = pixel;
        copy_pixel();
    }
    
    void copy_pixel(){
        pixels.emplace_back(last_pixel);
        array[last_pixel.hash()] = last_pixel;
    }

    void decode(const uint8_t* data, const uint8_t* end){
        const auto* start = data;
        while(pixels.size() < pixels_left){
            int pixel = pixels.size();
            uint8_t flag = *data++;
            if(flag == 254){ // QOI_OP_RGB
                add_pixel(read<RGB>(data));
            }else if(flag == 255){ //QOI_OP_RGBA
                add_pixel(read<RGBA>(data));
            }else if(flag < 64){ // QOI_OP_INDEX
                add_pixel(array[flag]);
            }else if(flag < 128){ // QOI_OP_DIFF
                last_pixel.r += ((flag >> 4) & 3u) - 2;
                last_pixel.g += ((flag >> 2) & 3u) - 2;
                last_pixel.b += ( flag       & 3u) - 2;
                copy_pixel();
            }else if(flag < 192){ // QOI_OP_LUMA                
                int luma = *data++;
                
                int dg = (flag & 63u) - 32;
                last_pixel.r += dg - 8 + ((luma >> 4) & 15u);
                last_pixel.g += dg;
                last_pixel.b += dg - 8 + (luma & 15u);
                copy_pixel();
            }else{ // QOI_OP_RUN
                int32_t x = (flag & 63) + 1;
                while(x-->0)
                    pixels.emplace_back(last_pixel);
            }
        }
        if(std::memcmp(data, "\0\0\0\0\0\0\0\1", 8) != 0)
            throw std::logic_error{"Input didn't contain end padding"};
    }
};

Image decode(const uint8_t* data, const uint8_t* end){
    auto header = read<Header>(data);
    Decoder<RGBA> decoder{header};
    decoder.decode(data, end);
    return {header, std::move(decoder.pixels)};
}

