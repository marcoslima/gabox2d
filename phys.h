#ifndef PHYS_H
#define PHYS_H
#include <box2d/math_functions.h>

#include "env.h"
#include "World.h"

namespace PHYS
{
    inline int ID_RODA1 = 1;
    inline int ID_RODA2 = 2;
    inline int ID_PESO1 = 3;
    inline int ID_PESO2 = 4;
    inline int ID_GROUND = 5;

    b2Vec2 b2Vec2_from_vec2_t(const MODEL::vec2_t& a);
    void buildWorld(const MODEL::CEnv& env, CWorld& world);
}

#endif //PHYS_H
