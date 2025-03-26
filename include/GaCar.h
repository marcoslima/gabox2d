#pragma once

#include <string>
#include <CarDef.h>
#include <IGaCar.h>

using namespace std;
using namespace MODEL;

#define GENES 300
#define GENES_LEN (GENES+1)

namespace GA
{
    class CGaCar final : public IGaCar
    {
    public:
        // Codificado
        string _genes;

        // Decodificado
        CCarDef _carro;

        // Suporte ao algoritmo genético:
        float _pontos;

        // Internas
    protected:
        void _generate_random_genes();

    public:
        CGaCar();
        explicit CGaCar(const char *szGenes);

        [[nodiscard]] string getGenes() const override;
        [[nodiscard]] float getPontuacao() const override;
        [[nodiscard]] CCarDef getCarro() const override;
        [[nodiscard]] string crossover(const string &genes, size_t crosspoint) const override;
        [[nodiscard]] ga_car_ptr_t clone() override;

        void decode() override; // Decodifica o carro dos genes (_genes) criando-o nas definições (_carro)
        void CreateCarFromGenes(const string &genes) override;
        void CreateRandomCar() override;
        void calc_fitness(fitness_params_t fitness_params, float max_t) override;
        void mutate() override;
    };
}
