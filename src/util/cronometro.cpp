#include "util/cronometro.h"


void CCronometro::Start()
{
    m_start = std::chrono::high_resolution_clock::now();
}

double CCronometro::Get() const
{
    const auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(now - m_start).count();
}

double CCronometro::End() const
{
    const auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(now - m_start).count();
}
