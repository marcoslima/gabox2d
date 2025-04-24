#pragma once

#include <string>
#include <CarDef.h>
#include <IGaCar.h>

using namespace std;
using namespace MODEL;

namespace GA
{
    class CGaCar final : public IGaCar
    {
    public:
        // Codificado
        string _genes;

        // Suporte ao algoritmo genético:
        float _pontos;

    protected:
        void _generate_random_genes();

    public:
        CGaCar();
        explicit CGaCar(const char *szGenes);

        [[nodiscard]] string getGenes() const override;
        [[nodiscard]] float getPontuacao() const override;
        [[nodiscard]] ga_car_ptr_t clone() override;

        void CreateCarFromGenes(const string &genes) override;
        void CreateRandomCar() override;
        void CreateFromCarDef(const CCarDef &carDef);
        void calc_fitness(fitness_params_t fitness_params, float max_t) override;
    };
}
