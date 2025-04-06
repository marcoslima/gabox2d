#pragma once
#include <string>


class pair_float_string_t : public std::pair<float, std::string>
{
public:
    pair_float_string_t() = default;
    pair_float_string_t(float first, const std::string& second) : pair(first, second) {}

    template<class T>
    void pack(T& pack)
    {
        pack(first);
        pack(second);
    }

};

class pair_size_string_t : public std::pair<size_t, std::string>
{
public:
    pair_size_string_t() = default;
    pair_size_string_t(size_t first, const std::string& second) : pair(first, second) {}

    template<class T>
    void pack(T& pack)
    {
        pack(first);
        pack(second);
    }
};
