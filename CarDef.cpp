#include "CarDef.h"

namespace MODEL
{
    CCarDef::CCarDef(void) {}

    CCarDef::~CCarDef(void) {}

    CCarDef::CRoda::CRoda() : CRoda(0, 0, 1, 1, 1, 1) {}

    CCarDef::CRoda::CRoda(const float x,
                          const float y,
                          const float r,
                          const float dens,
                          const float fric,
                          const float elas)
        : c{x, y, r}
          , b{dens, fric, elas} {}
};
