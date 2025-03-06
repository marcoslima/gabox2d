#pragma once

#include <random>


class CRandomProvider
{
    std::mt19937 m_gen;

public:
    CRandomProvider(std::random_device::result_type seed = std::random_device{}())
        : m_gen(seed)
    {
    }

    template<typename T>
    T rand(const T min, const T max)
    {
        std::uniform_int_distribution dis(min, max);
        return dis(m_gen);
    }

    int randInt(const int max)
    {
        return rand<int>(0, max);
    }

    char randChar(const char min, const char max)
    {
        return rand<char>(min, max);
    }
};