#pragma once
#include <box2d/box2d.h>

#include "CarDef.h"

namespace PHYS
{
    class IPhysCar
    {
    public:
        virtual ~IPhysCar() = default;

        virtual void create(const b2WorldId WorldId, const MODEL::CCarDef& carro) = 0;
        virtual void destroy() = 0;
        virtual void reset() = 0;
        virtual void init_simulation_vars() = 0;
        virtual void phys_end_simulate() = 0;
        virtual bool simulation_step() = 0;
        virtual void init() = 0;
        [[nodiscard]] virtual b2Vec2 getCenter() const = 0;
        [[nodiscard]] virtual b2BodyId getR1() const = 0;
        [[nodiscard]] virtual b2BodyId getR2() const = 0;
        [[nodiscard]] virtual b2BodyId getP1() const = 0;
        [[nodiscard]] virtual b2BodyId getP2() const = 0;
        [[nodiscard]] virtual float getT() const = 0;
        virtual void Destroy() = 0;
    };
}
