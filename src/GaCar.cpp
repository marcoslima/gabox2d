#include <cstring>
#include "GaCar.h"
#include <stdexcept>
#include <global_random.h>
#include <iostream>
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

    CGaCar::~CGaCar() = default;

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

    double CGaCar::getPontuacao() const
    {
        return _pontos;
    }

    void CGaCar::setGenes(const char *genes)
    {
        if (strlen(genes) != GENES)
        {
            cout << "Invalid genes size: " << strlen(genes) << endl;
            return;
        }

        _genes = genes;
    }

    void CGaCar::setPontos(const double pontos)
    {
        _pontos = pontos;
    }

    void CGaCar::CreateCarFromGenes(const char *genes)
    {
        setGenes(genes);
    }

    void CGaCar::CreateCar()
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
}
