#include <worker/worker.h>
#include <World.h>
#include <CarDef.h>
#include <PhysCar.h>
#include <iostream>
#include <base64.h>
#include <fstream>
#include <zipstring.h>
#include <msgpack11.hpp>

using namespace msgpack11;


map_measures_results_t _do_measures(
const env_data_t &env_data,
const map_individuals_t &individuals,
const float max_t)
{
    const PHYS::IWorldPtr world = make_shared<PHYS::CWorld>();
    world->create(env_data);

    map_measures_results_t measures_results;

    for (auto &car: individuals)
    {
        CCarDef car_def(car.second);
        const auto phys_car = PHYS::createPhysCar();
        try
        {
            phys_car->measure(world, car_def, max_t);
        } catch (const std::exception &e)
        {
            std::cerr << "CGa::Ordena:Medir: " << e.what() << '\n';
        }
        measures_results[car.first] = phys_car->get_ga_fitness_params();
    }
    world->destroy();
    return measures_results;
}

void send_result_out(const string& result)
{
    const string b64_encoded_result = binary_to_base64(result);
    std::cout << b64_encoded_result << std::endl;
}

string serialize_results(const map_measures_results_t &results) {
    MsgPack::array results_array;
    for (const auto &[id, fitness_params]: results)
    {
        MsgPack::object obj = fitness_params.to_object();
        results_array.emplace_back(MsgPack::object{
            {"id", id},
            {"fitness_params", obj}
        });
    }
    const MsgPack result_object = MsgPack::object{
        {"results", results_array}
    };
    return result_object.dump();
}

void do_work(const string& encoded_data)
{
    // Decode the base64-encoded string
    const auto data = base64_to_binary(encoded_data);

    // Deserialize the data
    string err;
    const MsgPack packed_data = MsgPack::parse(data, err);
    env_data_t env_data;
    env_data.tlx = packed_data["env_data"]["tlx"].float32_value();
    env_data.tly = packed_data["env_data"]["tly"].float32_value();
    env_data.brx = packed_data["env_data"]["brx"].float32_value();
    env_data.bry = packed_data["env_data"]["bry"].float32_value();
    const auto ground_array = packed_data["env_data"]["ground"].array_items();
    for (const auto &vec: ground_array)
    {
        vec2f_t vec2f(vec[0].float32_value(),
                      vec[1].float32_value());
        env_data.ground.push_back(vec2f);
    }

    // Decode individuals
    const auto individuals_array = packed_data["individuals"].array_items();
    map_individuals_t individuals;
    for (const auto &item: individuals_array)
    {
        const auto id = item["id"].uint64_value();
        const auto genes = item["genes"].string_value();
        individuals[id] = genes;
    }

    // Call the _do_measures function with the decoded data
    const auto results = _do_measures(env_data, individuals, 60.0f);
    const string serialized_result = serialize_results(results);

    send_result_out(serialized_result);
}
