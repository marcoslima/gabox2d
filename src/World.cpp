#include <World.h>
#include <box2d/box2d.h>
#include <box2d/types.h>

#include "phys.h"


namespace PHYS
{
    CWorld nullWorld;

    CWorld::CWorld()
    {
        m_WorldId = b2_nullWorldId;
        m_GroundId = b2_nullBodyId;
        m_ChainId = b2_nullChainId;
    }

    void CWorld::create(const CEnv &env)
    {
        _create_world();
        _create_ground(env);
        _create_walls_and_ceilings(env, 5.0f);
    }

    bool CWorld::isValid()
    {
        return b2World_IsValid(m_WorldId);
    }

    void CWorld::destroy()
    {
        b2DestroyWorld(m_WorldId);
    }

    void *CWorld::getWorld()
    {
        return &m_WorldId;
    }

    void *CWorld::getGround()
    {
        return &m_GroundId;
    }

    void *CWorld::getChain()
    {
        return &m_ChainId;
    }

    ///////////////////////////
    /// Private methods
    void CWorld::_create_world()
    {
        // ReSharper disable once CppUseStructuredBinding
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.workerCount = 8;

        worldDef.gravity = {0.0f, -10.0f};
        m_WorldId = b2CreateWorld(&worldDef);
    }

    b2Vec2 b2Vec2_from_vec2_t(const MODEL::vec2_t &a)
    {
        return b2Vec2(static_cast<float>(a.x),
                      static_cast<float>(a.y));
    }

    vector<b2Vec2> _get_b2vecs_from_ground(const MODEL::CEnv &env)
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

    void CWorld::_make_wall(const b2Vec2 pos, const b2Vec2 size)
    {
        noconst = true;

        b2BodyDef wallDef = b2DefaultBodyDef();
        wallDef.type = b2_staticBody;

        const b2Polygon groundBox = b2MakeBox(size.x, size.y);
        wallDef.position = pos;

        const b2BodyId wallId = b2CreateBody(m_WorldId, &wallDef);

        const b2ShapeDef groundShapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(wallId, &groundShapeDef, &groundBox);

        b2Body_SetUserData(wallId, &ID_GROUND);
    }

    void CWorld::_create_left_wall(const MODEL::CEnv &env, const float tick)
    {
        const auto pos = b2Vec2(env._tlx - tick, (env._bry + env._tly) / 2);
        const auto size = b2Vec2(tick, fabs(env._bry - env._tly));
        _make_wall(pos, size);
    }

    void CWorld::_create_right_wall(const MODEL::CEnv &env, const float tick)
    {
        const auto pos = b2Vec2(env._brx + tick, (env._bry + env._tly) / 2);
        const auto size = b2Vec2(tick, fabs(env._bry - env._tly));
        _make_wall(pos, size);
    }

    void CWorld::_create_ceiling(const MODEL::CEnv &env, const float tick)
    {
        const auto pos = b2Vec2((env._brx + env._tlx) / 2, env._tly + tick);
        const auto size = b2Vec2(fabs(env._brx - env._tlx), tick);
        _make_wall(pos, size);
    }

    void CWorld::_create_ground(const MODEL::CEnv &env)
    {
        const auto vecVertices = _get_b2vecs_from_ground(env);
        auto groundDef = b2DefaultBodyDef();
        groundDef.type = b2_staticBody;
        m_GroundId = b2CreateBody(m_WorldId, &groundDef);
        b2Body_SetUserData(m_GroundId, &ID_GROUND);

        auto shapeDef = b2DefaultChainDef();
        // shapeDef.friction = 1.0f;
        // shapeDef.restitution = 0.0f;
        shapeDef.points = vecVertices.data();
        shapeDef.count = static_cast<int32_t>(vecVertices.size());
        shapeDef.isLoop = true;
        b2CreateChain(m_GroundId, &shapeDef);
    }

    void CWorld::_create_walls_and_ceilings(const MODEL::CEnv &env, const float tick)
    {
        _create_left_wall(env, tick);
        _create_right_wall(env, tick);
        _create_ceiling(env, tick);
    }
} // PHYS