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

        void decode() override; // Decodifica o carro dos genes (_genes) criando-o nas definições (_carro)
        void calc_fitness(float contact1, float contact2, float velocity, float distance, float time, float max_t, bool is_dead) override;

        void CreateCarFromGenes(const char *genes) override;
        void CreateRandomCar() override;
    };
}
