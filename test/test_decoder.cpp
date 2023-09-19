#include <qoi/decoder.h>

#include <array>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

using namespace qoi;

TEST_CASE("Decode header 1"){
    auto file = std::to_array<uint8_t>({ 'q', 'o', 'i', 'f', 0,0,0,0,  0,0,0,0,  4, 1});
    Image image = decode(file);

    CHECK(image.header.width == 0);
    CHECK(image.header.height == 0);
    CHECK(image.header.channels == ChannelType::RGBA);
    CHECK(image.header.colorspace == ColorType::RGB);
}


TEST_CASE("Decode header 2"){
    auto file = std::to_array<uint8_t>({ 'q', 'o', 'i', 'f', 0,0,0,0,    0,0,0,0, 3, 0});
    Image image = decode(file);

    CHECK(image.header.width == 0);
    CHECK(image.header.height == 0);
    CHECK(image.header.channels == ChannelType::RGB);
    CHECK(image.header.colorspace == ColorType::SRGB);
}

TEST_CASE("Decode QOI_OP_RGB"){
    auto file = std::to_array<uint8_t>({ 'q', 'o', 'i', 'f', 0,0,0,2,    0,0,0,1, 4, 1,
                        254, 100, 200, 240,
                        254, 50, 100, 200});

    Image image = decode(file);

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 1);
    CHECK(image.header.channels == ChannelType::RGBA);
    CHECK(image.header.colorspace == ColorType::RGB);

    CHECK(image.pixels.size() == 2);
    CHECK(image.pixels[0].r == 100);
    CHECK(image.pixels[0].g == 200);
    CHECK(image.pixels[0].b == 240);
    CHECK(image.pixels[0].a == 255);
    CHECK(image.pixels[1].r == 50);
    CHECK(image.pixels[1].g == 100);
    CHECK(image.pixels[1].b == 200);
    CHECK(image.pixels[1].a == 255);
}

TEST_CASE("Decode QOI_OP_RGBA"){
    auto file = std::to_array<uint8_t>({ 'q', 'o', 'i', 'f', 0,0,0,2,    0,0,0,1, 4, 1,
                        255, 100, 200, 240, 10,
                        255, 50, 100, 200, 150});

    Image image = decode(file);

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 1);
    CHECK(image.header.channels == ChannelType::RGBA);
    CHECK(image.header.colorspace == ColorType::RGB);

    CHECK(image.pixels.size() == 2);
    CHECK(image.pixels[0].r == 100);
    CHECK(image.pixels[0].g == 200);
    CHECK(image.pixels[0].b == 240);
    CHECK(image.pixels[0].a == 10);
    CHECK(image.pixels[1].r == 50);
    CHECK(image.pixels[1].g == 100);
    CHECK(image.pixels[1].b == 200);
    CHECK(image.pixels[1].a == 150);
}

TEST_CASE("Decode QOI_OP_INDEX"){
    auto file = std::to_array<uint8_t>({ 'q', 'o', 'i', 'f', 0,0,0,3,    0,0,0,1, 4, 1,
                        255, 1, 1, 1, 1,
                        255, 255, 255, 255, 255,
                        26});

    Image image = decode(file);

    CHECK(image.header.width == 3);
    CHECK(image.header.height == 1);
    CHECK(image.header.channels == ChannelType::RGBA);
    CHECK(image.header.colorspace == ColorType::RGB);

    CHECK(image.pixels.size() == 3);
    CHECK(image.pixels[0].r == 1);
    CHECK(image.pixels[0].g == 1);
    CHECK(image.pixels[0].b == 1);
    CHECK(image.pixels[0].a == 1);
    CHECK(image.pixels[1].r == 255);
    CHECK(image.pixels[1].g == 255);
    CHECK(image.pixels[1].b == 255);
    CHECK(image.pixels[1].a == 255);
    CHECK(image.pixels[2].r == 1);
    CHECK(image.pixels[2].g == 1);
    CHECK(image.pixels[2].b == 1);
    CHECK(image.pixels[2].a == 1);
}

TEST_CASE("Decode QOI_OP_DIFF"){
    auto file = std::to_array<uint8_t>({ 'q', 'o', 'i', 'f', 0,0,0,2,    0,0,0,1, 4, 1,
                        254, 100, 100, 100,
                        127});

    Image image = decode(file);

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 1);
    CHECK(image.header.channels == ChannelType::RGBA);
    CHECK(image.header.colorspace == ColorType::RGB);

    CHECK(image.pixels.size() == 2);
    CHECK(image.pixels[0].r == 100);
    CHECK(image.pixels[0].g == 100);
    CHECK(image.pixels[0].b == 100);
    CHECK(image.pixels[0].a == 255);
    CHECK(image.pixels[1].r == 101);
    CHECK(image.pixels[1].g == 101);
    CHECK(image.pixels[1].b == 101);
    CHECK(image.pixels[1].a == 255);
}

TEST_CASE("Decode QOI_OP_LUMA"){
    auto file = std::to_array<uint8_t>({ 'q', 'o', 'i', 'f', 0,0,0,2,    0,0,0,1, 4, 1,
                        254, 100, 100, 100,
                        191, 255});

    Image image = decode(file);

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 1);
    CHECK(image.header.channels == ChannelType::RGBA);
    CHECK(image.header.colorspace == ColorType::RGB);

    CHECK(image.pixels.size() == 2);
    CHECK(image.pixels[0].r == 100);
    CHECK(image.pixels[0].g == 100);
    CHECK(image.pixels[0].b == 100);
    CHECK(image.pixels[0].a == 255);
    CHECK(image.pixels[1].r == 138);
    CHECK(image.pixels[1].g == 131);
    CHECK(image.pixels[1].b == 138);
    CHECK(image.pixels[1].a == 255);
}

TEST_CASE("Decode QOI_OP_RUN"){
    auto file = std::to_array<uint8_t>({ 'q', 'o', 'i', 'f', 0,0,0,2,    0,0,0,2, 4, 1,
                        254, 100, 100, 100,
                        194});

    Image image = decode(file);

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 2);
    CHECK(image.header.channels == ChannelType::RGBA);
    CHECK(image.header.colorspace == ColorType::RGB);

    CHECK(image.pixels.size() == 4);
    CHECK(image.pixels[0].r == 100);
    CHECK(image.pixels[0].g == 100);
    CHECK(image.pixels[0].b == 100);
    CHECK(image.pixels[0].a == 255);
    
    CHECK(image.pixels[1].r == 100);
    CHECK(image.pixels[1].g == 100);
    CHECK(image.pixels[1].b == 100);
    CHECK(image.pixels[1].a == 255);
    
    CHECK(image.pixels[2].r == 100);
    CHECK(image.pixels[2].g == 100);
    CHECK(image.pixels[2].b == 100);
    CHECK(image.pixels[2].a == 255);
    
    CHECK(image.pixels[3].r == 100);
    CHECK(image.pixels[3].g == 100);
    CHECK(image.pixels[3].b == 100);
    CHECK(image.pixels[3].a == 255);
}
