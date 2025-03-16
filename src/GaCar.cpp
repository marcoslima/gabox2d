#include <cstring>
#include "GaCar.h"
#include <stdexcept>
#include <global_random.h>
#include <iostream>
#include <sstream>
#include <ga_car_helpers.h>


namespace GA
{
    CGaCar::CGaCar()
        : _pontos(0)
    {
        _generate_random_genes();
    }

    CGaCar::CGaCar(const char *szGenes)
        : _genes(szGenes)
        , _pontos(0) {}

    void CGaCar::_generate_random_genes()
    {
        _genes.clear();

        for (int i = 0; i < GENES; i++)
        {
            _genes.push_back(random.discrete_random('A', 'Z'));
        }
    }

    string CGaCar::getGenes() const
    {
        return _genes;
    }

    float CGaCar::getPontuacao() const
    {
        return _pontos;
    }

    void CGaCar::CreateCarFromGenes(const char *genes)
    {
        if (strlen(genes) != GENES)
        {
            stringstream ss;
            ss << "Invalid genes size: " << strlen(genes);
            throw(length_error(ss.str().c_str()));
        }

        _genes = genes;
    }

    void CGaCar::CreateRandomCar()
    {
        _generate_random_genes();
    }

    void CGaCar::decode()
    {
        size_t nPos = 0;

        // Obtemos os body's e shape's def's dos genes:
        _carro._roda1 = DecodeRoda(_genes.c_str(), nPos);
        _carro._roda2 = DecodeRoda(_genes.c_str(), nPos);
        _carro._peso1 = DecodeRoda(_genes.c_str(), nPos);
        _carro._peso2 = DecodeRoda(_genes.c_str(), nPos);

        for (int i = 0; i < 6; i++)
        {
            constexpr int nLen = 4;
            constexpr float dMaxDamp = 2.0f;
            constexpr float dMinDamp = 0.0f;
            constexpr float dMaxFreq = 30.0f;
            constexpr float dMinFreq = 0.1f;
            constexpr float dFp = 50.0f;
            if (i < 4) _carro._torque[i] = DecodeGen(nLen, _genes.c_str(), -dFp, dFp, nPos);
            _carro._freq[i] = DecodeGen(nLen, _genes.c_str(), dMinFreq, dMaxFreq, nPos);
            _carro._damp[i] = DecodeGen(nLen, _genes.c_str(), dMinDamp, dMaxDamp, nPos);
        }
    }

    void CGaCar::calc_fitness(const float contact1,
                              const float contact2,
                              const float velocity,
                              const float distance,
                              const float time,
                              const float max_t,
                              const bool is_dead)
    {
        /*
            A pontuação é meio difícil porque, para ser absoluta, não pode
            depender da população.
            Mas se não depender, é muito difícil normalizar as partes (c1, c2, v, d e t).
            Sem normalizar, a distância, por exemplo, que pode ter valores grandes,
            será mais importante que os outros parâmetros de avaliação.

            Para resolver isso, vamos fazer o fitness como sendo a distância
            euclidiana de um vetor composto pelos parâmetros de avaliação a um
            vetor constante ideal.

            O vetor será (c1, c2, v, d, t).
            O vetor objetivo ideal será: (t_max, t_max, 1000, 1000, 0).
            Ou seja,
                . o tempo de contato das rodas é o máximo possível
                . A velocidade é a máxima possível
                . A distância percorrida é a máxima possível
                . O tempo gasto é o mínimo. No caso, nem é possível, pois é zero.

            Para não gastar um sqrt à toa, faremos o quadrado da distância.
        */
        const float p1 = max_t - contact1;
        const float p2 = max_t - contact2;
        const float p3 = 1000 - velocity;
        const float p4 = 1000 - distance;
        const float p5 = time; // 0 - t = -t, mas como será ao quadrado, deixa t mesmo.

        float pts = (p1*p1 + p2*p2 + p3*p3 + p4*p4 + p5*p5);

        // Se quebrou, vale um d�cimo de um que n�o quebrou:
        if(is_dead) pts *= 10;

        _pontos = pts;
    }
}
