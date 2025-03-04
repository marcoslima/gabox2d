#pragma once
#include <random>


class CRandom
{
private:
    mutable std::mt19937 generator;

public:
    explicit CRandom(unsigned seed): generator(seed) {}
    
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
    T normal_random(const T mean, const T stddev)
    {
        std::normal_distribution<T> distribution(mean, stddev);
        return distribution(generator);
    }


    template <typename T>
    T random(const T aMin, const T aMax) const
    {
        return real_random(aMin, aMax);
    }

    template <typename T>
    T rand_int(const T aMin, const T aMax) const
    {
        return discrete_random(aMin, aMax);
    }

    template <typename T>
    T rand_norm(const T mean, const T stddev)
    {
        return normal_random(mean, stddev);
    }
};

// Explicit instantiation for double, float, char and size_t:
template double CRandom::real_random(double aMin, double aMax) const;
template float CRandom::real_random(float aMin, float aMax) const;
template char CRandom::discrete_random(char aMin, char aMax) const;
template size_t CRandom::discrete_random(size_t aMin, size_t aMax) const;

// Explicit instantiation of Normal distribution for double and float:
template double CRandom::normal_random(double mean, double stddev);
template float CRandom::normal_random(float mean, float stddev);
