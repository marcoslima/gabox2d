#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <msgpack/helpers.h>
#include <msgpack11.hpp>
using namespace msgpack11;

#include "ga.h"

namespace ipc
{
    class GaStatus
    {
    public:
        size_t generation;
        float gps; // generations per second
        float bestFitness;
        std::string bestGenes;
        std::vector<pair_float_string_t> population;
        std::vector<pair_size_string_t> best_history;

        GaStatus(const size_t generation,
                 const float gps,
                 const float bestFitness,
                 const std::string &bestGenes, // NOLINT(*-pass-by-value)
                 const vec_car_t &population_,
                 const GA::vec_melhores_t &best_history_)
            : generation(generation),
              gps(gps),
              bestFitness(bestFitness),
              bestGenes(bestGenes)
        {
            std::ranges::transform(population_, std::back_inserter(population),
                                   [](const auto &pair)
                                   {
                                       return pair_float_string_t(pair->getFitness(), pair->getGenes());
                                   });
            std::ranges::transform(best_history_, std::back_inserter(best_history),
                                   [](const auto &pair)
                                   {
                                       return pair_size_string_t(pair.first, pair.second);
                                   });
        }

        GaStatus()
            : generation(0)
              , gps(0.0f)
              , bestFitness(0.0) {}

        template<class T>
        void pack(T &pack)
        {
            pack(gps);
            pack(generation);
            pack(bestFitness);
            pack(bestGenes);
            pack(population);
            pack(best_history);
        }
    };

    class GaStatusSerializer
    {
        GaStatus _status{0, 0.0, 0.0, "", {}, {}};

    public:
        static std::string serializeGaStatus(GaStatus &status)
        {
            const MsgPack data = MsgPack::object{
                {"gps", status.gps},
                {"generation", status.generation},
                {"bestFitness", status.bestFitness},
                {"bestGenes", status.bestGenes},
                {"population", to_object(status.population)},
                {"best_history", to_object(status.best_history)}
            };
            return data.dump();
        }

        GaStatus getStatus()
        {
            return _status;
        }

        bool deserializeGaStatus(const std::string &data)
        {
            try
            {
                std::string err;
                const auto obj = MsgPack::parse(data, err);
                _status.generation = obj["generation"].uint32_value();
                _status.gps = obj["gps"].float32_value();
                _status.bestFitness = obj["bestFitness"].float32_value();
                _status.bestGenes = obj["bestGenes"].string_value();
                _status.population = from_object<pair_float_string_t>(obj["population"]);
                _status.best_history = from_object<pair_size_string_t>(obj["best_history"]);
                return true;
            } catch (const std::exception &e)
            {
                cout << "Error deserializing data: " << e.what() << endl;
                return false;
            }
        }
    };
}
