//
// Created by marcos on 11/3/24.
//

#ifndef PHYS_H
#define PHYS_H
#include <box2d/id.h>
#include <box2d/math_functions.h>

#include "env.h"
#include "World.h"

namespace PHYS
{
    b2Vec2 b2Vec2_from_vec2_t(const MODEL::vec2_t& a);
    void buildWorld(const MODEL::CEnv& env, PHYS::CWorld& world);
}

#endif //PHYS_H
