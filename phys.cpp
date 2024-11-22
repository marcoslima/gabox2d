#include "phys.h"
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
        worldDef.workerCount = 8;

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
        auto groundDef = b2DefaultBodyDef();
        groundDef.type = b2_staticBody;
        world.m_GroundId = b2CreateBody(world.m_WorldId, &groundDef);
        b2Body_SetUserData(world.m_GroundId, &ID_GROUND);

        auto shapeDef = b2DefaultChainDef();
        shapeDef.friction = 1.0f;
        shapeDef.restitution = 0.0f;
        shapeDef.points = vecVertices.data();
        shapeDef.count = static_cast<int32_t>(vecVertices.size());
        shapeDef.isLoop = true;
        b2CreateChain(world.m_GroundId, &shapeDef);
    }

    void _make_left_wall(const CWorld &world, const MODEL::CEnv &env) {
        constexpr float tick = 5.0f;
        b2BodyDef wallDef = b2DefaultBodyDef();
        wallDef.type = b2_staticBody;
        const float height = fabs(env._bry - env._tly);
        const b2Polygon groundBox = b2MakeBox(tick, height);
        wallDef.position = b2Vec2(env._tlx-tick, (env._bry + env._tly) / 2);

        const b2BodyId wallId = b2CreateBody(world.m_WorldId, &wallDef);

        const b2ShapeDef groundShapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(wallId, &groundShapeDef, &groundBox);

        b2Body_SetUserData(wallId, &ID_GROUND);
    }

    void _make_right_wall(const CWorld &world, const MODEL::CEnv &env) {
        constexpr float tick = 5.0f;
        b2BodyDef wallDef = b2DefaultBodyDef();
        wallDef.type = b2_staticBody;
        const float height = fabs(env._bry - env._tly);
        const b2Polygon groundBox = b2MakeBox(tick, height);
        wallDef.position = b2Vec2(env._brx+tick, (env._bry + env._tly) / 2);

        const b2BodyId wallId = b2CreateBody(world.m_WorldId, &wallDef);

        const b2ShapeDef groundShapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(wallId, &groundShapeDef, &groundBox);

        b2Body_SetUserData(wallId, &ID_GROUND);
    }

    void _make_ceiling(const CWorld &world, const MODEL::CEnv &env) {
        constexpr float tick = 5.0f;
        b2BodyDef wallDef = b2DefaultBodyDef();
        wallDef.type = b2_staticBody;
        const float width = fabs(env._brx - env._tlx);
        const b2Polygon groundBox = b2MakeBox(width, tick);
        wallDef.position = b2Vec2((env._brx + env._tlx) / 2, env._tly+tick);

        const b2BodyId wallId = b2CreateBody(world.m_WorldId, &wallDef);

        const b2ShapeDef groundShapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(wallId, &groundShapeDef, &groundBox);

        b2Body_SetUserData(wallId, &ID_GROUND);
    }


    void _create_ground(CWorld& world, const MODEL::CEnv& env)
    {
        _create_ground_body(world, env);

        // Paredes e teto:
        _make_left_wall(world, env);
        _make_right_wall(world, env);
        _make_ceiling(world, env);
    }

    void buildWorld(const MODEL::CEnv& env, CWorld& world)
    {
        world.m_WorldId = _create_world();
        _create_ground(world, env);
    }
}
