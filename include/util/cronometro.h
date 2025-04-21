#pragma once

#include <chrono>


class CCronometro
{
    std::chrono::time_point<std::chrono::system_clock> m_start = std::chrono::high_resolution_clock::now();

public:
    void Start();
    [[nodiscard]] double Get() const;
    [[nodiscard]] double End() const;
};
