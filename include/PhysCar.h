#pragma once
#include <box2d/box2d.h>
#include <uuid/uuid.h>

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

    class CPhysCar
    {
    // public data
    public:
        // ID deste carro:
        uuid_t m_id;

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
        string m_dead_reason;

    // Public Methods
    public:
        CPhysCar();
        ~CPhysCar();

        bool operator==(const CPhysCar &) const;

        // Usa as definições decodificadas para criar o objeto em si no box2d
        virtual void create(const b2WorldId WorldId, const CCarDef& carro);

        virtual void destroy();
        void reset();

        virtual void init_simulation_vars();
        virtual void phys_end_simulate();
        bool simulation_step();

        void init();

        // semi-constantes
    public:
        float _timeStep;
        int32_t _iterations;

        // Temporárias durante simulação:
        b2Vec2 _x0;
        float _t;
        bool m_bContactR1;
        bool m_bContactR2;
        float _last_contact_r1;
        float _last_contact_r2;
        float _no_contact_time_r1;
        float _no_contact_time_r2;
        b2Vec2 _cVel;
        b2Vec2 _cPos;

        // Dados efêmeros. Só existem durante a medição/simulação do carro.
    public:
        float _trqA;
        float _trqB;
        float _trqC;
        float _trqD;

    private:
        void _simulation_pre_tick() const;

        void _register_contact_times();

        void _process_no_contact_time();

        void _process_touch_on_body(b2BodyId bodyId, bool bContact, float touch_strength);

        void _process_contact_data(const b2ContactData &contactData, b2BodyId bodyId);

        void _process_contacts(const b2ContactEvents &contacts);

        void _test_peso(b2BodyId pesoId, const string &name);

        void _test_contacts();

        void _process_contacts();

        void _remove_joints_if_dead();

        void _register_distance_travelled();

        void _calc_average_velocity();

        void _register_time_step();

        void _register_contacts();

        void _simulation_pos_tick();

        // CContactListener _cl;
        bool _bInStep;

        static void _verificar_step();

    protected:
        void _create_rodas_e_pesos(const CCarDef &carro, const car_t &car_def);

        void _set_torques(const car_t &car_def);

        [[nodiscard]] b2JointId _create_joint(b2BodyId bodyA, b2BodyId bodyB, const car_t &car_def, int param_index) const;

        void _create_joints(const car_t &car_def);


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

        [[nodiscard]] float getT() const
        {
            return _t;
        }

        void Destroy()
        {
            destroy();
        }

        bool m_bDead;
        float m_distancia;
        float m_contatoR1;
        float m_contatoR2;
        float m_acum_contatoR1;
        float m_acum_contatoR2;
        float m_vm;
        float m_t;
    };
}
