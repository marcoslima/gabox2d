#pragma once

#include <box2d/box2d.h>
#include <box2d/id.h>

#include <IWorld.h>
#include <env.h>

using namespace std;

namespace PHYS {

    class CWorld final : public IWorld
    {
        static constexpr auto tick = 5.0f;
        bool noconst = false;
        b2WorldId m_WorldId = b2_nullWorldId;
        b2BodyId m_GroundId = b2_nullBodyId;
        b2ChainId m_ChainId = b2_nullChainId;

        void _create_world();
        void _create_ground(const CEnv &env);
        void _create_walls_and_ceilings(const CEnv &env);
        void _create_left_wall(const CEnv &env);
        void _create_right_wall(const CEnv &env);
        void _create_ceiling(const CEnv &env);
        void _make_wall(b2Vec2 pos, b2Vec2 size);


    public:
        CWorld();
        [[nodiscard]] void* getWorld() override;
        [[nodiscard]] void* getGround() override;
        [[nodiscard]] void* getChain() override;

        void create(const CEnv &env) override;

        bool isValid() override;

        void destroy() override;
    };

    extern CWorld nullWorld;

} // PHYS
