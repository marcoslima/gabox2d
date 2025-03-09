#include "car.h"
#include <car_helpers.h>
#include <iostream>


CCar::CCar()
    : CGaCar() {}

CCar::CCar(const char *szGenes)
    : CGaCar(szGenes) {}

void CCar::beginSimulate(const b2WorldId WorldId)
{
    // Desinstanciamento
    _destroy();

    // Decodificamos os genes (genes -> carro | string -> CCarDef)
    _decode();

    // Instanciamento
    _create(WorldId, _carro);

    // Inicializamos a simulação física:
    _init_simulation_vars();
}

void CCar::endSimulate()
{
    // Liberamos os recursos da phys:
    _phys_end_simulate();
}

void CCar::CreateCar(const char *szGenes)
{
    if (b2World_IsValid(m_World.m_WorldId) && b2Body_IsValid(m_Roda1Id))
        _destroy();

    _init();
    CGaCar::CreateCar(szGenes);
}

void CCar::DestroyCar()
{
    _destroy();
}

bool CCar::doStep()
{
    const bool bRet = _simulation_step();

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
