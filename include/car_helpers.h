#include <car.h>

void TranslateCircle(const b2BodyId RodaId, GUI::CGrCar::gr_circle_t &grCircle);
void TranslateRoda(const b2BodyId RodaId, GUI::CGrCar::gr_roda_t &grRoda, const bool bContact);
void TranslatePeso(const b2BodyId PesoId, GUI::CGrCar::gr_peso_t &grPeso, const bool bBroke);
