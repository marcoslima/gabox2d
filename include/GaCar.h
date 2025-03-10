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
        double _pontos;

        // Internas
    protected:
        void _generate_random_genes();

    public:
        CGaCar();
        explicit CGaCar(const char *szGenes);
        ~CGaCar();

        [[nodiscard]] string getGenes() const;
        [[nodiscard]] double getPontuacao() const;
        void decode(); // Decodifica o carro dos genes (_genes) criando-o nas definições (_carro)
        void setGenes(const char *genes);
        void setPontos(const double pontos);
        void CreateCarFromGenes(const char *genes = nullptr);
        void CreateCar();
    };
}
