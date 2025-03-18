#pragma once

#include <car.h>

GUI::CGrCar::gr_circle_t TranslateCircle(const b2BodyId RodaId);
GUI::CGrCar::gr_roda_t TranslateRoda(const b2BodyId RodaId, const bool bContact);
GUI::CGrCar::gr_peso_t TranslatePeso(const b2BodyId PesoId, const bool bBroke);
