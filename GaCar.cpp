#include <random>
#include <cstring>
#include "GaCar.h"

namespace GA
{
    double d26[] =
    {
        1,
        26,
        676,
        17576,
        456976,
        11881376,
        308915776
    };

    int randInt(const int max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, max);
        return dis(gen);
    }

    char randChar(const char min, const char max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<char> dis(min, max);
        return dis(gen);
    }

    double map_values(const double in_min,
                      const double in_max,
                      const double out_min,
                      const double out_max,
                      const double val)
    {
        const double inDelta = in_max - in_min;
        const double outDelta = out_max - out_min;

        return out_min + (val - in_min) * outDelta / inDelta;
    }

    double DecodeGen(const int nLen, const char *genes, const double nMin, const double nMax, size_t &nPos)
    {
        double dVal = 0;
        for (int i = 0; i < nLen; i++)
        {
            char ch = *(genes + (nPos++));
            ch -= 'A';
            dVal += ch * d26[i];
            //dVal += (*(genes+(nPos++)) - 'A') * d26[i];
        }


        //	1 d�gito: A - Z ou 0 � 25, ou seja, d26[1]-1
        //	2 d�gitos: M�ximo: ZZ que � 25*26 + 25 = 675, ou seja, d26[2]-1

        return (double) map_values(0, d26[nLen] - 1, nMin, nMax, dVal);
    }

    CCarDef::CRoda DecodeRoda(const char *genes, size_t &nPos)
    {
        const int nLen = 4;
        return {
            DecodeGen(nLen, genes, -8, 8, nPos),
            DecodeGen(nLen, genes, 2, 8, nPos),
            DecodeGen(nLen, genes, 0.2, 3, nPos),
            DecodeGen(nLen, genes, 0.1, 10, nPos),
            DecodeGen(nLen, genes, 0.1, 2, nPos),
            DecodeGen(nLen, genes, 0, 1, nPos)
        };
    }

    CGaCar::CGaCar(): _pontos(0)
    {
        _generate_random_genes();
    }

    CGaCar::CGaCar(const char *szGenes)
        : _genes(szGenes)
          , _pontos(0) {}

    CGaCar::~CGaCar(void) = default;

    void CGaCar::_generate_random_genes()
    {
        _genes.clear();

        for (int i = 0; i < GENES; i++)
        {
            _genes.push_back(randChar('A', 'Z'));
        }
    }


    void CGaCar::getGenes(string &genes) const
    {
        genes = string(_genes);
    }

    string CGaCar::getGenesString() const
    {
        return {_genes};
    }

    void CGaCar::setGenes(const char *genes)
    {
        if (genes == nullptr || strlen(genes) != GENES)
            _generate_random_genes();
        else
            _genes = genes;
    }

    void CGaCar::CreateCar(const char *genes)
    {
        setGenes(genes);
    }

    void CGaCar::_decode()
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
            constexpr double dMaxDamp = 2.0;
            constexpr double dMinDamp = 0.0;
            constexpr double dMaxFreq = 30.0;
            constexpr double dMinFreq = 0.1;
            constexpr double dFp = 50;
            if (i < 4) _carro._torque[i] = DecodeGen(nLen, _genes.c_str(), -dFp, dFp, nPos);
            _carro._freq[i] = DecodeGen(nLen, _genes.c_str(), dMinFreq, dMaxFreq, nPos);
            _carro._damp[i] = DecodeGen(nLen, _genes.c_str(), dMinDamp, dMaxDamp, nPos);
        }
    }

    void CGaCar::Crossover(CGaCar &other)
    {
        const size_t nCross = 1 + randInt(GENES - 2);

        for (size_t i = nCross; i < GENES; i++)
        {
            const char tmp = getGene(i);
            setGene(i, other.getGene(i));
            other.setGene(i, tmp);
        }
    }
}; // namespace GA
