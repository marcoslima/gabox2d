#pragma once
#include <cstddef>
#include <string>

namespace MODEL
{
    class CCarDef
    {
    public:
        using circle_params_t = struct tagCircleParams
        {
            float x;
            float y;
            float raio;
        };

        class circle_params_bits
        {
        public:
            size_t x{8};
            size_t y{8};
            size_t raio{6};

            [[nodiscard]] size_t bits() const
            {
                return x + y + raio;
            }
        };

        using body_params_t = struct tagBodyParams
        {
            float densidade;
            float friccao;
            float elasticidade;
        };

        class body_params_bits
        {
        public:
            size_t densidade{6};
            size_t friccao{6};
            size_t elasticidade{6};

            [[nodiscard]] size_t bits() const
            {
                return densidade + friccao + elasticidade;
            }
        };

        class CRodaParams
        {
        public:
            circle_params_t circle;
            body_params_t body;

            CRodaParams();

            CRodaParams(float x, float y, float r, float dens, float fric, float elas);
            CRodaParams(const std::string &genes, size_t &pos);

            [[nodiscard]] static size_t bits()
            {
                return circle_params_bits().bits() + body_params_bits().bits();
            }
        };

        CRodaParams _roda1;
        CRodaParams _roda2;
        CRodaParams _peso1;
        CRodaParams _peso2;
        float _torque[4];
        float _freq[6];
        float _damp[6];

        CCarDef();

        explicit CCarDef(const std::string& genes);

        ~CCarDef() = default;

        bool operator==(const CCarDef &other) const;
    };

    class CCarDefBits
    {
    public:
        size_t _roda1{CCarDef::CRodaParams::bits()};
        size_t _roda2{CCarDef::CRodaParams::bits()};
        size_t _peso1{CCarDef::CRodaParams::bits()};
        size_t _peso2{CCarDef::CRodaParams::bits()};
        size_t _torque{8};
        size_t _freq{8};
        size_t _damp{8};

        [[nodiscard]] size_t bits() const
        {
            return _roda1 + _roda2 + _peso1 + _peso2 + _torque * 4 + _freq * 6 + _damp * 6;
        }
    };
    float decodeBinaryValue(const std::string &genes, size_t &pos, size_t bits, float min, float max);
}
