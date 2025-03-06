#include "car.h"

#include <iostream>


CCar::CCar(CRandomProvider random_provider)
    : CGaCar(random_provider) {}

CCar::CCar(CRandomProvider random_provider, const char *szGenes)
    : CGaCar(random_provider, szGenes) {}

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

void TranslateCircle(const b2BodyId RodaId, GUI::CGrCar::gr_circle_t &grCircle)
{
    // b2CircleShape *circle = (b2CircleShape*)pRoda->GetShapeList();
    b2ShapeId shapes[1];
    b2Body_GetShapes(RodaId, shapes, 1);

    // b2Vec2	pos  = pRoda->GetPosition() + circle->GetLocalPosition();
    const auto posRoda = b2Body_GetPosition(RodaId);
    auto [posCircle, radius] = b2Shape_GetCircle(shapes[0]);
    // cout << "Translating circle: " << posCircle.x << " " << posCircle.y << " " << radius << endl;
    const auto [x, y] = posRoda + posCircle;
    grCircle.center = PointF(x, y);
    grCircle.radius = radius;
}

void TranslateRoda(const b2BodyId RodaId, GUI::CGrCar::gr_roda_t &grRoda, const bool bContact)
{
    TranslateCircle(RodaId, grRoda.circle);

    const auto rotation = b2Body_GetRotation(RodaId);
    grRoda.angle = b2Rot_GetAngle(rotation);
    grRoda.touch = bContact;
}

void TranslatePeso(const b2BodyId PesoId, GUI::CGrCar::gr_peso_t &grPeso, const bool bBroke)
{
    TranslateCircle(PesoId, grPeso.circle);
    grPeso.broke = bBroke;
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
