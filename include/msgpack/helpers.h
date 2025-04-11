#pragma once
#include <string>
#include <msgpack11.hpp>

using namespace msgpack11;

class pair_float_string_t : public std::pair<float, std::string>
{
public:
    pair_float_string_t() = default;
    pair_float_string_t(float first, const std::string& second) : pair(first, second) {}
    [[nodiscard]] MsgPack::object to_object() const
    {
        return {{"first", first}, {"second", second}};
    }
    static pair_float_string_t from_object(const MsgPack& obj)
    {
        auto items = obj.object_items();
        return {items["first"].float32_value(), items["second"].string_value()};
    }
};

class pair_size_string_t : public std::pair<size_t, std::string>
{
public:
    pair_size_string_t() = default;
    pair_size_string_t(size_t first, const std::string& second) : pair(first, second) {}
    [[nodiscard]] MsgPack::object to_object() const
    {
        return {{"first", first}, {"second", second}};
    }

    static pair_size_string_t from_object(const MsgPack& obj)
    {
        auto items = obj.object_items();
        return {items["first"].uint32_value(), items["second"].string_value()};
    }
};

template<class T>
MsgPack::array to_object(const std::vector<T>& vec)
{
    MsgPack::array result;
    for (const auto& item : vec)
    {
        result.push_back(item.to_object());
    }
    return result;
}

template<class T>
std::vector<T> from_object(const MsgPack& obj)
{
    const auto& arr = obj.array_items();
    std::vector<T> result;
    for (auto& item : arr)
    {
        result.push_back(T::from_object(item));
    }
    return result;
}