#pragma once
#include <cstddef>
#include <string>

namespace MODEL
{
    class min_max_values
    {
    public:
        float m_min;
        float m_max;
        min_max_values(const float a_min, const float a_max): m_min(a_min), m_max(a_max){}
        [[nodiscard]] float delta() const {return m_max - m_min;}
    };

    class car_value_limits
    {
    public:
        static min_max_values roda_x_min_max;
        static min_max_values roda_y_min_max;
        static min_max_values roda_r_min_max;
        static min_max_values roda_densidade_min_max;
        static min_max_values roda_friccao_min_max;
        static min_max_values roda_elasticidade_min_max;
        static min_max_values peso_x_min_max;
        static min_max_values peso_y_min_max;
        static min_max_values peso_r_min_max;
        static min_max_values peso_densidade_min_max;
        static min_max_values peso_friccao_min_max;
        static min_max_values peso_elasticidade_min_max;
        static min_max_values torque_min_max;
        static min_max_values freq_min_max;
        static min_max_values damp_min_max;
    };

    class CCarDef
    {
    public:
        struct circle_params_t
        {
            float x;
            float y;
            float raio;

            struct bits
            {
                size_t x{8};
                size_t y{8};
                size_t raio{6};

                [[nodiscard]] size_t operator()() const
                {
                    return x + y + raio;
                }
            };
        };


        struct body_params_t
        {
            float densidade;
            float friccao;
            float elasticidade;

            class bits
            {
            public:
                size_t densidade{6};
                size_t friccao{6};
                size_t elasticidade{6};

                [[nodiscard]] size_t operator()() const
                {
                    return densidade + friccao + elasticidade;
                }
            };
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
                return circle_params_t::bits()() + body_params_t::bits()();
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

        [[nodiscard]] std::string toGenoma() const;

        struct bits
        {
            size_t _roda1{CCarDef::CRodaParams::bits()};
            size_t _roda2{CCarDef::CRodaParams::bits()};
            size_t _peso1{CCarDef::CRodaParams::bits()};
            size_t _peso2{CCarDef::CRodaParams::bits()};
            size_t _torque{8};
            size_t _freq{8};
            size_t _damp{8};

            [[nodiscard]] size_t operator()() const
            {
                return _roda1 + _roda2 + _peso1 + _peso2 + _torque * 4 + _freq * 6 + _damp * 6;
            }
        };
    };

    float decodeBinaryValue(const std::string &genes, size_t &pos, size_t bits, min_max_values &limits);
}
