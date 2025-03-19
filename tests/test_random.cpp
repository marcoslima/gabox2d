#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <gmock/gmock.h>

#include "CRandom.h"

TEST_CASE( "Change Seed", "[RANDOM]" )
{
    const CRandom random(0);

    REQUIRE(random.rand_int(0, 100) == 55);
    REQUIRE(random.rand_int(0, 100) == 59);

    random.set_seed(42);
    REQUIRE(random.rand_int(0, 100) == 37);
    REQUIRE(random.rand_int(0, 100) == 80);
}

TEST_CASE( "Random Numbers", "[RANDOM]" )
{
    const CRandom random(42);

    REQUIRE(random.rand_int(0, 100) == 37);
    REQUIRE(random.real_random(-1.0, 1.0) == Catch::Approx(0.9014286235));
    REQUIRE(random.discrete_random(-10, 10) == -7);
    REQUIRE(random.normal_random(0.0, 1.0) == Catch::Approx(0.4738608557));
}

TEST_CASE( "Explicit instantiations", "[RANDOM]" )
{
    const CRandom random(42);

    SECTION("double")
    {
        constexpr double double_min = -1.0;
        constexpr double double_max = 1.0;
        REQUIRE(random.real_random(double_min, double_max) == Catch::Approx(0.5930859686));
        REQUIRE(random.normal_random(double_min, double_max) == Catch::Approx(-0.4562568432));
    }

    SECTION("float")
    {
        constexpr float float_min = -1.0;
        constexpr float float_max = 1.0;
        REQUIRE(random.real_random(float_min, float_max) == Catch::Approx(-0.25092f));
        REQUIRE(random.normal_random(float_min, float_max) == Catch::Approx(-0.41826f));
    }

    SECTION("char")
    {
        constexpr char char_min = -10;
        constexpr char char_max = 10;
        REQUIRE(random.discrete_random<char>(char_min, char_max) == '\xFD');
    }

    SECTION("size_t")
    {
        size_t size_t_min = 0;
        size_t size_t_max = 100000;
        size_t result = random.discrete_random(size_t_min, size_t_max);
        REQUIRE(result == 37454);
    }

    SECTION("int")
    {
        constexpr int int_min = 0;
        constexpr int int_max = 100;
        REQUIRE(random.discrete_random(int_min, int_max) == 37);
    }

}

