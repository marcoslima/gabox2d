#pragma once

#include <string>
#include <CarDef.h>
#include <global_random.h>

using namespace std;
using namespace MODEL;

#define GENES 300
#define GENES_LEN (GENES+1)

namespace GA
{
    class CGaCar
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
        ~CGaCar();

        void setGenes(const char *genes);
        [[nodiscard]] string getGenes() const;

        void setPontos(float pontos);
        [[nodiscard]] float getPontuacao() const;

        virtual void decode(); // Decodifica o carro dos genes (_genes) criando-o nas definições (_carro)
        void calc_fitness(float contact1, float contact2, float velocity, float distance, float time, float max_t, bool is_dead);

        void CreateCarFromGenes(const char *genes = nullptr);
        void CreateRandomCar();
    };
}
