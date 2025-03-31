#pragma once

#include <memory>
#include <string>
#include <IWorld.h>
#include <vec2f_t.h>
#include <list>


class ICar;
using icar_ptr_t = std::unique_ptr<ICar>;


class ICar
{
public:
    virtual ~ICar() = default;

    virtual void calc_fitness(float max_t) = 0;
    virtual void Medir(PHYS::IWorldPtr world, float max_t) = 0;
    virtual void resetPhysCar() = 0;
    virtual void createGaRandomCar() = 0;
    virtual void createGaFromGenes(const string &genes) = 0;
    virtual void beginSimulate(PHYS::IWorldPtr world) = 0;
    virtual void draw(void *pParams) const = 0;
    virtual void mutate() = 0;
    virtual void doStep() = 0;
    [[nodiscard]] virtual float getFitness() const = 0;
    [[nodiscard]] virtual bool doStepGetContinue() = 0;
    [[nodiscard]] virtual vec2f_t getCenter() const = 0;
    [[nodiscard]] virtual string getGenes() const = 0;
    [[nodiscard]] virtual bool operator<(const ICar &rhs) const = 0;
    [[nodiscard]] virtual float getT() const = 0;
    [[nodiscard]] virtual string deadReason() const = 0;
    [[nodiscard]] virtual icar_ptr_t crossover(const icar_ptr_t &rhs, size_t crosspoint) const = 0;
    [[nodiscard]] virtual icar_ptr_t clone() = 0;

};

class ICarFactory
{
public:
    virtual ~ICarFactory() = default;

    virtual icar_ptr_t createCarFromGenes(const std::string& genes) = 0;
    virtual icar_ptr_t createRandomCar() = 0;
};

using vec_car_t = vector<icar_ptr_t>;
using lst_car_t = list<icar_ptr_t>;
using car_factory_ptr_t = std::unique_ptr<ICarFactory>;
