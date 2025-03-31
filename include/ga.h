#pragma once

#include <atomic>
#include <string>
#include <unordered_set>

#include "lmfisica.h"


using namespace std;
using namespace LmFisica;

#include <icar.h>
#include <IWorld.h>

using vec_double_t = vector<double>;

namespace GA
{
    using vec_string_t = vector<string>;
    using melhor_t = pair<size_t, string>;
    using vec_melhores_t = vector<melhor_t>;
    using vec_genes_t = vector<pair<float, string>>;

    class CGa
    {
        car_factory_ptr_t _carFactory;

        // Parâmetros para o algoritmo genético
        float  _max_t; // Tempo máximo de simulação
        size_t _populacao; // Número de indivíduos por geração
        size_t _elitismo; // Quantos indivíduos vão para geração seguinte inalterados
        size_t _alienismo; // Número de indivíduos randômicos inseridos a cada geração
        size_t _mut_int; // A mutação será +/- _mut_int numa letra
        float  _crossover; // Percentual de probabilidade de ocorrer crossover
        float  _mutacao; // Percentual de probabilidade de ocorrer mutação
        string _carWinner; // Indivíduo mais adaptado da geração atual (genes)

        // Acumuladores do algoritmo
        size_t _geracao; // Geração atual

        // Sinalização de extinção em massa:
        bool _bMassExtintion;

        // Individuo para ser incluido na próxima geração:
        string _strId2Include;

        // Controle de acesso às estruturas internas da classe
        // CRITICAL_SECTION	m_cs;

        // Armazenamento dos indivíduos
        // População (objetos CCar)
        lst_car_t m_populacao;

        // Nova população
        vec_car_t m_nova;

        vec_double_t _vec_select,
                     _vec_crossover,
                     _vec_mutate,
                     _vec_advance;
        // CFile _fileLog1;
        size_t _nCount;
        bool _bLogOpenned;

        // Histórico de melhoramentos
        vec_melhores_t m_melhores;
        unordered_set<std::string> m_melhores_set;

        // Funções internas
    private:
        void _do_elitism();
        void _do_alienism();
        void _do_manual_include();
        void _do_measures(const PHYS::IWorldPtr &world, atomic<bool> &stop_ga) const;
        void _do_calc_points();
        void _do_sort();
        void _cria_populacao();
        void _1Select();
        void _2Crossover();
        void _3Mutate();
        void _4AdvanceGeneration();

        // Interface
    public:
        // Construtor/destrutor
        explicit CGa(car_factory_ptr_t car_factory);

        ~CGa();

        // Especificação dos parâmetros do GA
        void setParams(size_t nPopulacao = 90,
                       size_t nElitismo = 1,
                       float crossover = 70,
                       float mutacao = 65,
                       size_t nAlienismo = 0,
                       size_t nMutInt = 5,
                       float dMax_t = 60);

        // Inicia o algoritmo
        void BeginEvolve();

        bool _not_in_melhores(const string &current_best);

        // Testa e ordena os indivíduos
        void Ordena(const PHYS::IWorldPtr &world, atomic<bool> &stop_ga);

        // Seleciona, cruza, muta e passa para geração seguinte
        void Step();

        // Comanda a extinção em massa para a próxima geração
        void MassExtinctionEvent();

        // Inclui um indivíduo na próxima geração
        void IncludeId(const string &strGenes);

        // Queries
        // CCar getWinner()
        // {
        //     return m_carWinner;
        // }

        [[nodiscard]] size_t getPopulacaoLen() const;
        [[nodiscard]] size_t getGeracao() const;
        [[nodiscard]] icar_ptr_t getBest() const;
        [[nodiscard]] const lst_car_t &getPopulacao() const;
        [[nodiscard]] const vec_melhores_t &getMelhores() const;

        // bool OpenLogFile();
    };
}
