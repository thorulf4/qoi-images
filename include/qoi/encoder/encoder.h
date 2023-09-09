#pragma once
#include <qoi.h>

#include <algorithm>
#include <cstring>

class Writer{
    uint8_t* out;
public:
    Writer(uint8_t* out) : out{out} {}
    template<typename T>
    requires (alignof(T) == 1 || alignof(T) == sizeof(T)) // Ensure no padding for trivial copy
    void write(const T& value){
        std::memcpy(out, &value, sizeof(T));
        out += sizeof(T);
    }
    void write(const Header& header);
    void write_repeat(uint32_t times);
    uint8_t* get_end() const;
};

template<std::ranges::range PixelRange, typename PixelType = std::ranges::range_value_t<PixelRange>>
uint8_t* encode(uint32_t width, uint32_t height, const PixelRange& pixels, uint8_t* out){
    Writer writer{out};
    writer.write(Header{width, height, PixelType::channel, ColorType::RGB});
    
    std::array<PixelType, 64> memory;
    constexpr static RGBA default_pixel = RGBA{0,0,0,255};
    const PixelType* last_pixel = &default_pixel;

    uint32_t repetition_count = 0;

    auto pixel_count = width * height;
    for(const PixelType& pixel : pixels){
        if(*last_pixel == pixel){
            repetition_count++;
            continue;
        }else if(repetition_count > 0){
            writer.write_repeat(repetition_count); // QOI_OP_RUN
            repetition_count = 0;
        }
        uint8_t hash = pixel.hash();

        if(memory[hash] == pixel){
            writer.write(hash); // QOI_OP_INDEX
            goto next;
        }else{
            memory[hash] = pixel;
        }

        if(std::is_same_v<PixelType, RGB> || last_pixel->a == pixel.a){
            auto diff_r = static_cast<int32_t>(pixel.r) - static_cast<int32_t>(last_pixel->r);
            auto diff_g = static_cast<int32_t>(pixel.g) - static_cast<int32_t>(last_pixel->g);
            auto diff_b = static_cast<int32_t>(pixel.b) - static_cast<int32_t>(last_pixel->b);
            if(diff_r >= -2 && diff_r <= 1 && diff_g >= -2 && diff_g <= 1 && diff_b >= -2 && diff_b <= 1){
                writer.write<uint8_t>(64 | ((diff_r+2) << 4) | ((diff_g+2) << 2) | (diff_b+2)); // QOI_OP_DIFF
                goto next;
            }

            if(diff_g >= -32 && diff_g <= 31){
                auto dr_dg = diff_r - diff_g;
                auto db_dg = diff_b - diff_g;
                if(dr_dg >= -8 && dr_dg <= 7 && db_dg >= -8 && db_dg <= 7){
                    writer.write<uint8_t>(128 | (diff_g + 32));
                    writer.write<uint8_t>(((dr_dg + 8) << 4) | (db_dg + 8)); // QOI_OP_LUMA
                    goto next;
                }
            }

            writer.write(254); // QOI_OP_RGB
            writer.write<RGB>(pixel);
        }else{
            writer.write(255); // QOI_OP_RGBA
            writer.write<RGBA>(pixel);
        }
        next:
        last_pixel = &pixel;
    }

    if(repetition_count > 0)
        writer.write_repeat(repetition_count); // QOI_OP_RUN

    
    return writer.get_end();
}