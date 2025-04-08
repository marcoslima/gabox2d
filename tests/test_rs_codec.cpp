#include <ranges>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <msgpack/msgpack.hpp>
#include <string>
#include <network/rs_codec.h>
using namespace std;

TEST_CASE("Codec", "[rs_codec]")
{
    constexpr auto string_size = 512;
    constexpr auto chunk_size = 100;
    constexpr auto redundancy = 5;
    RsCodec codec(chunk_size, redundancy);

    const string test_string(string_size, 'a');

    // Encode with reedsolomon
    auto encoded = codec.encode(test_string);

    // Assert the size of the encoded string is as expected
    constexpr auto overhead = ceil(string_size / static_cast<double>(chunk_size)) * redundancy;
    REQUIRE(encoded.size() == test_string.size() + overhead);

    SECTION("Decode")
    {
        // Decode with reedsolomon
        codec.decode(encoded);
        const auto decoded = codec.get_decoded_data();

        // Assert the decoded string is equal to the original string
        REQUIRE(decoded == test_string);
    }

    SECTION("With errors")
    {
        encoded[3] = 'b';
        encoded[4] = 'c';
        encoded[120] = 'd';

        // Decode with reedsolomon
        codec.decode(encoded);
        const auto decoded = codec.get_decoded_data();

        // Assert the decoded string is equal to the original string
        REQUIRE(decoded == test_string);
    }

    SECTION("With too much errors")
    {
        encoded[3] = 'b';
        encoded[4] = 'c';
        encoded[120] = 'd';
        encoded[121] = 'e';
        encoded[122] = 'f';

        REQUIRE(codec.decode(encoded) == false);
    }
}
