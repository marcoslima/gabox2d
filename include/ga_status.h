#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ga.h>
#include <ga_status/ga_status.pb.h>


namespace ipc
{
    using pair_float_string_t = std::pair<float, std::string>;
    using pair_size_string_t = std::pair<size_t, std::string>;

    class GaStatus
    {
    public:
        size_t generation;
        float gps; // generations per second
        float bestFitness;
        std::string bestGenes;
        std::vector<pair_float_string_t> population;
        GA::vec_melhores_t best_history;

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
            std::ranges::copy(best_history_, std::back_inserter(best_history));
        }

        GaStatus()
            : generation(0)
              , gps(0.0f)
              , bestFitness(0.0) {}
    };

    class GaStatusSerializer
    {
        GaStatus _status{0, 0.0, 0.0, "", {}, {}};

    public:
        static std::string serializeGaStatus(GaStatus &status)
        {
            const auto gaStatus = make_unique<ga_status::v1::GaStatus>();
            gaStatus->set_generation(status.generation);
            gaStatus->set_gps(status.gps);
            gaStatus->set_bestfitness(status.bestFitness);
            gaStatus->set_bestgenes(status.bestGenes);
            for (const auto &ind : status.population)
            {
                auto *individual = gaStatus->add_population();
                individual->set_fitness(ind.first);
                individual->set_genome(ind.second);
            }
            for (const auto &best : status.best_history)
            {
                auto *history = gaStatus->add_history();
                history->set_generation(best.generation);
                history->set_genome(best.genes);
                history->set_fitness(best.fitness);
            }
            std::string serialized_data;
            gaStatus->SerializeToString(&serialized_data);
            return serialized_data;
        }

        GaStatus getStatus()
        {
            return _status;
        }

        bool deserializeGaStatus(const std::string &data)
        {
            ga_status::v1::GaStatus gaStatus;
            if (!gaStatus.ParseFromString(data))
            {
                return false;
            }

            _status.generation = gaStatus.generation();
            _status.gps = gaStatus.gps();
            _status.bestFitness = gaStatus.bestfitness();
            _status.bestGenes = gaStatus.bestgenes();
            _status.population.clear();
            _status.best_history.clear();
            for (const auto &ind : gaStatus.population())
            {
                _status.population.emplace_back(ind.fitness(), ind.genome());
            }
            for (const auto &best : gaStatus.history())
            {
                _status.best_history.emplace_back(best.generation(), best.fitness(), best.genome());
            }
            return true;
        }
    };
}
