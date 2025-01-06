//
// Created by marcos on 12/19/24.
//

#ifndef CCRONOMETRO_H
#define CCRONOMETRO_H
#include <chrono>


class CCronometro
{
    std::chrono::time_point<std::chrono::system_clock> m_start = std::chrono::high_resolution_clock::now();

public:
    void Start();
    double Get() const;
    double End() const;
};



#endif //CCRONOMETRO_H
