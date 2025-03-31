#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <gmock/gmock.h>


#include <CRandom.h>

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
        MOCK_METHOD(string, getGenes, (), (const override));
        MOCK_METHOD(float, getPontuacao, (), (const override));
        MOCK_METHOD(CCarDef, getCarro, (), (const override));
        MOCK_METHOD(void, decode, (), (override));
        MOCK_METHOD(void, calc_fitness, (GA::fitness_params_t fitness_params, float max_t), (override));
        MOCK_METHOD(void, CreateCarFromGenes, (const string &genes), (override));
        MOCK_METHOD(void, CreateRandomCar, (), (override));
    };

    class MockCPhysCar : public PHYS::IPhysCar
    {
    public:
        MOCK_METHOD(void, reset, (), (override));
        MOCK_METHOD(bool, simulation_step, (), (override));
        MOCK_METHOD(void, measure, (PHYS::IWorld& world, const MODEL::CCarDef &carro, float max_t), (override));
        MOCK_METHOD(void, init, (), (override));
        MOCK_METHOD(void, fill_gr_car, (GUI::IGrCar &car), (override));
        MOCK_METHOD(GA::fitness_params_t, get_ga_fitness_params, (), (override));
        MOCK_METHOD(vec2f_t, getMassCenter, (), (const override));
        MOCK_METHOD(float, getCurrentX, (), (const override));
        MOCK_METHOD(b2BodyId, getR1, (), (const override));
        MOCK_METHOD(b2BodyId, getR2, (), (const override));
        MOCK_METHOD(b2BodyId, getP1, (), (const override));
        MOCK_METHOD(b2BodyId, getP2, (), (const override));
        MOCK_METHOD(float, getT, (), (const override));
        MOCK_METHOD(bool, isDead, (), (const override));
        MOCK_METHOD(string, deadReason, (), (const override));
        MOCK_METHOD(void, create, (PHYS::IWorld &world, const MODEL::CCarDef &carro), (override));
        MOCK_METHOD(void, destroy, (), (override));
        MOCK_METHOD(void, init_simulation_vars, (), (override));
    };

    class MockCGrCar : public GUI::IGrCar
    {
    public:
        MOCK_METHOD(void, setBroke, (bool), (override));
        MOCK_METHOD(void, draw, (void *pParams), (const override));
        MOCK_METHOD(void, setRoda1, (float center_x, float center_y, float radius, float angle, bool touch), (override));
        MOCK_METHOD(void, setRoda2, (float center_x, float center_y, float radius, float angle, bool touch), (override));
        MOCK_METHOD(void, setPeso1, (float center_x, float center_y, float radius, bool broke), (override));
        MOCK_METHOD(void, setPeso2, (float center_x, float center_y, float radius, bool broke), (override));
        MOCK_METHOD(void, setCenter, (float center_x, float center_y), (override));
    };

    class MockCWorld : public PHYS::IWorld
    {
        unsigned world_id;
        bool operator==(const MockCWorld &rhs) const { return world_id == rhs.world_id; }
        MOCK_METHOD(void*, getWorld, (), (override));
        MOCK_METHOD(void*, getGround, (), (override));
        MOCK_METHOD(void*, getChain, (), (override));
        MOCK_METHOD(void, create, (const MODEL::CEnv &env), (override));
        MOCK_METHOD(void, destroy, (), (override));
        MOCK_METHOD(bool, isValid, (), (override));
    public:
        MockCWorld() : world_id(0U) {}
    };

    auto ga_car = make_shared<MockCGaCar>();
    auto phys_car = make_shared<MockCPhysCar>();
    const auto gr_car = make_shared<MockCGrCar>();

    CCar sut(ga_car, phys_car, gr_car);

    SECTION("CCar beginSimulate") 
    {
        testing::InSequence seq;
        MockCWorld testWorld;

        {
            EXPECT_CALL(*ga_car, decode()).Times(1);
            EXPECT_CALL(*phys_car, create(testing::Ref(testWorld), testing::_)).Times(1);
        }

        sut.beginSimulate(testWorld);

        // Verify all expectations were met
        REQUIRE(testing::Mock::VerifyAndClearExpectations(&phys_car));
        REQUIRE(testing::Mock::VerifyAndClearExpectations(&ga_car));
    }
}

TEST_CASE( "CGaCar ctor with genes", "[CGaCar]" )
{
    GA::random.set_seed(42);

    const char *genes = "JUYET";
    GA::CGaCar car(genes);

    REQUIRE( car.getGenes() == string("JUYET") );
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


