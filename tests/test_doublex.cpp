#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <tuple>

#include <doublex.h>


TEST_CASE( "Doublex Instance", "[doublex]" ) 
{
    LmFisica::doublex n(1.5, 0.3);
    
    REQUIRE(n.V() == 1.5);
    REQUIRE(n.S() == 0.3);
}

TEST_CASE("Copy ctor", "[doublex]")
{
    LmFisica::doublex v1(1.5, 0.5);
    LmFisica::doublex v2(v1);

    REQUIRE(v2.V() == 1.5);
    REQUIRE(v2.S() == 0.5);
}

TEST_CASE("Add", "[doublex]")
{
    LmFisica::doublex v1(1.5, 3.0);
    LmFisica::doublex v2(2.7, 4.0);
    LmFisica::doublex r = v1 + v2;

    REQUIRE(r.V() == 4.2);
    REQUIRE(r.S() == 5.0);
}

TEST_CASE("Add Attribution", "[doublex]")
{
    LmFisica::doublex v1(1.5, 3.0);
    LmFisica::doublex v2(2.7, 4.0);
    v1 += v2;

    REQUIRE(v1.V() == 4.2);
    REQUIRE(v1.S() == 5.0);
}

TEST_CASE("Add double", "[doublex]")
{
    LmFisica::doublex v1(1.5, 3.0);
    double v2(2.7);
    auto r = v1 + v2;

    REQUIRE(r.V() == 4.2);
    REQUIRE(r.S() == 3.0);
}

TEST_CASE("Add double Attribution", "[doublex]")
{
    LmFisica::doublex v1(1.5, 3.0);
    double v2(2.7);
    v1 += v2;

    REQUIRE(v1.V() == 4.2);
    REQUIRE(v1.S() == 3.0);
}

TEST_CASE("Sub", "[doublex]")
{
    LmFisica::doublex v1(4.2, 3.0);
    LmFisica::doublex v2(1.7, 4.0);
    LmFisica::doublex r = v1 - v2;

    REQUIRE(r.V() == 2.5);
    REQUIRE(r.S() == 5.0);
}

TEST_CASE("Sub Attribution", "[doublex]")
{
    LmFisica::doublex v1(4.2, 3.0);
    LmFisica::doublex v2(1.7, 4.0);
    v1 -= v2;

    REQUIRE(v1.V() == 2.5);
    REQUIRE(v1.S() == 5.0);
}

TEST_CASE("Sub double", "[doublex]")
{
    LmFisica::doublex v1(4.2, 3.0);
    double v2(1.7);
    auto r = v1 - v2;

    REQUIRE(r.V() == 2.5);
    REQUIRE(r.S() == 3.0);
}

TEST_CASE("Sub double Attribution", "[doublex]")
{
    LmFisica::doublex v1(4.2, 3.0);
    double v2(1.7);
    v1 -= v2;

    REQUIRE(v1.V() == 2.5);
    REQUIRE(v1.S() == 3.0);
}

TEST_CASE("Mul", "[doublex]")
{
    LmFisica::doublex v1(2.0, sqrt(9.0));
    LmFisica::doublex v2(1.0, 2.0);
    LmFisica::doublex r = v1 * v2;

    REQUIRE(r.V() == 2.0);
    REQUIRE(r.S() == 5.0);
}

TEST_CASE("Mul Atribution", "[doublex]")
{
    LmFisica::doublex v1(2.0, sqrt(9.0));
    LmFisica::doublex v2(1.0, 2.0);
    v1 *= v2;

    REQUIRE(v1.V() == 2.0);
    REQUIRE(v1.S() == 5.0);
}

TEST_CASE("Mul double", "[doublex]")
{
    LmFisica::doublex v1(2.0, 3.0);
    double v2(4.0);
    auto r = v1 * v2;

    REQUIRE(r.V() == 8.0);
    REQUIRE(r.S() == 12.0);
}

TEST_CASE("Mul double Atribution", "[doublex]")
{
    LmFisica::doublex v1(2.0, 3.0);
    double v2(3.0);
    v1 *= v2;

    REQUIRE(v1.V() == 6.0);
    REQUIRE(v1.S() == 9.0);
}

TEST_CASE("Div", "[doublex]")
{
    LmFisica::doublex v1(9.0, 9.0);
    LmFisica::doublex v2(3.0, 4.0);
    LmFisica::doublex r = v1 / v2;

    REQUIRE(r.V() == 3.0);
    REQUIRE(r.S() == 5.0);
}

TEST_CASE("Div Attribution", "[doublex]")
{
    LmFisica::doublex v1(9.0, 9.0);
    LmFisica::doublex v2(3.0, 4.0);
    v1 /= v2;

    REQUIRE(v1.V() == 3.0);
    REQUIRE(v1.S() == 5.0);
}

TEST_CASE("Div double", "[doublex]")
{
    LmFisica::doublex v1(9.0, 9.0);
    double v2(3.0);
    auto r = v1 / v2;

    REQUIRE(r.V() == 3.0);
    REQUIRE(r.S() == 3.0);
}

TEST_CASE("Div double Attribution", "[doublex]")
{
    LmFisica::doublex v1(9.0, 12.0);
    double v2(3.0);
    v1 /= v2;

    REQUIRE(v1.V() == 3.0);
    REQUIRE(v1.S() == 4.0);
}

TEST_CASE("Pow", "[doublex]")
{
    LmFisica::doublex v1(2.0, 1.0);
    LmFisica::doublex v2(3.0, 0.1);
    LmFisica::doublex r = v1 ^ v2;

    REQUIRE(r.V() == 8.0);
    REQUIRE_THAT(r.S(), 
                Catch::Matchers::WithinAbs(12.0128052481, 
                                            0.0000000001));
}

TEST_CASE("Pow Attribution", "[doublex]")
{
    LmFisica::doublex v1(2.0, 1.0);
    LmFisica::doublex v2(3.0, 0.1);
    v1 ^= v2;

    REQUIRE(v1.V() == 8.0);
    REQUIRE_THAT(v1.S(), 
                Catch::Matchers::WithinAbs(12.0128052481, 
                                            0.0000000001));
}

TEST_CASE("Pow double", "[doublex]")
{
    LmFisica::doublex v1(2.0, 1.0);
    double v2(3.0);
    LmFisica::doublex r = v1 ^ v2;

    REQUIRE(r.V() == 8.0);
    REQUIRE_THAT(r.S(), 
                Catch::Matchers::WithinAbs(12.0, 
                                            0.0000000001));
}

TEST_CASE("Pow double Attribution", "[doublex]")
{
    LmFisica::doublex v1(2.0, 1.0);
    double v2(3.0);
    v1 ^= v2;

    REQUIRE(v1.V() == 8.0);
    REQUIRE_THAT(v1.S(), 
                Catch::Matchers::WithinAbs(12.0, 
                                            0.0000000001));
}

TEST_CASE("Minus", "[doublex]")
{
    LmFisica::doublex v1(2.0, 1.0);
    auto r = -v1;

    REQUIRE(r.V() == -2.0);
    REQUIRE(r.S() == v1.S());
}

TEST_CASE("Attribution", "[doublex]")
{
    LmFisica::doublex v1(3.0, 0.42);
    auto r = v1;

    REQUIRE(r.V() == 3.0);
    REQUIRE(r.S() == 0.42);
}

TEST_CASE("Attribution double", "[doublex]")
{
    LmFisica::doublex r;
    r = static_cast<double>(3.0);

    REQUIRE(r.V() == 3.0);
    REQUIRE(r.S() == DBL_EPSILON);
}


TEST_CASE("Attribution long", "[doublex]")
{
    LmFisica::doublex r;
    r = 42L;

    REQUIRE(r.V() == 42.0);
    REQUIRE(r.S() == DBL_EPSILON);
}


TEST_CASE("Attribution int", "[doublex]")
{
    LmFisica::doublex r;
    r = 42;

    REQUIRE(r.V() == 42.0);
    REQUIRE(r.S() == DBL_EPSILON);
}

TEST_CASE("Is Less", "[doublex]")
{
    LmFisica::doublex v1(2.0, 1.0);
    LmFisica::doublex v2(3.0, 1.0);
    bool r = v1 < v2;

    REQUIRE(r);
}

TEST_CASE("Is not Less", "[doublex]")
{
    LmFisica::doublex v1(3.0, 1.0);
    LmFisica::doublex v2(2.0, 1.0);
    bool r = v1 < v2;
    REQUIRE(!r);
}

TEST_CASE("Is Greater", "[doublex]")
{
    LmFisica::doublex v1(3.0, 1.0);
    LmFisica::doublex v2(2.0, 1.0);
    bool r = v1 > v2;

    REQUIRE(r);
}

TEST_CASE("Is not Greater", "[doublex]")
{
    LmFisica::doublex v1(2.0, 1.0);
    LmFisica::doublex v2(3.0, 1.0);
    bool r = v1 > v2;
    REQUIRE(!r);
}
////////////////////

TEST_CASE("Is Less or equal", "[doublex]")
{
    SECTION("Is less or equal section")
    {
        using dbx = LmFisica::doublex;
        dbx v1, v2;
        bool expected;
        std::tie(v1, v2, expected) = 
            GENERATE
            (
                table<dbx, dbx, bool>
                ({
                    std::make_tuple(dbx(2.0, 0.1),
                                    dbx(3.0, 0.1),
                                    true),
                    std::make_tuple(dbx(2.0, 0.1),
                                    dbx(2.0, 0.1),
                                    true),
                    std::make_tuple(dbx(2.0, 0.1),
                                    dbx(1.5, 0.1),
                                    false)
                })
            );
        auto result = v1 <= v2;
        REQUIRE(result == expected);
    }
}

TEST_CASE("Is Greather or equal", "[doublex]")
{
    SECTION("Is greather or equal section")
    {
        using dbx = LmFisica::doublex;
        dbx v1, v2;
        bool expected;
        std::tie(v1, v2, expected) = 
            GENERATE
            (
                table<dbx, dbx, bool>
                ({
                    std::make_tuple(dbx(3.0, 0.1),
                                    dbx(2.0, 0.1),
                                    true),
                    std::make_tuple(dbx(2.0, 0.1),
                                    dbx(2.0, 0.1),
                                    true),
                    std::make_tuple(dbx(1.5, 0.1),
                                    dbx(2.0, 0.1),
                                    false)
                })
            );
        auto result = v1 >= v2;
        REQUIRE(result == expected);
    }
}


TEST_CASE("Equal", "[doublex]")
{
    SECTION("Equal section")
    {
        using dbx = LmFisica::doublex;
        dbx v1, v2;
        bool expected;
        std::tie(v1, v2, expected) = 
            GENERATE
            (
                table<dbx, dbx, bool>
                ({
                    std::make_tuple(dbx(2.1, 0.1),
                                    dbx(2.0, 0.1),
                                    true),
                    std::make_tuple(dbx(2.1, 0.01),
                                    dbx(2.0, 0.01),
                                    false),
                    std::make_tuple(dbx(1.5, 0.1),
                                    dbx(2.0, 0.1),
                                    false)
                })
            );
        auto result = v1 == v2;
        CAPTURE(v1.V(), v1.S(), v2.V(), v2.S());
        REQUIRE(result == expected);
    }
}


TEST_CASE("Not equal", "[doublex]")
{
    SECTION("Not equal section")
    {
        using dbx = LmFisica::doublex;
        dbx v1, v2;
        bool expected;
        std::tie(v1, v2, expected) = 
            GENERATE
            (
                table<dbx, dbx, bool>
                ({
                    std::make_tuple(dbx(2.1, 0.1),
                                    dbx(2.0, 0.1),
                                    false),
                    std::make_tuple(dbx(2.1, 0.01),
                                    dbx(2.0, 0.01),
                                    true),
                    std::make_tuple(dbx(1.5, 0.1),
                                    dbx(2.0, 0.1),
                                    true)
                })
            );
        auto result = v1 != v2;
        CAPTURE(v1.V(), v1.S(), v2.V(), v2.S());
        REQUIRE(result == expected);
    }
}

//////////////////////////////

TEST_CASE("Sin", "[doublex]")
{
    SECTION("Sin section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(M_PI_4, 0.1),
                                    dbx(M_SQRT1_2, 0.1)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::sin(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.V()/10.0, 
                                       0.0000000001));
    }
}

TEST_CASE("Cos", "[doublex]")
{
    SECTION("Cos section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(M_PI_4, 0.1),
                                    dbx(M_SQRT1_2, M_SQRT1_2/10.0)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::cos(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("Tan", "[doublex]")
{
    SECTION("Tan section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(M_PI_4, 0.1),
                                    dbx(1.0, 0.2)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::tan(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("ASin", "[doublex]")
{
    SECTION("ASin section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(M_PI_4, 0.1),
                                    dbx(0.9033391108, 0.1615532655)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::asin(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("ACos", "[doublex]")
{
    SECTION("ACos section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(M_PI_4, 0.1),
                                    dbx(0.667457216, 0.1615532655)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::acos(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("ATan", "[doublex]")
{
    SECTION("ATan section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(M_PI_4, 0.1),
                                    dbx(0.66577375, 0.0618486458)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::atan(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("SinH", "[doublex]")
{
    SECTION("SinH section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(M_PI_4, 0.1),
                                    dbx(0.8686709615, 0.1324609089)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::sinh(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("CosH", "[doublex]")
{
    SECTION("CosH section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(M_PI_4, 0.1),
                                    dbx(1.3246090893, 0.0868670961)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::cosh(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("TanH", "[doublex]")
{
    SECTION("TanH section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(M_PI_4, 0.1),
                                    dbx(0.6557942026, 0.0569933964)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::tanh(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}
///////////////////////////////////

TEST_CASE("Floor", "[doublex]")
{
    SECTION("Floor section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(1.99, 0.1),
                                    dbx(1.0, 1.1)),
                    std::make_tuple(dbx(1.01, 0.1),
                                    dbx(1.0, 1.1)),
                    std::make_tuple(dbx(-1.99, 0.1),
                                    dbx(-2.0, 1.1)),
                    std::make_tuple(dbx(-1.01, 0.1),
                                    dbx(-2.0, 1.1)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::floor(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}


TEST_CASE("Ceil", "[doublex]")
{
    SECTION("Ceil section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(1.99, 0.1),
                                    dbx(2.0, 1.1)),
                    std::make_tuple(dbx(1.01, 0.1),
                                    dbx(2.0, 1.1)),
                    std::make_tuple(dbx(-1.99, 0.1),
                                    dbx(-1.0, 1.1)),
                    std::make_tuple(dbx(-1.01, 0.1),
                                    dbx(-1.0, 1.1)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::ceil(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("Significativos", "[doublex]")
{
    SECTION("Significativos section")
    {
        using dbx = LmFisica::doublex;
        dbx v, expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, dbx>
                ({
                    std::make_tuple(dbx(1.1234345699, 0.1),
                                    dbx(1.12, 0.1)),
                    std::make_tuple(dbx(1.12345345, 0.1234),
                                    dbx(1.12, 0.12)),
                })
            );
        CAPTURE(v.V(), v.S(), expected.V(), expected.S());
        auto result = LmFisica::signi(v);
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("Mul left double", "[doublex]")
{
    SECTION("Mul left double section")
    {
        using dbx = LmFisica::doublex;
        double vd;
        dbx vdbx, expected;
        std::tie(vd, vdbx, expected) = 
            GENERATE
            (
                table<double, dbx, dbx>
                ({
                    std::make_tuple(2.0, 
                                    dbx(3.0, 0.1),
                                    dbx(6.0, 0.2)),
                })
            );
        CAPTURE(vd, vdbx.V(), vdbx.S(), expected.V(), expected.S());
        auto result = vd * vdbx;
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}


TEST_CASE("Mod", "[doublex]")
{
    SECTION("Mod section")
    {
        using dbx = LmFisica::doublex;
        dbx v1, v2, expected;
        std::tie(v1, v2, expected) = 
            GENERATE
            (
                table<dbx, dbx, dbx>
                ({
                    std::make_tuple(dbx(12.1, 0.1),
                                    dbx(10.0, 0.1),
                                    dbx(2.1, 0.1414213562)),
                })
            );
        auto result = LmFisica::mod(v1, v2);
        CAPTURE(v1.V(), v1.S(), v2.V(), v2.S());
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("Log", "[doublex]")
{
    SECTION("Log section")
    {
        using dbx = LmFisica::doublex;
        dbx v1, v2, expected;
        std::tie(v1, v2, expected) = 
            GENERATE
            (
                table<dbx, dbx, dbx>
                ({
                    std::make_tuple(dbx(12.1, 0.1),
                                    dbx(10.0, 0.1),
                                    dbx(1.0827853703, 0.0035892106)),
                })
            );
        auto result = LmFisica::log(v1, v2);
        CAPTURE(v1.V(), v1.S(), v2.V(), v2.S());
        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}


TEST_CASE("Str", "[doublex]")
{
    SECTION("Str section")
    {
        using dbx = LmFisica::doublex;
        dbx v;
        std::string expected;
        std::tie(v, expected) = 
            GENERATE
            (
                table<dbx, std::string>
                ({
                    std::make_tuple(dbx(12.1, 0.1),
                                    std::string("12.100000 ± 0.100000")),
                })
            );
        auto result = v.str();
        REQUIRE(result == expected);
    }
}

TEST_CASE("Somatorio")
{
    using dbx = LmFisica::doublex;
    using vec_dbx = std::vector<dbx>;

    SECTION("One element")
    {
        vec_dbx sut = {dbx(42.17, 0.01)};

        REQUIRE(LmFisica::Somatorio(sut) == sut[0]);
    }

    SECTION("Two elements, same incertainty")
    {
        vec_dbx sut = {
            dbx(42.17, 0.01),
            dbx(7.83, 0.01),
        };
        dbx expected(50.0, 0.0141421356);

        auto result = LmFisica::Somatorio(sut);

        CAPTURE(sut[0].str(), sut[1].str(), result.str(), expected.str());

        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }

    SECTION("Two elements, different incertainties")
    {
        vec_dbx sut = {
            dbx(42.17, 0.01),
            dbx(7.83, 0.12),
        };
        dbx expected(50.0, 0.1204159458);

        auto result = LmFisica::Somatorio(sut);

        CAPTURE(sut[0].str(), sut[1].str(), result.str(), expected.str());

        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}


TEST_CASE("Somatorio2")
{
    using dbx = LmFisica::doublex;
    using vec_dbx = std::vector<dbx>;

    SECTION("One element")
    {
        vec_dbx sut = {dbx(42.17, 0.01)};
        dbx expected(1778.3089, 0.5963738593);

        auto result = LmFisica::Somatorio2(sut);

        CAPTURE(sut[0].str(), 
                sut[1].str(), 
                result.str(), 
                expected.str());

        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }

    SECTION("Two elements, same incertainty")
    {
        vec_dbx sut = {
            dbx(42.17, 0.01),
            dbx(7.83, 0.01),
        };
        dbx expected(1839.6178, 0.6065670284);

        auto result = LmFisica::Somatorio2(sut);

        CAPTURE(sut[0].str(), 
                sut[1].str(), 
                result.str(), 
                expected.str());

        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }

    SECTION("Two elements, different incertainties")
    {
        vec_dbx sut = {
            dbx(42.17, 0.01),
            dbx(7.83, 0.12),
        };
        dbx expected(1839.6178, 1.4564882766);

        auto result = LmFisica::Somatorio2(sut);

        CAPTURE(sut[0].str(), 
                sut[1].str(), 
                result.str(), 
                expected.str());

        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("Media")
{
    using dbx = LmFisica::doublex;
    using vec_dbx = std::vector<dbx>;

    SECTION("One element")
    {
        vec_dbx sut = {dbx(42.17, 0.01)};
        dbx expected(42.17, 0.01);

        auto result = LmFisica::Media(sut);

        CAPTURE(sut[0].str(), 
                sut[1].str(), 
                result.str(), 
                expected.str());

        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }

    SECTION("Two elements, same incertainty")
    {
        vec_dbx sut = {
            dbx(42.17, 0.01),
            dbx(7.83, 0.01),
        };
        dbx expected(25, 0.0070710678);

        auto result = LmFisica::Media(sut);

        CAPTURE(sut[0].str(), 
                sut[1].str(), 
                result.str(), 
                expected.str());

        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }

    SECTION("Two elements, different incertainties")
    {
        vec_dbx sut = {
            dbx(42.17, 0.01),
            dbx(7.83, 0.12),
        };
        dbx expected(25, 0.0602079729);

        auto result = LmFisica::Media(sut);

        CAPTURE(sut[0].str(), 
                sut[1].str(), 
                result.str(), 
                expected.str());

        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}

TEST_CASE("StdDev")
{
    using dbx = LmFisica::doublex;
    using vec_dbx = std::vector<dbx>;

    SECTION("One element")
    {
        vec_dbx sut = {dbx(42.17, 0.01)};
        dbx expected(0.0, 0.0);

        auto result = LmFisica::StdDev(sut);

        CAPTURE(sut[0].str(), 
                sut[1].str(), 
                result.str(), 
                expected.str());

        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }

    SECTION("Two elements, same incertainty")
    {
        vec_dbx sut = {
            dbx(42.17, 0.01),
            dbx(7.83, 0.01),
        };
        dbx expected(24.2820468659, 0.0173205081);

        auto result = LmFisica::StdDev(sut);

        CAPTURE(sut[0].str(), 
                sut[1].str(), 
                result.str(), 
                expected.str());

        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }

    SECTION("Two elements, different incertainties")
    {
        vec_dbx sut = {
            dbx(42.17, 0.01),
            dbx(7.83, 0.1810386699),
        };
        dbx expected(24.2820468659, 0.2220641799);

        auto result = LmFisica::StdDev(sut);

        CAPTURE(sut[0].str(), 
                sut[1].str(), 
                result.str(), 
                expected.str());

        REQUIRE_THAT(result.V(), 
            Catch::Matchers::WithinAbs(expected.V(), 
                                       0.0000000001));
        REQUIRE_THAT(result.S(), 
            Catch::Matchers::WithinAbs(expected.S(), 
                                       0.0000000001));
    }
}
