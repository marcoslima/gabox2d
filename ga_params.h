//
// Created by marcos on 12/15/24.
//

#ifndef GA_PARAMS_H
#define GA_PARAMS_H
#include <cstdint>

class ga_params_t
{
public:
    uint8_t m_nPopulacao;
    float m_fCrossover;
    uint8_t m_nElitismo;
    float m_fMutacao;
    uint8_t m_nAlienismo;
    uint8_t m_nMutInt;
    float m_fMaxT;

    ga_params_t(
        const uint8_t nPopulacao,
        const float fCrossover,
        const uint8_t nElitismo,
        const float fMutacao,
        const uint8_t nAlienismo,
        const uint8_t nMutInt,
        const float fMaxT)
    {
        m_nPopulacao = nPopulacao;
        m_fCrossover = fCrossover;
        m_nElitismo = nElitismo;
        m_fMutacao = fMutacao;
        m_nAlienismo = nAlienismo;
        m_nMutInt = nMutInt;
        m_fMaxT = fMaxT;
    }

    ga_params_t()
        : m_nPopulacao(90)
          , m_fCrossover(0.7)
          , m_nElitismo(2)
          , m_fMutacao(0.65)
          , m_nAlienismo(0)
          , m_nMutInt(10),
          m_fMaxT(60) {}
};

#endif //GA_PARAMS_H
