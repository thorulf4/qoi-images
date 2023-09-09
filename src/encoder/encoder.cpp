#include <encoder.h>

namespace ranges = std::ranges;
constexpr uint8_t OP_RUN = 192;

void Writer::write(const Header& header){
    ranges::copy_n("qoif", 4, out); out += 4;
    write(header.width);
    write(header.height);
    write(header.channels);
    write(header.colorspace);
}

void Writer::write_repeat(uint32_t times){
    while(times > 62){
        *out++ = OP_RUN | 62;
        times -= 62;
    }
    *out++ = OP_RUN | times;
}

uint8_t* Writer::get_end() const {
    return out;
}