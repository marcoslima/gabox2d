#pragma once
#include <random>


class CRandom
{
    mutable std::mt19937 generator;

public:
    explicit CRandom(const unsigned seed): generator(seed) {}
    CRandom(): generator(static_cast<unsigned>(time(nullptr))) {} // NOLINT(*-msc51-cpp)
    
    void set_seed(const unsigned seed) const
    {
        generator.seed(seed);
    }

    template <typename T>
    T real_random(T aMin, T aMax) const
    {
        std::uniform_real_distribution<T> distribution(aMin, aMax);
        return distribution(generator);
    }

    template <typename T>
    T discrete_random(T aMin, T aMax) const
    {
        std::uniform_int_distribution<T> distribution(aMin, aMax);
        return distribution(generator);
    }

    template<typename T>
    T normal_random(const T mean, const T stddev) const
    {
        std::normal_distribution<T> distribution(mean, stddev);
        return distribution(generator);
    }

    template <typename T>
    T rand_int(const T aMin, const T aMax) const
    {
        return discrete_random(aMin, aMax);
    }
    std::mt19937& get_engine()
    {
        return generator;
    }
};

// Explicit instantiation for double, float, char and size_t:
// template float CRandom::real_random(float aMin, float aMax) const;
// template double CRandom::real_random(double aMin, double aMax) const;
// template char CRandom::discrete_random(char aMin, char aMax) const;
// template size_t CRandom::discrete_random(size_t aMin, size_t aMax) const;
// template int CRandom::discrete_random(int aMin, int aMax) const;

// Explicit instantiation of Normal distribution for double and float:
template double CRandom::normal_random(double mean, double stddev) const;
template float CRandom::normal_random(float mean, float stddev) const;
