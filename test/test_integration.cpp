#include <qoi/core.h>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

using namespace qoi;

TEST_CASE("Integration OP_RUN"){
    std::vector<uint8_t> data;

    constexpr std::array<RGBA, 4> image_data = {
        RGBA{255,255,255}, RGBA{255,255,255},
        RGBA{255,255,255}, RGBA{255,255,255}
    };

    encode({2,2, ChannelType::RGBA, ColorType::RGB}, image_data, std::back_inserter(data));

    auto image = TransparentImage{data};

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 2);
    CHECK(image.pixels[0] == RGBA{255,255,255});
    CHECK(image.pixels[1] == RGBA{255,255,255});
    CHECK(image.pixels[2] == RGBA{255,255,255});
    CHECK(image.pixels[3] == RGBA{255,255,255});
}

TEST_CASE("Integration OP_INDEX"){
    std::vector<uint8_t> data;

    constexpr std::array<RGBA, 4> image_data = {
        RGBA{255,255,255}, RGBA{0,0,0},
        RGBA{255,255,255}, RGBA{0,0,0}
    };

    encode({2,2, ChannelType::RGBA, ColorType::RGB}, image_data, std::back_inserter(data));

    auto image = TransparentImage{data};

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 2);
    CHECK(image.pixels[0] == RGBA{255,255,255});
    CHECK(image.pixels[1] == RGBA{0,0,0});
    CHECK(image.pixels[2] == RGBA{255,255,255});
    CHECK(image.pixels[3] == RGBA{0,0,0});
}

TEST_CASE("Integrate with OP_DIFF"){
    std::vector<uint8_t> data;

    constexpr std::array<RGBA, 4> image_data = {
        RGBA{255,255,255}, RGBA{253,254,255},
        RGBA{252,252,253}, RGBA{253,251,251}
    };

    encode({2,2, ChannelType::RGBA, ColorType::RGB}, image_data, std::back_inserter(data));

    auto image = TransparentImage{data};

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 2);
    CHECK(image.pixels[0] == RGBA{255,255,255});
    CHECK(image.pixels[1] == RGBA{253,254,255});
    CHECK(image.pixels[2] == RGBA{252,252,253});
    CHECK(image.pixels[3] == RGBA{253,251,251});
}

TEST_CASE("Integrate with OP_LUMA"){
    std::vector<uint8_t> data;

    constexpr std::array<RGB, 4> image_data = {
        RGB{255,255,255}, RGB{235,234,236},
        RGB{205,204,206}, RGB{215,215,215}
    };

    encode({2,2, ChannelType::RGB, ColorType::RGB}, image_data, std::back_inserter(data));

    auto image = Image{data};

    CHECK(image.header.width == 2);
    CHECK(image.header.height == 2);
    CHECK(image.pixels[0] == RGB{255,255,255});
    CHECK(image.pixels[1] == RGB{235,234,236});
    CHECK(image.pixels[2] == RGB{205,204,206});
    CHECK(image.pixels[3] == RGB{215,215,215});
}