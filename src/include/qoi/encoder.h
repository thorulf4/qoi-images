#pragma once
#include <qoi/qoi.h>

#include <algorithm>
#include <cstring>
#include <array>
#include <bit>
#include <ranges>
#include <iterator>

namespace qoi{

template<typename OutputIterator>
void write_repeat(OutputIterator& it, uint32_t times){
    while(times > 62){
        *it++ = 253;
        times -= 62;
    }
    *it++ = 192 | (times-1);
}

template<std::endian endianess = std::endian::little, class ValueType>
void write(std::output_iterator<uint8_t> auto& it, const ValueType& value) {
    auto values = std::bit_cast<std::array<uint8_t, sizeof(ValueType)>>(value);
    if constexpr(endianess == std::endian::native)
        std::copy_n(values.begin(), sizeof(ValueType), it);
    else
        std::copy_n(values.rbegin(), sizeof(ValueType), it);
}

template<typename PixelType>
constexpr bool alpha_check(const PixelType& pixel, const PixelType& other){
    if constexpr(std::is_same_v<PixelType, RGBA>)
        return pixel.a == other.a;
    else
        return true;
}

template<std::ranges::range PixelRange, typename OutputIterator, typename PixelType = std::ranges::range_value_t<PixelRange>>
void encode(const Header& header, const PixelRange& pixels, OutputIterator it){
    std::copy_n("qoif", 4, it);
    write<std::endian::big>(it, header.width);
    write<std::endian::big>(it, header.height);
    *it++ = static_cast<uint8_t>(header.channels);
    *it++ = static_cast<uint8_t>(header.colorspace);
    
    std::array<PixelType, 64> memory;
    constexpr static RGBA default_pixel = RGBA{0,0,0,255};
    const PixelType* last_pixel = &default_pixel;

    uint32_t repetition_count = 0;
    for(const PixelType& pixel : pixels){
        if(*last_pixel == pixel){
            repetition_count++;
            continue;
        }else if(repetition_count > 0){
            write_repeat(it, repetition_count);
            repetition_count = 0;
        }
        uint8_t hash = pixel.hash();

        if(memory[hash] == pixel){
            *it++ = hash; // QOI_OP_INDEX
            goto next;
        }else{
            memory[hash] = pixel;
        }

        if(alpha_check<PixelType>(pixel, *last_pixel)){
            auto diff_r = static_cast<int32_t>(pixel.r) - static_cast<int32_t>(last_pixel->r);
            auto diff_g = static_cast<int32_t>(pixel.g) - static_cast<int32_t>(last_pixel->g);
            auto diff_b = static_cast<int32_t>(pixel.b) - static_cast<int32_t>(last_pixel->b);
            if(diff_r >= -2 && diff_r <= 1 && diff_g >= -2 && diff_g <= 1 && diff_b >= -2 && diff_b <= 1){
                *it++ = 64 | ((diff_r+2) << 4) | ((diff_g+2) << 2) | (diff_b+2); // QOI_OP_DIFF
                goto next;
            }

            if(diff_g >= -32 && diff_g <= 31){
                auto dr_dg = diff_r - diff_g;
                auto db_dg = diff_b - diff_g;
                if(dr_dg >= -8 && dr_dg <= 7 && db_dg >= -8 && db_dg <= 7){
                    *it++ = 128 | (diff_g + 32);
                    *it++ = ((dr_dg + 8) << 4) | (db_dg + 8); // QOI_OP_LUMA
                    goto next;
                }
            }

            *it++ = 254; // QOI_OP_RGB
            write(it, static_cast<const RGB&>(pixel));
        }else if constexpr(std::is_same_v<PixelType, RGBA>){
            *it++ = 255; // QOI_OP_RGBA
            write(it, static_cast<const RGBA&>(pixel));
        }
        next:
        last_pixel = &pixel;
    }

    if(repetition_count > 0)
        write_repeat(it, repetition_count); // QOI_OP_RUN
    std::copy_n("\0\0\0\0\0\0\0\1", 8, it);
}

}