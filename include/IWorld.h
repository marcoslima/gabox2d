#pragma once
#include "env.h"
using namespace MODEL;

namespace PHYS
{
    class IWorld
    {
    public:
        virtual ~IWorld() = default;

        virtual void* getWorld() = 0;
        virtual void* getGround() = 0;
        virtual void* getChain() = 0;
        virtual void create(const CEnv &env) = 0;
        virtual void destroy() = 0;

        virtual bool isValid() = 0;
    };
}