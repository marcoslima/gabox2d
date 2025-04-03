#pragma once

#include <string>
#include <fitness_params.h>
#include <CarDef.h>
#include <memory>


using namespace std;

namespace GA
{
    class IGaCar;
    using ga_car_ptr_t = shared_ptr<IGaCar>;

    class IGaCar
    {
    public:
        virtual ~IGaCar() = default;
        [[nodiscard]] virtual string getGenes() const = 0;
        [[nodiscard]] virtual float getPontuacao() const = 0;
        [[nodiscard]] virtual MODEL::CCarDef getCarro() const = 0;
        [[nodiscard]] virtual ga_car_ptr_t clone() = 0;
        virtual void decode() = 0; // Decodifica o carro dos genes (_genes) criando-o nas definições (_carro)
        virtual void calc_fitness(fitness_params_t fitness_params, float max_t) = 0;
        virtual void CreateCarFromGenes(const string &genes) = 0;
        virtual void CreateRandomCar() = 0;
    };
}
