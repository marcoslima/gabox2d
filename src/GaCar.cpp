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
            _genes.push_back(random.discrete_random('0', '1'));
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

    CCarDef CGaCar::getCarro() const
    {
        return _carro;
    }

    ga_car_ptr_t CGaCar::clone()
    {
        auto car = make_shared<CGaCar>();
        car->_genes = _genes;
        car->_pontos = _pontos;
        car->_carro = _carro;
        return car;
    }

    void CGaCar::CreateCarFromGenes(const string &genes)
    {
        if (genes.size() != GENES)
        {
            stringstream ss;
            ss << "Invalid genes size: " << genes.size();
            throw(length_error(ss.str().c_str()));
        }

        _genes = genes;
    }

    void CGaCar::CreateRandomCar()
    {
        _generate_random_genes();
    }

    void CGaCar::calc_fitness(const fitness_params_t fitness_params, const float max_t)
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
        const float p1 = max_t - fitness_params.contact1;
        const float p2 = max_t - fitness_params.contact2;
        const float p3 = 1000 - fitness_params.velocity;
        const float p4 = 1000 - fitness_params.distance;
        const float p5 = fitness_params.time; // 0 - t = -t, mas como será ao quadrado, deixa t mesmo.

        float pts = (p1*p1 + p2*p2 + p3*p3 + p4*p4 + p5*p5);

        // Se quebrou, vale um décimo de um que não quebrou:
        if(fitness_params.is_dead) pts *= 10;

        _pontos = pts;
    }

    CCarDef::CRodaParams CGaCar::decodeBinaryWheel(const string& genes, size_t& pos)
    {
        constexpr CCarDef::circle_params_bits circle_bits;
        constexpr CCarDef::body_params_bits body_bits;
        return {
            decodeBinaryValue(genes, pos, circle_bits.x, -8, 8),    // x
            decodeBinaryValue(genes, pos, circle_bits.y, 2, 8),     // y
            decodeBinaryValue(genes, pos, circle_bits.raio, 0.2, 3),   // radius
            decodeBinaryValue(genes, pos, body_bits.densidade, 0.1, 10),  // densidade
            decodeBinaryValue(genes, pos, body_bits.friccao, 0.1, 2),   // friccao
            decodeBinaryValue(genes, pos, body_bits.elasticidade, 0, 1)      // elasticidade
        };
    }

    float CGaCar::decodeBinaryValue(const string& genes, size_t& pos, const size_t bits, const float min, const float max)
    {
        // Extract the binary substring
        unsigned int value = 0;
        const auto genes_size = genes.size();
        for (size_t i = 0; i < bits && pos < genes_size; i++, pos++)
        {
            value = (value << 1) | (genes[pos] == '1' ? 1 : 0);
        }

        // Map binary value to float range
        const unsigned int maxValue = (1 << bits) - 1;
        return min + (max - min) * static_cast<float>(value) / static_cast<float>(maxValue);
    }
    void CGaCar::decode()
    {
        size_t pos = 0;

        // Decode wheels and weights
        _carro._roda1 = decodeBinaryWheel(_genes, pos);
        _carro._roda2 = decodeBinaryWheel(_genes, pos);
        _carro._peso1 = decodeBinaryWheel(_genes, pos);
        _carro._peso2 = decodeBinaryWheel(_genes, pos);

        // Decode joint parameters (torque, frequency, damping)
        constexpr int nMaxTorqueIndex = 3;
        constexpr float dMaxDamp = 2.0f;
        constexpr float dMinDamp = 0.0f;
        constexpr float dMaxFreq = 30.0f;
        constexpr float dMinFreq = 0.1f;
        constexpr float dFp = 50.0f;
        const CCarDefBits bits;
        for (int i = 0; i < 6; i++)
        {
            // Only decode torque for the first 4 indices
            if (i <= nMaxTorqueIndex) {
                _carro._torque[i] = decodeBinaryValue(_genes, pos, bits._torque, -dFp, dFp);
            }

            _carro._freq[i] = decodeBinaryValue(_genes, pos, bits._freq, dMinFreq, dMaxFreq);
            _carro._damp[i] = decodeBinaryValue(_genes, pos, bits._damp, dMinDamp, dMaxDamp);
        }
    }
}
