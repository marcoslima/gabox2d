#include <algorithm>
#include <future>
#include <random_genes_generator.h>
#include <thread>
#include "car.h"
#include "util/cronometro.h"
#include "CRandom.h"
#include "ga.h"
#include <iostream>
#include <util/base64.h>
#include <sstream>
#include <boost/process.hpp>
#include <worker_comms/individuals_batch.pb.h>
#include <worker_comms/results_batch.pb.h>


using namespace std;

namespace GA
{
    CRandom random(static_cast<unsigned>(time(nullptr)));

    bool _do_mutate(const float _mutacao)
    {
        return random.real_random(0.0, 100.0) < _mutacao;
    }

    CGa::CGa(car_factory_ptr_t car_factory)
        : _carFactory(std::move(car_factory))
          , _max_t(0)
          , _populacao(0)
          , _elitismo(0)
          , _alienismo(0)
          , _crossover(0)
          , _mutacao(0)
          , _geracao(0)
          , _bMassExtintion(false)
          , _nCount(0)
    {
        // InitializeCriticalSection(&m_cs);
        _bLogOpenned = false;
    }

    CGa::~CGa() = default;

    void CGa::_cria_populacao()
    {
        m_populacao.clear();
        generate_n(back_inserter(m_populacao), _populacao,
                   [&]
                   {
                       return _carFactory->createRandomCar();
                   });
    }

    void CGa::setParams(const size_t nPopulacao,
                        const size_t nElitismo,
                        const float crossover,
                        const float mutacao,
                        const size_t nAlienismo,
                        const size_t nMutInt,
                        const float dMax_t)
    {
        _populacao = nPopulacao;
        _elitismo = nElitismo;
        _crossover = crossover;
        _mutacao = mutacao;
        _alienismo = nAlienismo;
        _max_t = dMax_t;
    }

    void CGa::BeginEvolve()
    {
        // Cria a população:
        _cria_populacao();

        _bMassExtintion = false;
        _strId2Include.clear();
        _geracao = 1;

        // _fileLog1.Open("log1.txt",CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyWrite);
        _nCount = 0;
        _bLogOpenned = true;
    }

    bool CGa::_not_in_melhores(const string &current_best)
    {
        return !m_melhores_set.contains(current_best);
    }

    map_measures_results_t CGa::_do_measures_parallel(
        const env_data_t &env_data,
        const map_individuals_t &individuals,
        atomic<bool> &stop_ga) const
    {
        const auto num_threads = static_cast<unsigned int>(std::thread::hardware_concurrency() * 1.5);

        // Calculate partition size
        const size_t total_individuals = individuals.size();
        const size_t partition_size = total_individuals / num_threads;

        vector<map_individuals_t> partitions(num_threads);

        size_t individual_idx = 0;
        for (const auto &[id, genes]: individuals)
        {
            const size_t partition_idx = individual_idx / partition_size;
            if (partition_idx < num_threads)
            {
                partitions[partition_idx][id] = genes;
            } else
            {
                // Put remaining individuals in the last partition
                partitions[num_threads - 1][id] = genes;
            }
            individual_idx++;
        }

        // Create threads and futures for results
        vector<future<map_measures_results_t> > futures;

        // Launch threads
        for (const auto &partition: partitions)
        {
            if (!partition.empty())
            {
                futures.push_back(
                    async(std::launch::async, [this, &env_data, &partition]
                    {
                        return _do_measures(env_data, partition);
                    })
                );
            }
        }

        // Collect results
        map_measures_results_t combined_results;
        for (auto &future: futures)
        {
            if (stop_ga.load()) break;

            auto results = future.get();
            combined_results.insert(results.begin(), results.end());
        }

        return combined_results;
    }

    void CGa::_do_calc_points(const map_measures_results_t &measures_results)
    {
        for (size_t i = 0; i < m_populacao.size(); ++i)
            m_populacao[i]->calc_fitness(measures_results.at(i), _max_t);
    }

    void CGa::_do_sort()
    {
        ranges::sort(m_populacao.begin(), m_populacao.end(),
                     [](const auto &lhs, const auto &rhs)
                     {
                         return lhs->getFitness() < rhs->getFitness();
                     });
    }

    void CGa::Ordena(const env_data_t &env_data, atomic<bool> &stop_ga)
    {
        map_individuals_t individuals;
        for (size_t i = 0; i < m_populacao.size(); ++i)
        {
            individuals[i] = m_populacao[i]->getGenes();
        }

        const auto measure_results = _do_measures_parallel(env_data, individuals, stop_ga);

        _do_calc_points(measure_results);
        _do_sort();

        _carWinner = m_populacao.front()->getGenes();

        if (m_melhores.empty() || _not_in_melhores(_carWinner))
        {
            m_melhores.emplace_back(_geracao, _carWinner);
            m_melhores_set.insert(_carWinner);
        }
    }

    void CGa::_do_elitism()
    {
        const auto it = m_populacao.begin();
        const auto end = std::next(it, static_cast<long>(_elitismo));
        m_nova.reserve(_populacao);
        transform(it, end, back_inserter(m_nova),
                  [](const auto &car)
                  {
                      return car->getGenes();
                  });
    }

    string CGa::_generate_random_genes()
    {
        return generate_random_genes(CCarDef::bits::len());
    }

    void CGa::_do_alienism()
    {
        generate_n(back_inserter(m_nova), _alienismo,
                   [&]
                   {
                       return _generate_random_genes();
                   });
    }

    void CGa::_do_manual_include()
    {
        if (!_strId2Include.empty())
        {
            m_nova.push_back(_strId2Include);
            _strId2Include.clear();
        }
    }

    string CGa::serialize_work_payload(const env_data_t &env_data, const map_individuals_t &individuals) const // NOLINT(*-convert-member-functions-to-static)
    {
        individualsBatch batch;
        auto* environment = new envData();
        environment->set_tlx(env_data.tlx);
        environment->set_tly(env_data.tly);
        environment->set_brx(env_data.brx);
        environment->set_bry(env_data.bry);
        for (const auto &ground: env_data.ground)
        {
            auto* vec2f = environment->add_ground();
            vec2f->set_x(ground.x);
            vec2f->set_y(ground.y);
        }
        batch.set_allocated_environment(environment);
        for (const auto &[individual_id, genes]: individuals)
        {
            auto* individual = batch.add_batch();
            individual->set_individual_id(individual_id);
            individual->set_genome(genes);
        }

        string serialized_data;
        batch.SerializeToString(&serialized_data);

        return serialized_data;
    }

    map_measures_results_t CGa::desserialize_results(const std::string &serialized_result) const // NOLINT(*-convert-member-functions-to-static)
    {
        map_measures_results_t results;

        batchResults batch_results;
        if (!batch_results.ParseFromString(serialized_result))
        {
            std::cerr << "Failed to parse deserialized results" << std::endl;
            return results;
        }
        for (const auto &individual: batch_results.results())
        {
            const auto& result_fitness = individual.fitness_params();
            const fitness_params_t fitness_params(
                result_fitness.contact1(),
                result_fitness.contact2(),
                result_fitness.velocity(),
                result_fitness.distance(),
                result_fitness.time(),
                result_fitness.is_dead());
            auto individual_id = individual.individual_id();
            results[individual_id] = fitness_params;
        }
        return results;
    }

    map_measures_results_t CGa::_do_measures(const env_data_t &env_data, const map_individuals_t &individuals) const
    {
        const string serialized_data = serialize_work_payload(env_data, individuals);
        const string b64_encoded = binary_to_base64(serialized_data);

        boost::process::ipstream pipe_stdout;
        boost::process::ipstream pipe_stderr;
        stringstream ss;
        ss << "GaBox2d " << b64_encoded;
        boost::process::child c(ss.str().c_str(),
                                 boost::process::std_out > pipe_stdout,
                                 boost::process::std_err > pipe_stderr);

        string output, line;
        while (pipe_stdout && std::getline(pipe_stdout, line))
        {
            output += line + "\n";
        }

        c.wait();

        // decode results from base64:
        const auto pack_result = base64_to_binary(output);

        return desserialize_results(pack_result);
    }

    void CGa::_1Select()
    {
        // A população já está ordenada.
        _do_elitism();
        _do_alienism();
        _do_manual_include();
        _populate_weights();
    }

    genes_pair_t CGa::_get_parents()
    {
        size_t nId1, nId2;
        while (true)
        {
            nId1 = _roulette_select();
            nId2 = _roulette_select();

            // Clamp no range permitido
            nId1 = std::min(nId1, m_populacao.size() - 1);
            nId2 = std::min(nId2, m_populacao.size() - 1);

            if (nId1 != nId2)
                break;
        }

        const auto itCar1 = std::next(m_populacao.begin(), static_cast<long>(nId1));
        const auto itCar2 = std::next(m_populacao.begin(), static_cast<long>(nId2));

        return make_pair((*itCar1)->getGenes(), (*itCar2)->getGenes());
    }

    bool CGa::_random_do_crossover_or_not() const
    {
        return random.real_random(0.0, 100.0) < _crossover;
    }

    genes_pair_t crossover(const genes_pair_t &genes_pair, const size_t crosspoint)
    {
        const auto parent1 = genes_pair.first;
        const auto parent2 = genes_pair.second;

        auto child1 = parent1.substr(0, crosspoint) + parent2.substr(crosspoint);
        auto child2 = parent2.substr(0, crosspoint) + parent1.substr(crosspoint);

        return make_pair(child1, child2);
    }

    string _double_point_crossover(const string &parent1, const string &parent2, const size_t crosspoint1,
                                   const size_t crosspoint2)
    {
        return parent1.substr(0, crosspoint1) +
               parent2.substr(crosspoint1, crosspoint2 - crosspoint1) +
               parent1.substr(crosspoint2);
    }

    genes_pair_t crossover(const genes_pair_t &genes_pair, const size_t crosspoint1, const size_t crosspoint2)
    {
        const auto parent1 = genes_pair.first;
        const auto parent2 = genes_pair.second;

        auto child1 = _double_point_crossover(parent1, parent2, crosspoint1, crosspoint2);
        auto child2 = _double_point_crossover(parent2, parent1, crosspoint1, crosspoint2);
        return make_pair(child1, child2);
    }

    genes_pair_t _make_single_point_crossover(const string &car1, const string &car2)
    {
        const auto cross_point = random.rand_int<size_t>(1, CCarDef::bits::len() - 2);
        return crossover(make_pair(car1, car2), cross_point);
    }

    genes_pair_t _make_double_point_crossover(const string &car1, const string &car2)
    {
        const auto cross_point1 = random.rand_int<size_t>(1, CCarDef::bits::len() - 2);
        const auto cross_point2 = random.rand_int(cross_point1 + 1, CCarDef::bits::len() - 1);

        return crossover(make_pair(car1, car2), cross_point1, cross_point2);
    }

    void CGa::_2Crossover()
    {
        unordered_set<string> in_population;
        while (m_nova.size() < _populacao)
        {
            const auto parents = _get_parents();

            // Faz crossover?
            if (_random_do_crossover_or_not())
            {
                const auto childs = _make_single_point_crossover(parents.first, parents.second);

                // Verifica se o filho já existe na população
                if (in_population.contains(childs.first) || in_population.contains(childs.second)) continue;

                in_population.insert(childs.first);
                in_population.insert(childs.second);
                m_nova.push_back(childs.first);
                m_nova.push_back(childs.second);
            } else
            {
                m_nova.push_back(parents.first);
                m_nova.push_back(parents.second);
            }
        }
    }

    void CGa::_mutate_genes(string &genes)
    {
        const auto point_of_mutation = random.rand_int<size_t>(0, CCarDef::bits::len() - 1);

        ///// O legível:
        // const auto intensidade = random.discrete_random<char>(1, 10);
        // const auto direcao = random.discrete_random<char>(0, 1)?(1):(-1);
        // const auto mutacao = static_cast<char>(intensidade * direcao);
        // const auto new_gene = static_cast<char>(_genes[point_of_mutation] + mutacao);
        // const auto g = std::clamp<char>(new_gene, 'A', 'Z');
        // _genes[point_of_mutation] = g;

        //// O performático: (nunca edite: faça acima e depois remonte o abaixo)
        // genes[point_of_mutation] = std::clamp<char>(static_cast<char>(genes[point_of_mutation]
        //                                                               + static_cast<char>(
        //                                                                   random.discrete_random<char>(1, 10)
        //                                                                   * random.discrete_random<char>(0, 1)
        //                                                                       ? (1)
        //                                                                       : (-1))), 'A', 'Z');
        genes[point_of_mutation] = genes[point_of_mutation] == '0' ? '1' : '0';
    }

    void CGa::_3Mutate()
    {
        const size_t first_new = _elitismo + _alienismo;
        const size_t nova_len = m_nova.size();
        for (auto i = first_new; i < nova_len; i++)
        {
            if (random.real_random(0.0, 100.0) > _mutacao)
                _mutate_genes(m_nova[i]);
        }
    }

    void CGa::_4AdvanceGeneration()
    {
        // Nova geração:
        m_populacao.clear();

        // Se for para fazer extinção em massa, criamos aleatórios no lugar:
        if (_bMassExtintion)
        {
            _cria_populacao();
            _bMassExtintion = false;
            _geracao = 1;
        } else
        {
            ranges::transform(m_nova, back_inserter(m_populacao),
                              [&](const auto &genes)
                              {
                                  return _carFactory->createCarFromGenes(genes);
                              });
            _geracao++;
        }

        m_nova.clear();
    }

    void CGa::_populate_weights()
    {
        _vec_weights.clear();
        _vec_weights.reserve(m_populacao.size());
        // Extract fitness values and add them as weights
        for (const auto &car: m_populacao)
        {
            // Use fitness as weight (higher fitness = higher probability)
            // May need to invert depending on how fitness is calculated
            _vec_weights.push_back(1.0 / car->getFitness());
        }

        // Create a discrete distribution based on the weights
        _roulette_distribution = std::discrete_distribution<size_t>(_vec_weights.begin(), _vec_weights.end());
    }

    size_t CGa::_roulette_select()
    {
        return _roulette_distribution(random.get_engine());
    }

    void CGa::MassExtinctionEvent()
    {
        _bMassExtintion = true;
    }

    void CGa::IncludeId(const string &strGenes)
    {
        _strId2Include = strGenes;
    }

    void CGa::Step()
    {
        _1Select();
        _2Crossover();
        _3Mutate();
        _4AdvanceGeneration();
    }

    [[nodiscard]] size_t CGa::getPopulacaoLen() const
    {
        return _populacao;
    }

    [[nodiscard]] size_t CGa::getGeracao() const
    {
        return _geracao;
    }

    [[nodiscard]] icar_ptr_t CGa::getBest() const
    {
        return m_populacao.front()->clone();
    }

    const vec_car_t &CGa::getPopulacao() const
    {
        return m_populacao;
    }

    const vec_melhores_t &CGa::getMelhores() const
    {
        return m_melhores;
    }

    // TODO: Mover esta função para um lugar mais apropriado
    /*
    bool CGa::OpenLogFile(void)
    {
        CString strFile;
        CTime   tmNow = CTime::GetCurrentTime();
        strFile.Format("GA_%s_log", tmNow.Format("%Y%m%d%H%M%S"));

        CString strDat = strFile + ".dat";
        CString strPlt = strFile + ".plt";

        CFile filePlt;
        if(!filePlt.Open(strPlt,CFile::modeReadWrite|CFile::modeCreate|CFile::shareExclusive))
        {
            return false;
        }

        CString strPltContent;
        strPltContent.Format(	"reset;\n"
                                "set terminal jpeg font arial 26 size 1400,750;\n"
                                "set grid xtics x2tics ytics y2tics;\n"
                                "show label;\n"
                                "set xlabel \"Gera��o\";\n"
                                "set ylabel \"Pontua��o\";\n"
                                "set output \"%s.jpg\";\n"
                                "plot	\"%s\" with lines title  \"fit\";\n"
                                , strFile,strDat);

        filePlt.Write(strPltContent,strPltContent.GetLength());
        filePlt.Close();


        if(!_fileLog.Open(strDat,CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyWrite))
        {
            return false;
        }

        return true;
    }
    */
}
