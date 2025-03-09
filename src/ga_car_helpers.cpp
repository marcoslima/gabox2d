#include<algorithm>
#include<stdexcept>
#include "ga_car_helpers.h"


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

    MODEL::CCarDef::CRodaParams DecodeRoda(const char *genes, size_t &nPos)
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
} // namespace GA