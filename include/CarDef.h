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

            explicit circle_params_t(const std::string& genes, size_t &pos);
            circle_params_t(float x, float y, float r);
            [[nodiscard]] std::string genes() const;

            // Bit sizes for each parameter
            struct bits
            {
                static size_t x;
                static size_t y;
                static size_t raio;

                [[nodiscard]] static size_t len()
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

            body_params_t(const std::string& genes, size_t &pos);
            body_params_t(float dens, float fric, float elas);
            [[nodiscard]] std::string genes() const;

            class bits
            {
            public:
                static size_t densidade;
                static size_t friccao;
                static size_t elasticidade;

                [[nodiscard]] static size_t len()
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
            [[nodiscard]] std::string genes() const;

            struct bits
            {
                [[nodiscard]] static size_t len()
                {
                    return circle_params_t::bits::len() + body_params_t::bits::len();
                }
            };
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
        [[nodiscard]] std::string genes() const;

        ~CCarDef() = default;

        bool operator==(const CCarDef &other) const;

        struct bits
        {
            static size_t _roda1;
            static size_t _roda2;
            static size_t _peso1;
            static size_t _peso2;
            static size_t _torque;
            static size_t _freq;
            static size_t _damp;

            [[nodiscard]] static size_t len()
            {
                return _roda1 + _roda2 + _peso1 + _peso2 + _torque * 4 + _freq * 6 + _damp * 6;
            }
        };
    };

    float decodeBinaryValue(const std::string &genes, size_t &pos, size_t bits, min_max_values &limits);
}
