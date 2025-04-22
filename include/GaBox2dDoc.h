#pragma once
#include <IGaBox2dDoc.h>
#include "car.h"
#include "ga.h"
#include "env.h"

namespace GUI
{
    extern vec_vecs_t g_vecGroundPoints;

    class CGaBox2dDoc final : public IGaBox2dDoc
    {
        bool _isSimulating = false;
        PHYS::IWorldPtr _world;
        icar_ptr_t m_car;
        size_t _generation = -1;
        float _fitness = 0.0f;

    public:
        bool m_bQuit = false;
        vec_car_t m_populacao;
        CEnv m_env;
        vec_vecs_t m_vecGround;

        CGaBox2dDoc();

        ~CGaBox2dDoc() override;

        [[nodiscard]] bool isSimulating() const override;

        [[nodiscard]] PHYS::IWorldPtr GetWorld() const override;

        [[nodiscard]] bool isQuit() const override;

        [[nodiscard]] vec_vecs_t GetGround() const override;

        [[nodiscard]] CEnv GetEnv() const override;

        [[nodiscard]] size_t getGeneration() const override;

        [[nodiscard]] float getFitness() const override;

        void BeginSimulation() override;

        void EndSimulation() override;

        void Quit() override;

        const icar_ptr_t &GetCar() override;

        bool OnNewDocument(const CEnv &env) override;

        void _start_world() override;

        void OnEditEditarch() override;

        void createRandomCar() override;

        void simulateGaCar(const std::string &genes, float fitness, size_t generation) override;
    };
}