#include <car_helpers.h>


GUI::CGrCar::gr_circle_t TranslateCircle(const b2BodyId RodaId)
{
    GUI::CGrCar::gr_circle_t grCircle;
    b2ShapeId shapes[1];
    b2Body_GetShapes(RodaId, shapes, 1);

    const auto posRoda = b2Body_GetPosition(RodaId);
    auto [posCircle, radius] = b2Shape_GetCircle(shapes[0]);
    const auto [x, y] = posRoda + posCircle;
    grCircle.center = sf::Vector2f(x, y);
    grCircle.radius = radius;

    return grCircle;
}

GUI::CGrCar::gr_roda_t TranslateRoda(const b2BodyId RodaId, const bool bContact)
{
    GUI::CGrCar::gr_roda_t grRoda;
    grRoda.circle = TranslateCircle(RodaId);

    const auto rotation = b2Body_GetRotation(RodaId);
    grRoda.angle = b2Rot_GetAngle(rotation);
    grRoda.touch = bContact;

    return grRoda;
}

GUI::CGrCar::gr_peso_t TranslatePeso(const b2BodyId PesoId, const bool bBroke)
{
    GUI::CGrCar::gr_peso_t grPeso;
    grPeso.circle = TranslateCircle(PesoId);
    grPeso.broke = bBroke;

    return grPeso;
}
