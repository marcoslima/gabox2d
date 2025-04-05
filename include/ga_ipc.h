#pragma once

#include <string>
#include <vector>
#include <ga.h>
#include <msgpack/msgpack.hpp>

namespace ipc {
    struct GaStatus {
        int generation;
        double gps; // generations per second
        double bestFitness;
        std::string bestGenes;
        GA::vec_genes_t population;
        GA::vec_melhores_t best_history;
    };

    class GaStatusSerializer
    {
        static const string START;
        static const string END;
        std::string _data;
        GaStatus current_status_{};

        bool _try_parse();
        bool _parse(size_t start, size_t length);
    public:
        // Serialization functions for GaStatus
        static std::string serializeGaStatus(const GaStatus& status);
        GaStatus getStatus();

        bool deserializeGaStatus(const std::string &data);
    };
}
