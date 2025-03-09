#include <car_helpers.h>


void TranslateCircle(const b2BodyId RodaId, GUI::CGrCar::gr_circle_t &grCircle)
{
    // b2CircleShape *circle = (b2CircleShape*)pRoda->GetShapeList();
    b2ShapeId shapes[1];
    b2Body_GetShapes(RodaId, shapes, 1);

    const auto posRoda = b2Body_GetPosition(RodaId);
    auto [posCircle, radius] = b2Shape_GetCircle(shapes[0]);
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
