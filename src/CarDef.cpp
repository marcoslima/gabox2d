#include "CarDef.h"

#include <bitset>

namespace MODEL
{
    min_max_values car_value_limits::roda_x_min_max{-8.0f, 8.0f};
    min_max_values car_value_limits::roda_y_min_max{2.0f, 8.0f};
    min_max_values car_value_limits::roda_r_min_max{0.2f, 3.0f};
    min_max_values car_value_limits::roda_densidade_min_max{0.1f, 10.0f};
    min_max_values car_value_limits::roda_friccao_min_max{0.1f, 5.0f};
    min_max_values car_value_limits::roda_elasticidade_min_max{0.0f, 1.0f};
    min_max_values car_value_limits::torque_min_max{-50.0f, 50.0f};
    min_max_values car_value_limits::freq_min_max{0.1f, 30.0f};
    min_max_values car_value_limits::damp_min_max{0.0f, 2.0f};

    size_t CCarDef::bits::_roda1{CRodaParams::bits::len()};
    size_t CCarDef::bits::_roda2{CRodaParams::bits::len()};
    size_t CCarDef::bits::_peso1{CRodaParams::bits::len()};
    size_t CCarDef::bits::_peso2{CRodaParams::bits::len()};
    size_t CCarDef::bits::_torque{8};
    size_t CCarDef::bits::_freq{8};
    size_t CCarDef::bits::_damp{8};

    size_t CCarDef::circle_params_t::bits::x{8};
    size_t CCarDef::circle_params_t::bits::y{8};
    size_t CCarDef::circle_params_t::bits::raio{6};

    size_t CCarDef::body_params_t::bits::densidade{6};
    size_t CCarDef::body_params_t::bits::friccao{6};
    size_t CCarDef::body_params_t::bits::elasticidade{6};

    CCarDef::CCarDef(): _torque{}, _freq{}, _damp{} {}

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

    float decodeBinaryValue(const std::string &genes, size_t &pos, const size_t bits, min_max_values &limits)
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
        return limits.m_min + limits.delta() * static_cast<float>(value) / static_cast<float>(maxValue);
    }

    void encodeBinaryValue(std::string& genes, float value, const size_t bit_len, min_max_values &limits)
    {
        const unsigned int maxValue = (1 << bit_len) - 1;
        const auto mappedValue = static_cast<unsigned int>((value - limits.m_min) / limits.delta() * static_cast<float>(maxValue));
        for (int i = static_cast<int>(bit_len) - 1; i >= 0; i--)
        {
            genes.push_back((mappedValue & (1 << i)) ? '1' : '0');
        }
    }

    CCarDef::circle_params_t::circle_params_t(const std::string &genes, size_t& pos)
        : x{decodeBinaryValue(genes, pos, bits::x, car_value_limits::roda_x_min_max)}
        , y{decodeBinaryValue(genes, pos, bits::y, car_value_limits::roda_y_min_max)}
        , raio{decodeBinaryValue(genes, pos, bits::raio, car_value_limits::roda_r_min_max)} {}

    CCarDef::circle_params_t::circle_params_t(const float x, const float y, const float r) : x{x}, y{y}, raio{r} {}
    std::string CCarDef::circle_params_t::genes() const
    {
        std::string genes;
        genes.reserve(bits::len());
        encodeBinaryValue(genes, x, bits::x, car_value_limits::roda_x_min_max);
        encodeBinaryValue(genes, y, bits::y, car_value_limits::roda_y_min_max);
        encodeBinaryValue(genes, raio, bits::raio, car_value_limits::roda_r_min_max);
        return genes;
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

        for (int i = 0; i < 6; i++)
        {
            // Only decode torque for the first 4 indices
            if (i <= nMaxTorqueIndex)
            {
                _torque[i] = decodeBinaryValue(genes, pos, bits::_torque, car_value_limits::torque_min_max);
            }

            _freq[i] = decodeBinaryValue(genes, pos, bits::_freq, car_value_limits::freq_min_max);
            _damp[i] = decodeBinaryValue(genes, pos, bits::_damp, car_value_limits::damp_min_max);
        }
    }

    std::string CCarDef::genes() const
    {
        const std::string part1 = _roda1.genes() + _roda2.genes() + _peso1.genes() + _peso2.genes();
        std::string part2;
        for (int i = 0; i < 6; i++)
        {
            if (i < 4)
            {
                encodeBinaryValue(part2, _torque[i], bits::_torque, car_value_limits::torque_min_max);
            }
            encodeBinaryValue(part2, _freq[i], bits::_freq, car_value_limits::freq_min_max);
            encodeBinaryValue(part2, _damp[i], bits::_damp, car_value_limits::damp_min_max);
        }
        return part1 + part2;
    }

    CCarDef::CRodaParams::CRodaParams(const std::string &genes, size_t &pos)
        : circle{genes, pos}
        , body{genes, pos} {}

    std::string CCarDef::CRodaParams::genes() const
    {
        return circle.genes() + body.genes();
    }

    CCarDef::body_params_t::body_params_t(const std::string &genes, size_t &pos)
        : densidade{decodeBinaryValue(genes, pos, bits::densidade, car_value_limits::roda_densidade_min_max)},
          friccao{decodeBinaryValue(genes, pos, bits::friccao, car_value_limits::roda_friccao_min_max)},
          elasticidade{decodeBinaryValue(genes, pos, bits::elasticidade, car_value_limits::roda_elasticidade_min_max)} {}

    CCarDef::body_params_t::body_params_t(const float dens, const float fric, const float elas)
        : densidade{dens}, friccao{fric}, elasticidade{elas} {}

    std::string CCarDef::body_params_t::genes() const
    {
        std::string genes;
        genes.reserve(bits::len());
        encodeBinaryValue(genes, densidade, bits::densidade, car_value_limits::roda_densidade_min_max);
        encodeBinaryValue(genes, friccao, bits::friccao, car_value_limits::roda_friccao_min_max);
        encodeBinaryValue(genes, elasticidade, bits::elasticidade, car_value_limits::roda_elasticidade_min_max);
        return genes;
    }

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
}
