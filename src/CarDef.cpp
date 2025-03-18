#include "CarDef.h"

namespace MODEL
{
    CCarDef::CCarDef(): _torque{}, _freq{}, _damp{} {}

    CCarDef::~CCarDef(void) = default;

    bool operator==(const CCarDef::CCircleParams& left, const CCarDef::CCircleParams& right);
    bool operator==(const CCarDef::CBodyParams& left, const CCarDef::CBodyParams& right);
    bool operator==(const CCarDef::CRodaParams& left, const CCarDef::CRodaParams& right);

    bool CCarDef::operator==(const CCarDef &other) const
    {
        bool result = _roda1 == other._roda1
                      && _roda2 == other._roda2
                      && _peso1 == other._peso1
                      && _peso2 == other._peso2;
        for (int i = 0; i < 4; i++)
        {
            result = result && _torque[i] == other._torque[i];
        }
        for (int i = 0; i < 6; i++)
        {
            result = result && _freq[i] == other._freq[i];
            result = result && _damp[i] == other._damp[i];
        }
        return result;
    }

    CCarDef::CRodaParams::CRodaParams() : CRodaParams(0, 0, 1, 1, 1, 1) {}

    CCarDef::CRodaParams::CRodaParams(const float x,
                                      const float y,
                                      const float r,
                                      const float dens,
                                      const float fric,
                                      const float elas)
        : circle{x, y, r}
        , body{dens, fric, elas} {}

    bool operator==(const CCarDef::CCircleParams& left, const CCarDef::CCircleParams& right)
    {
        return left.x == right.x
                && left.y == right.y
                && left.raio == right.raio;
    }

    bool operator==(const CCarDef::CBodyParams& left, const CCarDef::CBodyParams& right)
    {
        return left.densidade == right.densidade
                && left.friccao == right.friccao
                && left.elasticidade == right.elasticidade;
    }

    bool operator==(const CCarDef::CRodaParams& left, const CCarDef::CRodaParams& right)
    {
        return left.circle == right.circle
                && left.body == right.body;
    }
};
