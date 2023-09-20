#include <qoi/core.h>

#include <array>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

using namespace qoi;

TEST_CASE("Decode header 1"){
    auto image = Image{1,2, ColorType::RGB};
    image.pixels.emplace_back(255,255,255);
    image.pixels.emplace_back(255,255,255);
    
    
    std::vector<uint8_t> data;
    image.write(std::back_inserter(data));
    
    REQUIRE(data.size() == 27);

    CHECK(data[0] == 113);
    CHECK(data[1] == 111);
    CHECK(data[2] == 105);
    CHECK(data[3] == 102);
    CHECK(data[4] == 0);
    CHECK(data[5] == 0);
    CHECK(data[6] == 0);
    CHECK(data[7] == 1);
    CHECK(data[8] == 0);
    CHECK(data[9] == 0);
    CHECK(data[10] == 0);
    CHECK(data[11] == 2);
    CHECK(data[12] == 3);
    CHECK(data[13] == 1);

    CHECK(data[14] == 254);
    CHECK(data[15] == 255);
    CHECK(data[16] == 255);
    CHECK(data[17] == 255);

    CHECK(data[18] == 192);

    CHECK(data[19] == 0);
    CHECK(data[20] == 0);
    CHECK(data[21] == 0);
    CHECK(data[22] == 0);
    CHECK(data[23] == 0);
    CHECK(data[24] == 0);
    CHECK(data[25] == 0);
    CHECK(data[26] == 1);
}

TEST_CASE("writer little endian"){
    auto data = std::array<uint8_t, 4>{};
    auto it = data.begin();
    qoi::write<std::endian::little>(it, 32);

    CHECK(it == data.end());
    CHECK(data[0] == 32);
    CHECK(data[1] == 0);
    CHECK(data[2] == 0);
    CHECK(data[3] == 0);   
}

TEST_CASE("writer big endian"){
    auto data = std::array<uint8_t, 4>{};
    auto it = data.begin();
    qoi::write<std::endian::big>(it, 32);

    CHECK(it == data.end());
    CHECK(data[0] == 0);
    CHECK(data[1] == 0);
    CHECK(data[2] == 0);
    CHECK(data[3] == 32);   
}