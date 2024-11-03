//
// Created by marcos on 10/28/24.
//

#ifndef CNORMALSEEDEDRANDOM_H
#define CNORMALSEEDEDRANDOM_H
#include <random>


class CNormalSeededRandom
{
private:
    std::mt19937 generator;

public:
    explicit CNormalSeededRandom(unsigned seed);
    double random(double mean, double stddev);
    float random(float mean, float stddev);
};


#endif //CNORMALSEEDEDRANDOM_H
