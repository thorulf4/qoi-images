#include <decoder.h>

#include <cstdint>
#include <ranges>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <array>
#include <vector>

int RGB::hash() const {
    return (r * 3 + g * 5 + b * 7) % 64;
}

int RGBA::hash() const {
    return (r * 3 + g * 5 + b * 7 + a*11) % 64;
}

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
    header.width = read<uint32_t>(data);
    header.height = read<uint32_t>(data);
    header.channels = read<ChannelType>(data);
    header.colorspace = read<ColorType>(data);
    return header;
}


template<typename PixelType>
class Decoder{
    std::array<PixelType, 64> array{};
    PixelType last_pixel{};
    uint32_t pixels_left;

public:
    std::vector<PixelType> pixels{};
    Decoder(const Header& header){
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

    void decode(const uint8_t* data){
        for(; pixels_left > 0; --pixels_left){
            
            uint8_t flag = *data++;

            if(flag == 254){ // QOI_OP_RGB
                add_pixel(read<RGB>(data));
            }else if(flag == 255){ //QOI_OP_RGBA
                add_pixel(read<RGBA>(data));
            }else if(flag < 64){ // QOI_OP_INDEX
                uint8_t x = flag & 63;
                add_pixel(array[x]);
            }else if(flag < 128){ // QOI_OP_DIFF
                last_pixel.r += ((flag & 48) >> 4) - 2;
                last_pixel.g += ((flag & 12) >> 2) - 2;
                last_pixel.b += (flag & 3) - 2;
                copy_pixel();
            }else if(flag < 192){ // QOI_OP_LUMA
                uint8_t dg = (flag & 63) - 32;
                uint8_t luma = *data++;
                uint8_t db_dg = (luma & 15u) - 8;
                uint8_t dr_dg = ((luma & 240u) >> 4) - 8;
                
                last_pixel.r += dr_dg + dg;
                last_pixel.g += dg;
                last_pixel.b += db_dg + dg;
                copy_pixel();
            }else{ // QOI_OP_RUN
                uint8_t x = flag & 63;
                pixels_left -= (x - 1);
                for(; x > 0; --x)
                    pixels.emplace_back(last_pixel);
            }
        }
    }
};

Image decode(const uint8_t* data){
    auto header = read<Header>(data);
    Decoder<RGBA> decoder{header};
    decoder.decode(data);
    return {header, std::move(decoder.pixels)};
}

