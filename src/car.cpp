#include "car.h"
#include <car_helpers.h>
#include <iostream>


CCar::CCar()
    : m_ga_car()
    , m_phys_car()
    , m_gr_car() {}

CCar::CCar(const char *szGenes)
    : m_ga_car(szGenes) {}

void CCar::beginSimulate(const b2WorldId WorldId)
{
    // Desinstanciamento
    m_phys_car.destroy();

    // Decodificamos os genes (genes -> carro | string -> CCarDef)
    m_ga_car.decode();

    // Instanciamento
    m_phys_car.create(WorldId, m_ga_car._carro);

    // Inicializamos a simulação física:
    m_phys_car.init_simulation_vars();
}

void CCar::endSimulate()
{
    // Liberamos os recursos da phys:
    m_phys_car.phys_end_simulate();
}

void CCar::CreateCar(const char *szGenes)
{
    m_phys_car.destroy();

    m_phys_car.init();

    m_ga_car.CreateCar(szGenes);
}

void CCar::DestroyCar()
{
    m_phys_car.destroy();
}

bool CCar::doStep()
{
    const bool bRet = m_phys_car.simulation_step();

    UpdateGraphicsData();

    return bRet;
}

void CCar::UpdateGraphicsData()
{
    TranslateRoda(m_Roda1Id, _roda1, m_bContactR1);
    TranslateRoda(m_Roda2Id, _roda2, m_bContactR2);
    TranslatePeso(m_Peso1Id, _peso1, m_bDead);
    TranslatePeso(m_Peso2Id, _peso2, m_bDead);
    const auto [x, y] = getCenter();
    _cm = PointF(x, y);
    _broke = m_bDead;
}

void CCar::Medir(const b2WorldId WorldId, const double max_t)
{
    beginSimulate(WorldId);
    const float x0 = getCenter().x;
    for (int k = 0; _t < max_t; k++)
    {
        if (!doStep())
            break;
    }
    const float x = getCenter().x;
    m_t = _t;
    Destroy();

    m_distancia = x - x0;
}
