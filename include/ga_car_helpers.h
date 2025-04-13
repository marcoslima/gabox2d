#pragma once

#include <CarDef.h>


namespace GA
{
    float map_values(float in_min,
                     float in_max,
                     float out_min,
                     float out_max,
                     float val);
                     
    float DecodeGen(int nLen,
                    const char *genes,
                    float nMin,
                    float nMax,
                    size_t &nPos);

    MODEL::CCarDef::CRodaParams DecodeRoda(const char *genes, size_t &nPos);
}
