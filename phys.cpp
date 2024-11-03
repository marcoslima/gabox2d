#include "phys.h"

#include <memory>
#include <box2d/box2d.h>
#include <box2d/types.h>

namespace PHYS
{
    b2Vec2 b2Vec2_from_vec2_t(const MODEL::vec2_t& a)
    {
        return b2Vec2(static_cast<float>(a.x),
                      static_cast<float>(a.y));
    }

    b2WorldId _create_world()
    {
        // ReSharper disable once CppUseStructuredBinding
        b2WorldDef worldDef = b2DefaultWorldDef();

        worldDef.gravity = {0.0f, -10.0f};
        return b2CreateWorld(&worldDef);
    }

    vector<b2Vec2> _get_b2vecs_from_ground(const MODEL::CEnv& env)
    {
        const MODEL::vec_vecs_t GroundPoly = env.get_vecs();
        const int32_t nSize = GroundPoly.size(); // NOLINT(*-narrowing-conversions)
        vector<b2Vec2> vecVertices(nSize);
        for (size_t k = 0; k < nSize; k++)
        {
            vecVertices[k] = b2Vec2_from_vec2_t(GroundPoly[k]);
        }
        return vecVertices;
    }

    b2ChainDef _create_chain_def(const vector<b2Vec2>& vecVertices)
    {
        b2ChainDef chainDef = b2DefaultChainDef();
        chainDef.points = vecVertices.data();
        chainDef.count = vecVertices.size(); // NOLINT(*-narrowing-conversions)
        // chainDef.customColor = b2_colorSteelBlue;
        chainDef.isLoop = true;
        chainDef.friction = 1.0f;
        chainDef.restitution = 0.0f;

        return chainDef;
    }

    void _create_ground_body(CWorld& world, const MODEL::CEnv& env)
    {
        const auto vecVertices= _get_b2vecs_from_ground(env);
        const auto chainDef = _create_chain_def(vecVertices);

        const b2BodyDef bodyDef = b2DefaultBodyDef();
        world.m_GroundId = b2CreateBody( world.m_WorldId, &bodyDef );
        world.m_ChainId = b2CreateChain( world.m_GroundId, &chainDef );
    }

    void _create_ground(CWorld& world, const MODEL::CEnv& env)
    {
        _create_ground_body(world, env);

        // TODO: Fazer as paredes e o teto.

        // TODO: Verificar se é necessário.
        // b2Body_SetUserData(world.m_GroundId, ID_GROUND);
    }

    void buildWorld(const MODEL::CEnv& env, CWorld& world)
    {
        world.m_WorldId = _create_world();
        _create_ground(world, env);
    }
}
