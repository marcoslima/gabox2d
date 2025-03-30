#pragma once

// In a shared header file (ga_ipc.h)
#include <boost/asio.hpp>
#include <string>
#include <vector>

namespace ipc {
    struct GaStatus {
        int generation;
        double gps; // generations per second
        double bestFitness;
        std::string bestGenes;
        std::vector<std::string> population;
    };

    // Serialization functions for GaStatus
    std::string serializeGaStatus(const GaStatus& status);
    GaStatus deserializeGaStatus(const std::string& data);
}
