#include <ga_ipc.h>

namespace ipc
{
    std::string serializeGaStatus(const GaStatus& status)
    {
        std::string data;
        data += std::to_string(status.generation) + "\n";
        data += std::to_string(status.gps) + "\n";
        data += std::to_string(status.bestFitness) + "\n";
        data += status.bestGenes + "\n";
        for (const auto& gene : status.population)
        {
            data += gene + "\n";
        }
        return data;
    }

    GaStatus deserializeGaStatus(const std::string& data)
    {
        GaStatus status;
        std::istringstream stream(data);
        std::string line;

        std::getline(stream, line);
        status.generation = std::stoi(line);

        std::getline(stream, line);
        status.gps = std::stod(line);

        std::getline(stream, line);
        status.bestFitness = std::stod(line);

        std::getline(stream, status.bestGenes);

        while (std::getline(stream, line))
        {
            if (!line.empty())
            {
                status.population.push_back(line);
            }
        }

        return status;
    }
}
