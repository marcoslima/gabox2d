#include "PhysCar.h"

#include <iostream>
#include <ostream>

namespace PHYS
{
    int ID_RODA1 = 1;
    int ID_RODA2 = 2;
    int ID_PESO1 = 3;
    int ID_PESO2 = 4;
    int ID_GROUND = 5;

    CPhysCar::CPhysCar() // NOLINT(*-pro-type-member-init)
    {
        _init();
    }

    CPhysCar::~CPhysCar() = default;

    b2_def_t TranslateRoda(const CCarDef::CRoda &roda)
    {
        b2_def_t ret;
        ret.bd = b2DefaultBodyDef();
        ret.bd.position = b2Vec2(roda.c.x, roda.c.y);
        ret.bd.type = b2_dynamicBody;
        ret.sd.radius = roda.c.raio;

        return ret;
    }
    b2BodyId CreateRoda(const b2WorldId WorldId,
                        const car_t& car_def,
                        const CCarDef::CRoda &roda,
                        void* IdBody)
    {
        const b2BodyId RodaId = b2CreateBody(WorldId, &car_def.R1.bd);
        b2ShapeDef shape_def = b2DefaultShapeDef();
        shape_def.friction = roda.b.friccao;
        shape_def.density = roda.b.densidade;
        shape_def.restitution = roda.b.elasticidade;
        b2CreateCircleShape(RodaId, &shape_def, &car_def.R1.sd);
        b2Body_SetUserData(RodaId, IdBody);

        return RodaId;
    }

    void CPhysCar::_translate_rodas_e_pesos(const CCarDef& carro)
    {
        _car_def.R1 = TranslateRoda(carro._roda1);
        _car_def.R2 = TranslateRoda(carro._roda2);
        _car_def.P1 = TranslateRoda(carro._peso1);
        _car_def.P2 = TranslateRoda(carro._peso2);
    }
    void CPhysCar::_copy_dyn_params(const CCarDef &carro)
    {
        copy(begin(carro._torque), end(carro._torque), begin(_car_def.torque));
        copy(begin(carro._freq), end(carro._freq), begin(_car_def.freq));
        copy(begin(carro._damp), end(carro._damp), begin(_car_def.damp));
    }

    void CPhysCar::_create_rodas_e_pesos(const CCarDef &carro) {
        //////////////////////////////////////////////
        // Criação dos objetos:
        m_Roda1Id = CreateRoda(m_World.m_WorldId, _car_def, carro._roda1, &ID_RODA1);
        m_Roda2Id = CreateRoda(m_World.m_WorldId, _car_def, carro._roda2, &ID_RODA2);
        m_Peso1Id = CreateRoda(m_World.m_WorldId, _car_def, carro._peso1, &ID_PESO1);
        m_Peso2Id = CreateRoda(m_World.m_WorldId, _car_def, carro._peso2, &ID_PESO2);
    }

    void CPhysCar::_set_torques() {
        // Torques:
        _trqA = _car_def.torque[0];
        _trqB = _car_def.torque[1];
        _trqC = _car_def.torque[2];
        _trqD = _car_def.torque[3];
    }

    b2JointId CPhysCar::_create_joint(const b2BodyId bodyA,
                                      const b2BodyId bodyB,
                                      const int param_index) const
    {
        b2DistanceJointDef jd;
        jd = b2DefaultDistanceJointDef();
        jd.bodyIdA = bodyA;
        jd.bodyIdB = bodyB;
        jd.localAnchorA = b2Body_GetWorldPoint(bodyA, b2Vec2(0, 0));
        jd.localAnchorB = b2Body_GetWorldPoint(bodyB, b2Vec2(0, 0));
        jd.collideConnected = true;
        jd.hertz = _car_def.freq[param_index];
        jd.dampingRatio = _car_def.damp[param_index];
        return b2CreateDistanceJoint(m_World.m_WorldId, &jd);
    }

    void CPhysCar::_create_joints() {
        m_Jc1c2Id = _create_joint(m_Roda1Id, m_Roda2Id, 0);
        m_Jc1p1Id = _create_joint(m_Roda1Id, m_Peso1Id, 1);
        m_Jc1p2Id = _create_joint(m_Roda1Id, m_Peso2Id, 2);
        m_Jc2p1Id = _create_joint(m_Roda2Id, m_Peso1Id, 3);
        m_Jc2p2Id = _create_joint(m_Roda2Id, m_Peso2Id, 4);
        m_Jp1p2Id = _create_joint(m_Peso1Id, m_Peso2Id, 5);
    }

    void CPhysCar::_create(const b2WorldId WorldId, const CCarDef &carro)
    {
        _verificar_step();
        if (b2World_IsValid(WorldId))
            _destroy();

        m_World.m_WorldId = WorldId;
        _translate_rodas_e_pesos(carro);
        _copy_dyn_params(carro);
        _create_rodas_e_pesos(carro);
        _create_joints();
        _set_torques();
    }

#if 0
  #define ISBODY(s1,s2,id) (s1 == id  || s2 == id)

    void CContactListener::Add(const b2ContactPoint *point)
    {
        int s1 = *((int *) point->shape1->GetBody()->GetUserData());
        int s2 = *((int *) point->shape2->GetBody()->GetUserData());

        bool bGround = ISBODY(s1, s2, ID_GROUND);
        bool bR1 = ISBODY(s1, s2, ID_RODA1);
        bool bR2 = ISBODY(s1, s2, ID_RODA2);
        bool bP1 = ISBODY(s1, s2, ID_PESO1);
        bool bP2 = ISBODY(s1, s2, ID_PESO2);

        if ((bGround && (bP1 || bP2)) || (bR1 && bR2))
            m_bDead = true;

        m_cVel = point->velocity;
        m_cPos = point->position;
        /*
            if(point->velocity.Length() > 100.0)
            {
                m_bDead = true;
            }
        */
    }

    void CContactListener::Persist(const b2ContactPoint *point)
    {
        const int s1 = *const_cast<int *>(point->shape1->GetBody()->GetUserData());
        const int s2 = *const_cast<int *>(point->shape2->GetBody()->GetUserData());

        const bool bGround = ISBODY(s1, s2, ID_GROUND);
        bool bR1 = ISBODY(s1, s2, ID_RODA1);
        bool bR2 = ISBODY(s1, s2, ID_RODA2);

        if (bGround)
        {
            m_bContactR1 |= bR1;
            m_bContactR2 |= bR2;
        }
    }
#endif


    // Executa um passo da simulação e retorna false se o carro morreu.
    bool CPhysCar::_simulation_step()
    {
        _bInStep = true;
        _simulation_pre_tick();

        // TODO: Converter o contactlistener
        // _cl.m_bDead = false;
        // _cl.m_bContactR1 = false;
        // _cl.m_bContactR2 = false;

        // b2World_Step(m_World.m_WorldId, _timeStep, _iterations);
        b2World_Step(m_World.m_WorldId, _timeStep, 8);

#if 0 // TODO: Converter o contactlistener
          // Est� vivo ainda?
        m_bDead |= _cl.m_bDead;
        _cVel = _cl.m_cVel;
        _cPos = _cl.m_cPos;
        m_bContactR1 = _cl.m_bContactR1;
        m_bContactR2 = _cl.m_bContactR2;

        // Contato das rodas:
        if (m_bContactR1)
        {
            m_acum_contatoR1 += _timeStep;
            _last_contact_r1 = _t;
        }
        if (m_bContactR2)
        {
            m_acum_contatoR2 += _timeStep;
            _last_contact_r2 = _t;
        }

        _no_contact_time_r1 = _t - _last_contact_r1;
        _no_contact_time_r2 = _t - _last_contact_r2;

        if (_no_contact_time_r1 > 2 || _no_contact_time_r2 > 2)
        {
            m_bDead = true;
        }
#endif


        if (m_bDead && b2Joint_IsValid(m_Jp1p2Id))
        {
            b2DestroyJoint(m_Jc1c2Id);
            b2DestroyJoint(m_Jc1p1Id);
            b2DestroyJoint(m_Jc1p2Id);
            b2DestroyJoint(m_Jc2p1Id);
            b2DestroyJoint(m_Jc2p2Id);
            b2DestroyJoint(m_Jp1p2Id);

            m_Jc1c2Id = b2_nullJointId;
            m_Jc1p1Id = b2_nullJointId;
            m_Jc1p2Id = b2_nullJointId;
            m_Jc2p1Id = b2_nullJointId;
            m_Jc2p2Id = b2_nullJointId;
            m_Jp1p2Id = b2_nullJointId;
        }

        m_contatoR1 = m_acum_contatoR1;
        m_contatoR2 = m_acum_contatoR2;

        b2Vec2 x = getCenter();
        m_distancia = x.x - _x0.x;
        if (m_distancia < 0)
            m_distancia = 0;


        if (_t != 0)
        {
            m_vm = m_distancia / _t;
        } else
        {
            m_vm = 0;
        }

        _t += _timeStep;

        _bInStep = false;
        return !m_bDead;
    }

    void CPhysCar::_phys_begin_simulate()
    {
        _x0 = getCenter();
        _t = 0;

        // TODO: Converter.
        // _pWorld->SetContactListener(&_cl);

        m_contatoR1 = 0;
        m_contatoR2 = 0;
        m_acum_contatoR1 = 0;
        m_acum_contatoR2 = 0;

        m_vm = 0;
        m_distancia = 0;

        _last_contact_r1 = 0;
        _last_contact_r2 = 0;
    }

    void CPhysCar::_phys_end_simulate()
    {
        // TODO: Verificar se é necessário após conversão.
        // _pWorld->SetContactListener(NULL);
    }


    void CPhysCar::_simulation_pre_tick()
    {
        float fAngulo;

        const float dx = b2Body_GetPosition(m_Roda2Id).x - b2Body_GetPosition(m_Roda1Id).x;
        const float dy = b2Body_GetPosition(m_Roda2Id).y - b2Body_GetPosition(m_Roda1Id).y;
        if (dx == 0)
        {
            fAngulo = M_PI_2 * (dy > 0) ? (1) : (-1);
        } else
        {
            fAngulo = atan(dy / dx);
        }

        if (fAngulo > M_PI_2)
            fAngulo -= M_PI_2;
        if (fAngulo < -M_PI_2)
            fAngulo += M_PI_2;

        _angle = fAngulo * (180 / M_PI);

        if (!m_bDead)
        {
            if (m_bContactR1)
                // _pRoda1->ApplyTorque((_trqA + _trqB) * _pRoda1->GetMass());
                b2Body_ApplyTorque(m_Roda1Id, (_trqA + _trqB) * b2Body_GetMass(m_Roda1Id), true);

            if (m_bContactR2)
                // _pRoda2->ApplyTorque((_trqC + _trqD) * _pRoda2->GetMass());
                b2Body_ApplyTorque(m_Roda2Id, (_trqC + _trqD) * b2Body_GetMass(m_Roda2Id), true);
        }
    }


    b2Vec2 CPhysCar::getCenter() const
    {
        b2Vec2 pos[5];
        float massa[4];

        pos[0] = b2Body_GetPosition(m_Roda1Id);
        pos[1] = b2Body_GetPosition(m_Roda2Id);
        pos[2] = b2Body_GetPosition(m_Peso1Id);
        pos[3] = b2Body_GetPosition(m_Peso2Id);

        massa[0] = b2Body_GetMass(m_Roda1Id);
        massa[1] = b2Body_GetMass(m_Roda2Id);
        massa[2] = b2Body_GetMass(m_Peso1Id);
        massa[3] = b2Body_GetMass(m_Peso2Id);

        pos[4] = (massa[0] * pos[0] + massa[1] * pos[1] + massa[2] * pos[2] + massa[3] * pos[3]);
        pos[4] *= 1.0f / (massa[0] + massa[1] + massa[2] + massa[3]);

        return pos[4];
    }

    void CPhysCar::_init()
    {
        m_Roda1Id = b2_nullBodyId;
        m_Roda2Id = b2_nullBodyId;
        m_Peso1Id = b2_nullBodyId;
        m_Peso2Id = b2_nullBodyId;

        m_Jc1c2Id = b2_nullJointId;
        m_Jc1p1Id = b2_nullJointId;
        m_Jc1p2Id = b2_nullJointId;
        m_Jc2p1Id = b2_nullJointId;
        m_Jc2p2Id = b2_nullJointId;
        m_Jp1p2Id = b2_nullJointId;

        _timeStep = 1.0f / 50.0f;
        _iterations = 10;
        _bInStep = false;
        _bBroke = false;
        m_bDead = false;
    }

    void CPhysCar::_destroy()
    {
        if (!b2World_IsValid(m_World.m_WorldId))
            return;

        _verificar_step();

        if(!b2Body_IsValid(m_Roda1Id))
            return;

        // Consideraremos que todos os corpos existem se um deles existir
        b2DestroyBody(m_Roda1Id);
        b2DestroyBody(m_Roda2Id);
        b2DestroyBody(m_Peso1Id);
        b2DestroyBody(m_Peso2Id);
        // _pWorld->SetContactListener(NULL);

        // NULLamos tudo:
        _init();
    }

    void CPhysCar::_verificar_step()
    {
#if 0 // TODO: Verificar se é necessário após conversão.
          if (_bInStep)
        {
            // Aguardamos at� 5 segundos antes de prosseguir:
            for (int i = 0; i < 500 || !_bInStep; i++)
            {
                Sleep(10);
            }
        }
#endif
    }
}; //namespace PHYS
