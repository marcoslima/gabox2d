#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <msgpack/helpers.h>
#include <msgpack/msgpack.hpp>

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
                 const lst_car_t &population_,
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
              , gps(0.0)
              , bestFitness(0.0) {}

        template<class T>
        void pack(T &pack)
        {
            pack(generation);
            pack(gps);
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
            auto data = msgpack::pack(status);
            return string(data.begin(), data.end());
        }

        GaStatus getStatus()
        {
            return _status;
        }

        bool deserializeGaStatus(const std::vector<uint8_t> &data)
        {
            try
            {
                // cout << "Deserializing data: " << data.size() << " bytes..." << endl;
                const auto status = msgpack::unpack<GaStatus>(data);
                _status = status;
                return true;
            } catch (const std::exception &e)
            {
                cout << "Error deserializing data: " << e.what() << endl;
                return false;
            }
        }
    };
}
