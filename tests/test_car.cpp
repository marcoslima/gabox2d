#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <gmock/gmock.h>

#include <cstdint>
#include <iostream>
#include "car.h"
#include "GaCar.h"

#include <global_random.h>

bool operator==(const b2WorldId &lhs, const b2WorldId &rhs)
{
    return lhs.index1 == rhs.index1 && lhs.generation == rhs.generation;
}


TEST_CASE( "CCar instance", "[CCar]" ) 
{
    class MockCGaCar : public GA::IGaCar
    {
    public:
        MOCK_METHOD(string, getGenes, (), ( const override ));
        MOCK_METHOD(float, getPontuacao, (), (const override));
        MOCK_METHOD(void, decode, (), (override));
        MOCK_METHOD(void, calc_fitness, (float contact1, float contact2, float velocity, float distance, float time, float max_t, bool is_dead), (override));
        MOCK_METHOD(void, CreateCarFromGenes, (const char *genes), (override));
        MOCK_METHOD(void, CreateRandomCar, (), (override));
    };

    class MockCPhysCar : public PHYS::IPhysCar
    {
    public:
        MOCK_METHOD(void, destroy, (), (override));
        MOCK_METHOD(void, create, (b2WorldId WorldId, const CCarDef& carro), (override));
        MOCK_METHOD(void, init_simulation_vars, (), (override));
        MOCK_METHOD(void, phys_end_simulate, (), (override));
    };

    class MockCGrCar : public GUI::CGrCar
    {
    public:
    };

    MockCGaCar ga_car;
    MockCPhysCar phys_car;
    MockCGrCar gr_car;

    CCar sut;

    SECTION("CCar beginSimulate") 
    {
        testing::InSequence seq;
        b2WorldId testWorldId = {42, 17};

        {
            EXPECT_CALL(phys_car, destroy()).Times(1);
            EXPECT_CALL(ga_car, decode()).Times(1);
            EXPECT_CALL(phys_car, create(testWorldId, testing::_)).Times(1);
            EXPECT_CALL(phys_car, init_simulation_vars()).Times(1);
        }

        sut.beginSimulate(testWorldId);

        // Verify all expectations were met
        REQUIRE(testing::Mock::VerifyAndClearExpectations(&phys_car));
        REQUIRE(testing::Mock::VerifyAndClearExpectations(&ga_car));        
    }

    SECTION("CCar endSimulate") 
    {
        testing::InSequence seq;

        {
            EXPECT_CALL(phys_car, phys_end_simulate()).Times(1);
        }

        sut.endSimulate();

        // Verify all expectations were met
        REQUIRE(testing::Mock::VerifyAndClearExpectations(&phys_car));
    }
}

TEST_CASE( "CGaCar ctor with genes", "[CGaCar]" )
{
    GA::random.set_seed(42);

    const char *genes = "JUYET";
    GA::CGaCar car(genes);

    REQUIRE( car.getGenes() == string("JUYET") );
}

TEST_CASE("CCar beginSimulate", "[CCar]")
{
    // CCar car;
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


