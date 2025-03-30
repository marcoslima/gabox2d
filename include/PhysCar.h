#pragma once
#include <box2d/box2d.h>

#include "CarDef.h"
#include "env.h"
#include "World.h"
#include <IPhysCar.h>
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

    class CPhysCar final : public IPhysCar
    {
        // Instanciamento do carro no box2d
        b2BodyId m_Roda1Id{};
        b2BodyId m_Roda2Id{};
        b2BodyId m_Peso1Id{};
        b2BodyId m_Peso2Id{};

        b2JointId m_Jc1c2Id{};
        b2JointId m_Jc1p1Id{};
        b2JointId m_Jc1p2Id{};
        b2JointId m_Jc2p1Id{};
        b2JointId m_Jc2p2Id{};
        b2JointId m_Jp1p2Id{};

        IWorldPtr m_World;
        b2WorldId m_WorldId{};
        string m_dead_reason;

        bool _bInStep{};
        bool m_bDead{};
        float m_distancia{};
        float m_contatoR1{};
        float m_contatoR2{};
        float m_acum_contatoR1{};
        float m_acum_contatoR2{};
        float m_vm{};

        // semi-constantes
        float _timeStep{};
        int32_t _iterations{};

        // Temporárias durante simulação:
        b2Vec2 _x0{};
        float _t{};
        bool m_bContactR1{};
        bool m_bContactR2{};
        float _last_contact_r1{};
        float _last_contact_r2{};
        float _no_contact_time_r1{};
        float _no_contact_time_r2{};
        b2Vec2 _cVel{};
        b2Vec2 _cPos{};

        // Dados efêmeros. Só existem durante a medição/simulação do carro.
        float _trqA{};
        float _trqB{};
        float _trqC{};
        float _trqD{};


    // Public Methods
    public:
        CPhysCar();
        ~CPhysCar() override;

        // CPhysCar &operator=(const CPhysCar &other);

        // Usa as definições decodificadas para criar o objeto em si no box2d
        void create(IWorldPtr world, const CCarDef& carro) override;

        void destroy() override;
        void reset() override;

        void init_simulation_vars() override;
        bool simulation_step_get_dead() override;
        void simulation_step() override;
        void measure(IWorldPtr world, const CCarDef &carro, float max_t) override;
        void init() override;

    private:
        void _simulation_pre_tick() const;
        void _register_contact_times();
        void _process_no_contact_time();
        void _test_peso(b2BodyId pesoId, const string &name);
        void _test_contacts();
        void _process_contacts();
        void _remove_joints_if_dead();
        void _register_distance_travelled();
        void _calc_average_velocity();
        void _register_time_step();
        void _register_contacts();
        void _simulation_pos_tick();
        void _create_rodas_e_pesos(const CCarDef &carro, const car_t &car_def);
        void _set_torques(const car_t &car_def);
        [[nodiscard]] b2JointId _create_joint(b2BodyId bodyA, b2BodyId bodyB, const car_t &car_def, int param_index) const;
        void _create_joints(const car_t &car_def);

    public:
        [[nodiscard]] vec2f_t getMassCenter() const override;
        [[nodiscard]] float getCurrentX() const override;
        [[nodiscard]] float getT() const override;
        [[nodiscard]] bool isDead() const override;
        [[nodiscard]] string deadReason() const override;
        [[nodiscard]] phys_car_ptr_t clone() const override;

        void fill_gr_car(GUI::IGrCar &car) override;
        GA::fitness_params_t get_ga_fitness_params() override;
    };
}
