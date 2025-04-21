#include <worker/worker.h>
#include <World.h>
#include <CarDef.h>
#include <PhysCar.h>
#include <iostream>
#include <util/base64.h>
#include <worker_comms/individuals_batch.pb.h>
#include <worker_comms/results_batch.pb.h>

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

string serialize_results(const map_measures_results_t &results)
{
    batchResults batch_results;

    for (const auto &result: results)
    {
        auto* fitness_params = new fitnessParams();
        fitness_params->set_contact1(result.second.contact1);
        fitness_params->set_contact2(result.second.contact2);
        fitness_params->set_velocity(result.second.velocity);
        fitness_params->set_distance(result.second.distance);
        fitness_params->set_time(result.second.time);
        fitness_params->set_is_dead(result.second.is_dead);

        auto* individual_result = batch_results.add_results();
        individual_result->set_individual_id(result.first);
        individual_result->set_allocated_fitness_params(fitness_params);
    }
    string serialized_data;
    batch_results.SerializeToString(&serialized_data);
    return serialized_data;
}

pair<env_data_t, map_individuals_t> deserialize_work_payload(const string& result)
{
    individualsBatch individuals_batch;
    if (!individuals_batch.ParseFromString(result))
    {
        std::cerr << "Failed to parse deserialized results" << std::endl;
        return {};
    }
    env_data_t env_data;
    env_data.tlx = individuals_batch.environment().tlx();
    env_data.tly = individuals_batch.environment().tly();
    env_data.brx = individuals_batch.environment().brx();
    env_data.bry = individuals_batch.environment().bry();
    for (const auto &point: individuals_batch.environment().ground())
    {
        vec2f_t thisPoint(point.x(), point.y());
        env_data.ground.emplace_back(thisPoint);
    }
    map_individuals_t individuals;
    for (const auto &individual: individuals_batch.batch())
    {
        individuals[individual.individual_id()] = individual.genome();
    }
    return {env_data, individuals};
}

void do_work(const string& encoded_data)
{
    // Decode the base64-encoded string
    const auto data = base64_to_binary(encoded_data);
    const auto payload = deserialize_work_payload(data);
    const auto& env_data = payload.first;
    const auto& individuals = payload.second;

    // Call the _do_measures function with the decoded data
    const auto results = _do_measures(env_data, individuals, 60.0f);

    const string serialized_result = serialize_results(results);

    send_result_out(serialized_result);
}
