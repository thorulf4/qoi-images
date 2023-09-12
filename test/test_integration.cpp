#include <qoi/encoder.h>
#include <qoi/decoder.h>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("Integration OP_RUN"){
    std::array<uint8_t, 1024> data;

    constexpr std::array<RGBA, 4> image_data = {
        RGBA{255,255,255}, RGBA{255,255,255},
        RGBA{255,255,255}, RGBA{255,255,255}
    };

    auto* end = encode(2,2, image_data, data.data());

    Image image = decode(data.data(), data.end());

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 2);
    CHECK(image.pixels[0] == RGBA{255,255,255});
    CHECK(image.pixels[1] == RGBA{255,255,255});
    CHECK(image.pixels[2] == RGBA{255,255,255});
    CHECK(image.pixels[3] == RGBA{255,255,255});
}

TEST_CASE("Integration OP_INDEX"){
    std::array<uint8_t, 1024> data;

    constexpr std::array<RGBA, 4> image_data = {
        RGBA{255,255,255}, RGBA{0,0,0},
        RGBA{255,255,255}, RGBA{0,0,0}
    };

    auto* end = encode(2,2, image_data, data.data());

    Image image = decode(data.data(), data.end());

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 2);
    CHECK(image.pixels[0] == RGBA{255,255,255});
    CHECK(image.pixels[1] == RGBA{0,0,0});
    CHECK(image.pixels[2] == RGBA{255,255,255});
    CHECK(image.pixels[3] == RGBA{0,0,0});
}

TEST_CASE("Integrate with OP_DIFF"){
    std::array<uint8_t, 1024> data;

    constexpr std::array<RGBA, 4> image_data = {
        RGBA{255,255,255}, RGBA{253,254,255},
        RGBA{252,252,253}, RGBA{253,251,251}
    };

    auto* end = encode(2,2, image_data, data.data());

    Image image = decode(data.data(), data.end());

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 2);
    CHECK(image.pixels[0] == RGBA{255,255,255});
    CHECK(image.pixels[1] == RGBA{253,254,255});
    CHECK(image.pixels[2] == RGBA{252,252,253});
    CHECK(image.pixels[3] == RGBA{253,251,251});
}

TEST_CASE("Integrate with OP_LUMA"){
    std::array<uint8_t, 1024> data;

    constexpr std::array<RGBA, 4> image_data = {
        RGBA{255,255,255}, RGBA{235,234,236},
        RGBA{205,204,206}, RGBA{215,215,215}
    };

    auto* end = encode(2,2, image_data, data.data());

    Image image = decode(data.data(), data.end());

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 2);
    CHECK(image.pixels[0] == RGBA{255,255,255});
    CHECK(image.pixels[1] == RGBA{235,234,236});
    CHECK(image.pixels[2] == RGBA{205,204,206});
    CHECK(image.pixels[3] == RGBA{215,215,215});
}