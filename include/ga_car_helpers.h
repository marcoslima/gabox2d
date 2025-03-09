#pragma once

#include <CarDef.h>
#include <cstdint>
#include <stddef.h>


namespace GA
{
    float map_values(const float in_min,
                     const float in_max,
                     const float out_min,
                     const float out_max,
                     const float val);
                     
    float DecodeGen(const int nLen, 
                    const char *genes, 
                    const float nMin, 
                    const float nMax, 
                    size_t &nPos);

    MODEL::CCarDef::CRodaParams DecodeRoda(const char *genes, size_t &nPos);
}
