#include <ga_ipc.h>
#include <sstream>

namespace ipc
{
    const string GaStatusSerializer::START = "START_DATA";
    const string GaStatusSerializer::END = "END_DATA";

    std::string GaStatusSerializer::serializeGaStatus(const GaStatus& status)
    {
        std::string data;
        data += START + "\n";
        data += std::to_string(status.generation) + "\n";
        data += std::to_string(status.gps) + "\n";
        data += std::to_string(status.bestFitness) + "\n";
        data += status.bestGenes + "\n";
        data += std::to_string(status.population.size()) + "\n";
        for (const auto& gene : status.population)
        {
            data += std::to_string(gene.first) + "\n" + gene.second + "\n";
        }

        data += std::to_string(status.best_history.size()) + "\n";
        for (const auto& best : status.best_history)
        {
            data += std::to_string(best.first) + "\n" + best.second + "\n";
        }

        data += END + "\n";

        return data;
    }

    bool GaStatusSerializer::deserializeGaStatus(const std::string &data)
    {
        _data += data;
        return _try_parse();
    }

    GaStatus GaStatusSerializer::getStatus()
    {
        return current_status_;
    }

    bool GaStatusSerializer::_try_parse()
    {
        if (_data.size() == 0) return false;

        const auto start = _data.rfind(START);
        const auto end = _data.rfind(END);
        if (start == std::string::npos) return false;
        if (end == std::string::npos) return false;

        const auto length = end - start + END.size();

        return _parse(start, length);
    }

    bool GaStatusSerializer::_parse(const size_t start, const size_t length)
    {
        std::istringstream stream(_data.substr(start, length));
        std::string line;

        std::getline(stream, line);
        if (line != START) return false;

        std::getline(stream, line);
        current_status_.generation = std::stoi(line);

        std::getline(stream, line);
        current_status_.gps = std::stod(line);

        std::getline(stream, line);
        current_status_.bestFitness = std::stod(line);

        std::getline(stream, current_status_.bestGenes);

        std::getline(stream, line);
        const size_t populationSize = std::stoul(line);

        current_status_.population.clear();
        std::string genes;
        for (size_t i = 0; i < populationSize; i++)
        {
            std::getline(stream, line);
            float points = std::stod(line);
            std::getline(stream, genes);
            current_status_.population.emplace_back(points, genes);
        }

        current_status_.best_history.clear();
        std::getline(stream, line);
        const size_t bestHistorySize = std::stoul(line);
        for (size_t i = 0; i < bestHistorySize; i++)
        {
            std::getline(stream, line);
            int generation = std::stoi(line);
            std::getline(stream, genes);
            current_status_.best_history.emplace_back(generation, genes);
        }

        std::getline(stream, line);
        if (line != END) return false;

        _data.clear();
        return true;
    }
}
