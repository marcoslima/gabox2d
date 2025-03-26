#pragma once
#include <fitness_params.h>
#include <box2d/box2d.h>

#include "CarDef.h"
#include <IGrCar.h>
#include <IVec2f.h>

namespace PHYS
{
    class IPhysCar;
    using phys_car_ptr_t = shared_ptr<IPhysCar>;

    class IPhysCar
    {
    public:
        virtual ~IPhysCar() = default;

        virtual void create(IWorld &world, const MODEL::CCarDef& carro) = 0;
        virtual void destroy() = 0;
        virtual void reset() = 0;
        virtual void init_simulation_vars() = 0;
        virtual bool simulation_step() = 0;
        virtual void measure(IWorld& world, const MODEL::CCarDef &carro, float max_t) = 0;
        virtual void init() = 0;
        virtual void fill_gr_car(GUI::IGrCar& car) = 0;
        virtual GA::fitness_params_t get_ga_fitness_params() = 0;
        [[nodiscard]] virtual IVec2f getMassCenter() const = 0;
        [[nodiscard]] virtual float getCurrentX() const = 0;
        [[nodiscard]] virtual float getT() const = 0;
        [[nodiscard]] virtual bool isDead() const = 0;
        [[nodiscard]] virtual string deadReason() const = 0;
        [[nodiscard]] virtual phys_car_ptr_t clone() const = 0;
    };
}
