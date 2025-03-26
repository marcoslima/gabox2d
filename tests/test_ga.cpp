#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <gmock/gmock.h>

#include <ga.h>
using namespace GA;

TEST_CASE("CGa", "[GA]")
{
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
        explicit MockCWorld(const unsigned id = 0U) : world_id(id) {}
    };
    class MockCar: public ICar
    {
        MOCK_METHOD(void, calc_fitness, (float max_t), (override));
        MOCK_METHOD(void, Medir, (PHYS::IWorld &world, float max_t), (override));
        MOCK_METHOD(float, getFitness, (), (const, override));
        MOCK_METHOD(void, resetPhysCar, (), (override));
        MOCK_METHOD(void, createGaRandomCar, (), (override));
        MOCK_METHOD(void, createGaFromGenes, (const string &genes), (override));
        MOCK_METHOD(void, beginSimulate, (PHYS::IWorld &world), (override));
        MOCK_METHOD(void, draw, (void *pParams), (const, override));
        MOCK_METHOD(IVec2f, getCenter, (), (const, override));
        MOCK_METHOD(string, getGenes, (), (const, override));
        // MOCK_METHOD(bool, operator<, (const ICar &rhs), (const, override));
        MOCK_METHOD(float, getT, (), (const, override));
        MOCK_METHOD(bool, doStep, (), (override));
        MOCK_METHOD(string, deadReason, (), (const, override));
    };
    class MockCarFactory : public ICarFactory
    {
        MOCK_METHOD(icar_ptr_t, createCarFromGenes, (const std::string& genes), (override));
        MOCK_METHOD(icar_ptr_t, createRandomCar, (), (override));
    };

    auto make_sut = [](unique_ptr<ICarFactory> factory = make_unique<MockCarFactory>()) -> unique_ptr<CGa>
    {
        return make_unique<CGa>(std::move(factory));
    };

    SECTION("Instance")
    {
        REQUIRE(make_sut()->m_melhores.empty());
    }

    SECTION("setParams")
    {
        const auto sut = make_sut();
        sut->setParams(90, 1, 70, 65, 0, 5, 60);
        REQUIRE(sut->getPopulacaoLen() == 90);
    }

    SECTION("BeginEvolve")
    {
        const auto sut = make_sut();
        sut->setParams(90, 1, 70, 65, 0, 5, 60);
        sut->BeginEvolve();
        REQUIRE(sut->getGeracao() == 1);
    }

    SECTION("Ordena")
    {
        MockCWorld world(42);
        class MockCarOrdena: public ICar
        {
        public:
            MOCK_METHOD(void, calc_fitness, (float max_t), (override));
            MOCK_METHOD(void, Medir, (PHYS::IWorld &world, float max_t), (override));
            MOCK_METHOD(float, getFitness, (), (const, override));
            MOCK_METHOD(void, resetPhysCar, (), (override));
            MOCK_METHOD(void, createGaRandomCar, (), (override));
            MOCK_METHOD(void, createGaFromGenes, (const string &genes), (override));
            MOCK_METHOD(void, beginSimulate, (PHYS::IWorld &world), (override));
            MOCK_METHOD(void, draw, (void *pParams), (const, override));
            MOCK_METHOD(IVec2f, getCenter, (), (const, override));
            MOCK_METHOD(string, getGenes, (), (const, override));
            MOCK_METHOD(float, getT, (), (const, override));
            MOCK_METHOD(bool, doStep, (), (override));
            MOCK_METHOD(string, deadReason, (), (const, override));
            bool operator<(const ICar &rhs) const override
                { return getFitness() < rhs.getFitness(); }
        };
        class MockCarFactoryOrdena : public ICarFactory
        {
        public:
            MOCK_METHOD(icar_ptr_t, createCarFromGenes, (const std::string& genes), (override));
            icar_ptr_t createRandomCar() override
            {
                return make_unique<MockCarOrdena>();
            }
        };
        auto factory = make_unique<MockCarFactoryOrdena>();
        const auto sut = make_sut(std::move(factory));

        sut->setParams(90, 1, 70, 65, 0, 5, 60);
        atomic<bool> stop_ga;
        sut->BeginEvolve();
        sut->Ordena(world, stop_ga);
        REQUIRE(sut->getGeracao() == 1);
    }
}
