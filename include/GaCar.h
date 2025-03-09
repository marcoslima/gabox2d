#pragma once
#include <string>
#include "CarDef.h"
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

        // Suporte ao algoritmo gen�tico:
        double _pontos;

        // Internas
    protected:
        void _generate_random_genes();

    public:
        CGaCar();

        // Decodifica o carro dos genes criando-o nas definições
        void decode();

        explicit CGaCar(const char *szGenes);

        ~CGaCar();

        [[nodiscard]] char getGene(const size_t nIndex) const
        {
            return _genes[nIndex];
        }

        void setGene(const size_t nIndex, const char gen)
        {
            _genes[nIndex] = gen;
        }

        [[nodiscard]] string getGenes() const;

        [[nodiscard]] double getPontuacao() const
        {
            return _pontos;
        }

        void setGenes(const char *genes);

        void setPontos(const double pontos)
        {
            _pontos = pontos;
        }

        void CreateCarFromGenes(const char *genes = nullptr);
        void CreateCar();


        // Processo evolutivo:
        void Crossover(CGaCar &other);
    };
}
