#pragma once
#include <msgpack11.hpp>

namespace GA
{
    class fitness_params_t
    {
    public:
        float contact1;
        float contact2;
        float velocity;
        float distance;
        float time;
        bool is_dead;

        msgpack11::MsgPack::object to_object() const
        {
            return {{"contact1", contact1},
                    {"contact2", contact2},
                    {"velocity", velocity},
                    {"distance", distance},
                    {"time", time},
                    {"is_dead", is_dead}};
        }

        static fitness_params_t from_object(const msgpack11::MsgPack &obj)
        {
            auto items = obj.object_items();
            return {items["contact1"].float32_value(),
                    items["contact2"].float32_value(),
                    items["velocity"].float32_value(),
                    items["distance"].float32_value(),
                    items["time"].float32_value(),
                    items["is_dead"].bool_value()};
        }
    };
}
