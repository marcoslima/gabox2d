#pragma once
#include <box2d/box2d.h>
#include "CarDef.h"
#include "env.h"
#include "World.h"
using namespace MODEL;

namespace PHYS
{
    using b2_roda_ou_peso_def = struct tagB2RodaPesoDef
    {
        b2Circle b2_circle{};
        b2BodyDef b2_body_def{b2DefaultBodyDef()};
    };

    using car_t = struct tagCarParams
    {
        // Roda 1
        b2_roda_ou_peso_def R1;

        // Roda 2
        b2_roda_ou_peso_def R2;

        // Peso 1
        b2_roda_ou_peso_def P1;

        // Peso 2
        b2_roda_ou_peso_def P2;

        float torque[4]{};

        // Para que os distance joints façam parte do genoma,
        // teremos a frequencia de resposta e o damp deles codificados
        // também.
        float freq[6]{};
        float damp[6]{};
    };

    class CPhysCar;

    // class CContactListener
    // {
    // public:
    //     void Add(const b2Contact *point);
    //
    //     void Persist(const b2Contact *point);
    //
    // public:
    //     bool m_bDead;
    //     bool m_bContactR1;
    //     bool m_bContactR2;
    //     b2Vec2 m_cVel;
    //     b2Vec2 m_cPos;
    // };

    class CPhysCar
    {
        // friend class CContactListener;

    public:
        CPhysCar();

        ~CPhysCar();

        // Instanciamento do carro no box2d
        b2BodyId m_Roda1Id;
        b2BodyId m_Roda2Id;
        b2BodyId m_Peso1Id;
        b2BodyId m_Peso2Id;

        b2JointId m_Jc1c2Id;
        b2JointId m_Jc1p1Id;
        b2JointId m_Jc1p2Id;
        b2JointId m_Jc2p1Id;
        b2JointId m_Jc2p2Id;
        b2JointId m_Jp1p2Id;

        CWorld m_World;

        bool _bBroke;

        // semi-constantes
    protected:
        float _timeStep;
        int32_t _iterations;

        // Temporárias durante simulação:
        b2Vec2 _x0;
        double _t;
        bool m_bContactR1;
        bool m_bContactR2;
        double _last_contact_r1;
        double _last_contact_r2;
        double _no_contact_time_r1;
        double _no_contact_time_r2;
        b2Vec2 _cVel;
        b2Vec2 _cPos;

        // Dados efêmeros. Só existem durante a medição/simulação do carro.
    public:
        float _trqA;
        float _trqB;
        float _trqC;
        float _trqD;

        double _angle;

    private:
        void _simulation_pre_tick();

        // CContactListener _cl;
        bool _bInStep;

        static void _verificar_step();

    protected:
        void _init();

        void _create_rodas_e_pesos(const CCarDef &carro, const car_t &car_def);

        void _set_torques(const car_t &car_def);

        [[nodiscard]] b2JointId _create_joint(b2BodyId bodyA, b2BodyId bodyB, const car_t &car_def, int param_index) const;

        void _create_joints(const car_t &car_def);

        // Usa as definições decodificadas para criar o objeto em si no box2d
        void _create(b2WorldId WorldId, const CCarDef& carro);

        void _destroy();

        void _init_simulation_vars();

        static void _phys_end_simulate();

        bool _simulation_step();


        // Suporte à simulação no Box2d
    public:
        [[nodiscard]] b2Vec2 getCenter() const;

        // Queries
        [[nodiscard]] b2BodyId getR1() const
        {
            return m_Roda1Id;
        }

        [[nodiscard]] b2BodyId getR2() const
        {
            return m_Roda2Id;
        }

        [[nodiscard]] b2BodyId getP1() const
        {
            return m_Peso1Id;
        }

        [[nodiscard]] b2BodyId getP2() const
        {
            return m_Peso2Id;
        }

        [[nodiscard]] double getT() const
        {
            return _t;
        }

        void Destroy()
        {
            _destroy();
        }

        bool m_bDead;
        double m_distancia;
        double m_contatoR1;
        double m_contatoR2;
        double m_acum_contatoR1;
        double m_acum_contatoR2;
        double m_vm;
        double m_t;
    };
}
