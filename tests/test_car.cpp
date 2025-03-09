#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <cstdint>
#include "car.h"
#include "GaCar.h"

#include <global_random.h>

#define DUMMY_GENES "JUYETUPPELECBLWIPDSQABZSVYFAEZEQHPNALAHNPKDBHZJGLCUQFJNZPMBWPRELBAYYZOVKHACGRGLRDPMVAEXKGERTILNFOOEAZVULYKXYPSXICOFNBYIVKTHOVPJZHPOHDHUEBAZLUKFHAAVFSSSUUPBYJQDXWWQLICBJIRIRSPQHXOMJDZSWTSOGUGMBNSLCALCFAXQMIONSXDGPKOTFFYCPHSEWYQVHQCWLUFEKXWOIUDXJIXCHFQLAVJWHAENNKMFSDHIGYEIFNOSKJBZGZGGSMSHDHZAGPZNKBAHI"
#define DUMMY_ALTGENES "ASDFGUPPELECBLWIPDSQABZSVYFAEZEQHPNALAHNPKDBHZJGLCUQFJNZPMBWPRELBAYYZOVKHACGRGLRDPMVAEXKGERTILNFOOEAZVULYKXYPSXICOFNBYIVKTHOVPJZHPOHDHUEBAZLUKFHAAVFSSSUUPBYJQDXWWQLICBJIRIRSPQHXOMJDZSWTSOGUGMBNSLCALCFAXQMIONSXDGPKOTFFYCPHSEWYQVHQCWLUFEKXWOIUDXJIXCHFQLAVJWHAENNKMFSDHIGYEIFNOSKJBZGZGGSMSHDHZAGPZNKBAHI"

TEST_CASE( "CCar instance", "[CCar]" ) {
    GA::random.set_seed(42);

    CCar car;

    REQUIRE( car.getGenes() == string(DUMMY_GENES) );
}

TEST_CASE( "CGaCar ctor with genes", "[CGaCar]" )
{
    GA::random.set_seed(42);

    const char *genes = "JUYET";
    GA::CGaCar car(genes);

    REQUIRE( car.getGenes() == string("JUYET") );
}

TEST_CASE( "CGaCar getGenesString", "[CGaCar]" )
{
    GA::random.set_seed(42);

    GA::CGaCar car(DUMMY_GENES);

    REQUIRE( car.getGenesString() == string(DUMMY_GENES) );
}

TEST_CASE( "CGaCar getGenes", "[CGaCar]" )
{
    GA::random.set_seed(42);

    GA::CGaCar car(DUMMY_GENES);
    string genes;
    car.getGenes(genes);

    REQUIRE( genes == string(DUMMY_GENES) );
}

TEST_CASE( "CGaCar setGenes", "[CGaCar]" )
{
    GA::random.set_seed(42);

    GA::CGaCar car(DUMMY_GENES);
    car.setGenes(DUMMY_ALTGENES);

    REQUIRE( car.getGenes() == string(DUMMY_ALTGENES) );
}

TEST_CASE( "CGaCar setGenes too short", "[CGaCar]" )
{
    GA::random.set_seed(42);

    GA::CGaCar car(DUMMY_ALTGENES);
    car.setGenes("ASDF");

    REQUIRE( car.getGenes() == string(DUMMY_GENES) );
}


TEST_CASE( "CGaCar CreateCar", "[CGaCar]" )
{
    GA::random.set_seed(42);

    GA::CGaCar car(DUMMY_ALTGENES);
    car.CreateCar(DUMMY_GENES);

    REQUIRE( car.getGenes() == string(DUMMY_GENES) );
}

namespace GA
{
    float DecodeGen(const int nLen, const char *genes, const float nMin, const float nMax, size_t &nPos);
    float map_values(const float in_min, const float in_max, const float out_min, const float out_max, const float val);
}

TEST_CASE( "DecodeGen", "[CCar]" ) {
    GA::random.set_seed(42);

    const char *genes = "JUYET";
    size_t nPos = 0;
    const float val = GA::DecodeGen(5, genes, -8, 8, nPos);
    Catch::Approx expected = Catch::Approx(3.80954f).epsilon(0.00001);
    REQUIRE( val == expected );
    REQUIRE( nPos == 5 );
}


TEST_CASE( "map_values", "[CCar]" ) {
    REQUIRE( GA::map_values(0, 100, 0, 1, 50) == 0.5f );
    REQUIRE( GA::map_values(0, 100, 0, 1, 0) == 0.0f );
    REQUIRE( GA::map_values(0, 100, 0, 1, 100) == 1.0f );
    REQUIRE( GA::map_values(0, 100, 0, 1, 101) == 1.0f );
    REQUIRE( GA::map_values(0, 100, 0, 1, -1) == 0.0f );
}


