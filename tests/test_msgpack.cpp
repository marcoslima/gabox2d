#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <ga_ipc.h>
#include <msgpack/msgpack.hpp>
#include <iostream>


TEST_CASE("MsgPack", "[msgpack]")
{
    ipc::GaStatus status;

    status.generation = 1;
    status.gps = 2.0;
    status.bestFitness = 3.0;
    status.bestGenes = "100101011001010110101010101110000110101111010";
    for (const auto &gene: vector{
             pair_float_string_t(0.8, "100101011001010110101010101110000110101111"),
             pair_float_string_t(0.7, "100101011001010110101010101110000110101111"),
             pair_float_string_t(0.6, "100101011001010110101010101110000110101111")
         })
    {
        status.population.push_back(gene);
    }
    for (const auto &gene: vector{
             pair_size_string_t(12, "100101011001010110101010101110000110101111"),
             pair_size_string_t(13, "100101011001010110101010101110000110101111"),
             pair_size_string_t(15, "100101011001010110101010101110000110101111")
         })
    {
        status.best_history.push_back(gene);
    }

    auto serialized = msgpack::pack(status);
    // cout << "Serialized size: " << serialized.size() << endl;
    // for (auto &byte: serialized)
    // {
    //     cout << std::hex << static_cast<int>(byte) << " ";
    // }
    // cout << endl;
    auto deserialized = msgpack::unpack<ipc::GaStatus>(serialized.data(), serialized.size());
    REQUIRE(deserialized.generation == 1);
    REQUIRE(deserialized.gps == status.gps);
    REQUIRE(deserialized.bestFitness == status.bestFitness);
    REQUIRE(deserialized.bestGenes == status.bestGenes);
}
