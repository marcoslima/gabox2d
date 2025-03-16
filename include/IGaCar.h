#pragma once

#include <string>

using namespace std;

namespace GA
{
    class IGaCar
    {
    public:
        virtual ~IGaCar() = default;

        [[nodiscard]] virtual string getGenes() const = 0;

        [[nodiscard]] virtual float getPontuacao() const = 0;

        virtual void decode() = 0; // Decodifica o carro dos genes (_genes) criando-o nas definições (_carro)
        virtual void calc_fitness(float contact1, float contact2, float velocity, float distance, float time, float max_t, bool is_dead) = 0;

        virtual void CreateCarFromGenes(const char *genes) = 0;
        virtual void CreateRandomCar() = 0;
    };
}
