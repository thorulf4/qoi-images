#pragma once
#include <qoi.h>

#include <vector>

struct Image{
    Header header;
    std::vector<RGBA> pixels;
};

Image decode(const uint8_t* data);