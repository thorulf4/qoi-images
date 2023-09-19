#pragma once
#include <qoi/qoi.h>

#include <vector>
#include <cstdint>
#include <ranges>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <array>
#include <vector>
#include <bit>
#include <iterator>
#include <utility>

namespace qoi {

template<typename IterType, std::size_t... I>
auto iterator_to_array(IterType& it, std::index_sequence<I...>) {
    return std::to_array({(I, *it++)...});
}

template<typename T, typename IterType>
T read(IterType& it){
    return std::bit_cast<T>(iterator_to_array(it, std::make_index_sequence<sizeof(T)>{}));
}

Header read_header(auto& it){
    if(*it++ != 'q' || *it++ != 'o' || *it++ != 'i' || *it++ != 'f')
        throw std::logic_error{"Binary data must start with 'qoif' magic bytes"};
    return {__builtin_bswap32(read<uint32_t>(it)), __builtin_bswap32(read<uint32_t>(it)), static_cast<ChannelType>(*it++), static_cast<ColorType>(*it++)};
}

void decode_pixels(std::ranges::range auto data_range, auto output_it){
    std::array<RGBA, 64> array{};
    RGBA last_pixel = RGBA{0, 0, 0, 255};
    auto input_it = std::ranges::begin(data_range);
    while(input_it != std::ranges::end(data_range)){
        uint8_t flag = *input_it++;
        if(flag == 254){ // QOI_OP_RGB
            last_pixel = read<RGB>(input_it);
        }else if(flag == 255){ //QOI_OP_RGBA
            last_pixel = read<RGBA>(input_it);
        }else if(flag < 64){ // QOI_OP_INDEX
            if(flag == 0 && *input_it == 0)
                break; // Two 0's mean we've reached the end
            last_pixel = array[flag];
        }else if(flag < 128){ // QOI_OP_DIFF
            last_pixel.r += ((flag >> 4) & 3u) - 2;
            last_pixel.g += ((flag >> 2) & 3u) - 2;
            last_pixel.b += ( flag       & 3u) - 2;
        }else if(flag < 192){ // QOI_OP_LUMA                
            int luma = *input_it++;
            int dg = (flag & 63u) - 32;
            last_pixel.r += dg - 8 + ((luma >> 4) & 15u);
            last_pixel.g += dg;
            last_pixel.b += dg - 8 + (luma & 15u);
        }else{ // QOI_OP_RUN
            int32_t x = (flag & 63) + 1;
            while(x-->0){
                *output_it++ = last_pixel;
            }
            continue;
        }
        *output_it++ = last_pixel;
        array[last_pixel.hash()] = last_pixel;
    }
    if(!std::equal(input_it, data_range.end(), "\0\0\0\0\0\0\1"))
        throw std::logic_error{"Input didn't contain end padding"};
}

struct Image{
    Header header;
    std::vector<RGBA> pixels;

    template<typename IteratorType>
    requires ( sizeof(std::iter_value_t<IteratorType>) == 1 )
    explicit Image(IteratorType start, IteratorType end) {
        header = read_header(start);
        pixels.reserve(header.width * header.height);
        decode_pixels(std::ranges::subrange(std::move(start), std::move(end)), std::back_inserter(pixels));
    }
};

template<std::ranges::range DataRange>
requires ( sizeof(std::ranges::range_value_t<DataRange>) == 1 )
Image decode(const DataRange& data_range){
    return Image{data_range.begin(), data_range.end()};
}

}
