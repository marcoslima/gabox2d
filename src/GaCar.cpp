#include <cstring>
#include "GaCar.h"
#include <stdexcept>
#include <global_random.h>


namespace GA
{
    float d26[] =
    {
        1.0f,
        26.0f,
        676.0f,
        17576.0f,
        456976.0f,
        11881376.0f,
        308915776.0f
    };

    float map_values(const float in_min,
                      const float in_max,
                      const float out_min,
                      const float out_max,
                      const float val)
    {
        const float inDelta = in_max - in_min;
        const float outDelta = out_max - out_min;

        const float result = std::clamp(out_min + (val - in_min) * outDelta / inDelta, out_min, out_max);

        if(result < out_min)
        {
            throw std::runtime_error("Valor fora do range");
        }
        return result;
    }

    float DecodeGen(const int nLen, 
                    const char *genes, 
                    const float nMin, 
                    const float nMax, 
                    size_t &nPos)
    {
        float dVal = 0;
        constexpr char A = 'A';
        for (int i = 0; i < nLen; i++)
        {
            // dVal += (*(genes+(nPos++)) - 'A') * d26[i]; // Linha original
            const char* addr = genes + nPos++;
            const char ch = *addr;
            const float mul = static_cast<float>(ch) - A;
            dVal += mul * d26[i];
        }


        //	1 dígito: A - Z ou 0 � 25, ou seja, d26[1]-1
        //	2 dígitos: Máximo: ZZ que é 25*26 + 25 = 675, ou seja, d26[2]-1

        return map_values(0, d26[nLen] - 1, nMin, nMax, dVal);
    }

    CCarDef::CRodaParams DecodeRoda(const char *genes, size_t &nPos)
    {
        constexpr int nLen = 4;
        // float x, float y, float r, float dens, float fric, float elas
        return {
            DecodeGen(nLen, genes, -8, 8, nPos),    // x
            DecodeGen(nLen, genes, 2, 8, nPos),     // y
            DecodeGen(nLen, genes, 0.2, 3, nPos),   // radius
            DecodeGen(nLen, genes, 0.1, 10, nPos),  // densidade
            DecodeGen(nLen, genes, 0.1, 2, nPos),   // friccao
            DecodeGen(nLen, genes, 0, 1, nPos)      // elasticidade
        };
    }

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

    void CGaCar::decode()
    {
#if 1
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
#else
        constexpr float ry = 7.0f+25;
        constexpr float py = 10.0f+25;
        constexpr float r1x = 5.0f+15;
        constexpr float r1y = ry;
        constexpr float r1r = 2.0f;
        constexpr float r2x = 13.0f+15;
        constexpr float r2y = ry;
        constexpr float r2r = 0.5f;
        constexpr float p1x = 7.0f+15;
        constexpr float p1y = py;
        constexpr float p1r = 1.0f;
        constexpr float p2x = 11.0f+15;
        constexpr float p2y = py;
        constexpr float p2r = 1.0f;
        constexpr float dens = 1.0;
        constexpr float fric = 1.0;
        constexpr float elas = 0.7;

        _carro._roda1 = CCarDef::CRodaParams(r1x, r1y, r1r, dens, fric, elas);
        _carro._roda2 = CCarDef::CRodaParams(r2x, r2y, r2r, dens, fric, elas);
        _carro._peso1 = CCarDef::CRodaParams(p1x, p1y, p1r, dens, fric, elas);
        _carro._peso2 = CCarDef::CRodaParams(p2x, p2y, p2r, dens, fric, elas);
        for(float & torque : _carro._torque)
        {
            constexpr float torque_amount = 10.0;
            torque = torque_amount;
        }
        for(int i = 0; i < 6; i++)
        {
            constexpr float damp = 0.0f;
            constexpr float freq = 1.5f;
            _carro._damp[i] = damp;
            _carro._freq[i] = freq;
        }
#endif
    }

    void CGaCar::Crossover(CGaCar &other)
    {
        const size_t nCross = 1 + GA::random.rand_int(0, GENES - 2);

        for (size_t i = nCross; i < GENES; i++)
        {
            const char tmp = getGene(i);
            setGene(i, other.getGene(i));
            other.setGene(i, tmp);
        }
    }
}
