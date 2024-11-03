//
// Created by marcos on 10/28/24.
//

#include "CNormalSeededRandom.h"

#include <random>

CNormalSeededRandom::CNormalSeededRandom(const unsigned seed)
    : generator(seed) {}

double CNormalSeededRandom::random(const double mean, const double stddev)
{
    std::normal_distribution<double> distribution(mean, stddev);
    return distribution(generator);
}

float CNormalSeededRandom::random(const float mean, const float stddev)
{
    std::normal_distribution<float> distribution(mean, stddev);
    return distribution(generator);
}
