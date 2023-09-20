#pragma once
#include "encoder.h"
#include "decoder.h"

namespace qoi{

template<typename PixelType>
struct BasicImage{
    Header header;
    std::vector<PixelType> pixels;

    template<typename IteratorType>
    BasicImage(IteratorType start, IteratorType end) {
        header = read_header(start);
        pixels.reserve(header.width * header.height);
        decode_pixels(std::ranges::subrange(std::move(start), std::move(end)), std::back_inserter(pixels));
    }
    template<std::ranges::range ByteRange>
    explicit BasicImage(const ByteRange& range): BasicImage{range.begin(), range.end()} {}
    BasicImage(uint32_t width, uint32_t height, ColorType type): header{width, header, PixelType::channel, type} {}

    void write(auto output_it){
        encode(header, pixels, std::move(output_it));
    }
};

using Image = BasicImage<RGB>;
using TransparentImage = BasicImage<RGBA>;

}