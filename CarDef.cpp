#include "CarDef.h"

namespace MODEL
{
    CCarDef::CCarDef(void) {}

    CCarDef::~CCarDef(void) {}

    CCarDef::CRodaParams::CRodaParams() : CRodaParams(0, 0, 1, 1, 1, 1) {}

    CCarDef::CRodaParams::CRodaParams(const float x,
                          const float y,
                          const float r,
                          const float dens,
                          const float fric,
                          const float elas)
        : circle{x, y, r}
          , body{dens, fric, elas} {}
};
