#pragma once
#include <memory>
#include <icar.h>
#include <IWorld.h>


namespace GUI
{
    class IGaBox2dDoc
    {
    public:
        virtual ~IGaBox2dDoc() = default;
        virtual void BeginSimulation() = 0;
        virtual void EndSimulation() = 0;
        virtual void Quit() = 0;
        virtual const icar_ptr_t &GetCar() = 0;
        virtual bool OnNewDocument(const CEnv &env) = 0;
        virtual void _start_world() = 0;
        virtual void OnEditEditarch() = 0;
        virtual void createRandomCar() = 0;
        virtual void simulateGaCar(const std::string &genes, float fitness, size_t generation) = 0;

        [[nodiscard]] virtual bool isSimulating() const = 0;
        [[nodiscard]] virtual PHYS::IWorldPtr GetWorld() const = 0;
        [[nodiscard]] virtual bool isQuit() const = 0;
        [[nodiscard]] virtual vec_vecs_t GetGround() const = 0;
        [[nodiscard]] virtual CEnv GetEnv() const = 0;
        [[nodiscard]] virtual size_t getGeneration() const = 0;
        [[nodiscard]] virtual float getFitness() const = 0;
    };

    using IGaBox2dDocPtr = std::shared_ptr<IGaBox2dDoc>;
}; //namespace GUI
