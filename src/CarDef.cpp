#include "CarDef.h"

namespace MODEL
{
    CCarDef::CCarDef(): _torque{}, _freq{}, _damp{} {}

    CCarDef::~CCarDef() = default;

    bool operator==(const CCarDef::circle_params_t &left, const CCarDef::circle_params_t &right);

    bool operator==(const CCarDef::body_params_t &left, const CCarDef::body_params_t &right);

    bool operator==(const CCarDef::CRodaParams &left, const CCarDef::CRodaParams &right);

    bool CCarDef::operator==(const CCarDef &other) const
    {
        if (_roda1 != other._roda1
            || _roda2 != other._roda2
            || _peso1 != other._peso1
            || _peso2 != other._peso2)
            return false;

        for (int i = 0; i < 4; i++)
            if (_torque[i] != other._torque[i]) return false;


        for (int i = 0; i < 6; i++)
        {
            if (_freq[i] != other._freq[i]) return false;
            if (_damp[i] != other._damp[i]) return false;
        }
        return true;
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

    CCarDef::CCarDef(const std::string &genes)
        : _torque{}
          , _freq{}
          , _damp{}
    {
        size_t pos = 0;

        // Decode wheels and weights
        _roda1 = CRodaParams(genes, pos);
        _roda2 = CRodaParams(genes, pos);
        _peso1 = CRodaParams(genes, pos);
        _peso2 = CRodaParams(genes, pos);

        // Decode joint parameters (torque, frequency, damping)
        constexpr int nMaxTorqueIndex = 3;
        constexpr float dMaxDamp = 2.0f;
        constexpr float dMinDamp = 0.0f;
        constexpr float dMaxFreq = 30.0f;
        constexpr float dMinFreq = 0.1f;
        constexpr float dFp = 50.0f;
        const CCarDefBits bits;
        for (int i = 0; i < 6; i++)
        {
            // Only decode torque for the first 4 indices
            if (i <= nMaxTorqueIndex)
            {
                _torque[i] = decodeBinaryValue(genes, pos, bits._torque, -dFp, dFp);
            }

            _freq[i] = decodeBinaryValue(genes, pos, bits._freq, dMinFreq, dMaxFreq);
            _damp[i] = decodeBinaryValue(genes, pos, bits._damp, dMinDamp, dMaxDamp);
        }
    }

    float decodeBinaryValue(const std::string &genes, size_t &pos, const size_t bits, const float min, const float max)
    {
        // Extract the binary substring
        unsigned int value = 0;
        const auto genes_size = genes.size();
        for (size_t i = 0; i < bits && pos < genes_size; i++, pos++)
        {
            value = value << 1 | (genes[pos] == '1' ? 1 : 0);
        }

        // Map binary value to float range
        const unsigned int maxValue = (1 << bits) - 1;
        return min + (max - min) * static_cast<float>(value) / static_cast<float>(maxValue);
    }

    CCarDef::CRodaParams::CRodaParams(const std::string &genes, size_t &pos)
        : circle{
              decodeBinaryValue(genes, pos, circle_params_bits().x, -8.0, 8.0), // x
              decodeBinaryValue(genes, pos, circle_params_bits().y, 2.0, 8.0), // y
              decodeBinaryValue(genes, pos, circle_params_bits().raio, 0.2, 3.0), // radius
          },
          body{
              decodeBinaryValue(genes, pos, body_params_bits().densidade, 0.1, 10.0), // densidade
              decodeBinaryValue(genes, pos, body_params_bits().friccao, 0.1, 5.0), // friccao
              decodeBinaryValue(genes, pos, body_params_bits().elasticidade, 0.0, 1.0) // elasticidade
          } {}


    bool operator==(const CCarDef::circle_params_t &left, const CCarDef::circle_params_t &right)
    {
        return left.x == right.x
               && left.y == right.y
               && left.raio == right.raio;
    }

    bool operator==(const CCarDef::body_params_t &left, const CCarDef::body_params_t &right)
    {
        return left.densidade == right.densidade
               && left.friccao == right.friccao
               && left.elasticidade == right.elasticidade;
    }

    bool operator==(const CCarDef::CRodaParams &left, const CCarDef::CRodaParams &right)
    {
        return left.circle == right.circle
               && left.body == right.body;
    }
};
