#pragma once
#include <string>
#include "CarDef.h"

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

        // Decodifica o carro dos genes criando-o nas definições
        void _decode();

    public:
        CGaCar();

        explicit CGaCar(const char *szGenes);

        ~CGaCar();

        void getGenes(string &genes) const;

        [[nodiscard]] const char *getGenes() const
        {
            return _genes.c_str();
        }

        [[nodiscard]] char getGene(const size_t nIndex) const
        {
            return _genes[nIndex];
        }

        void setGene(const size_t nIndex, const char gen)
        {
            _genes[nIndex] = gen;
        }

        [[nodiscard]] string getGenesString() const;

        [[nodiscard]] double getPontuacao() const
        {
            return _pontos;
        }

        void setGenes(const char *genes);

        void setPontos(const double pontos)
        {
            _pontos = pontos;
        }

        void CreateCar(const char *genes = nullptr);


        // Processo evolutivo:
        void Crossover(CGaCar &other);
    };
}
