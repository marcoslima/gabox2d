#include <algorithm>

#include "car.h"
using namespace std;
#include "CCronometro.h"
#include "CRandom.h"
#include "ga.h"
#include <iostream>

#define VERIFY(x, msg) if(!(x)) {cout << "FAIL: " << msg << endl;}

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
          , _mut_int(0)
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

        for (size_t i = 0; i < _populacao; i++)
            m_populacao.push_back(_carFactory->createRandomCar());
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
        _mut_int = nMutInt;
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

    void CGa::_do_measures(const PHYS::IWorldPtr &world, atomic<bool> &stop_ga) const
    {
        for (auto &car: m_populacao)
        {
            try
            {
                car->Medir(world, _max_t);
            } catch (const std::exception &e)
            {
                std::cerr << "CGa::Ordena:Medir: " << e.what() << '\n';
            }

            // if (stop_ga.load()) break;
        }
    }

    void CGa::_do_calc_points()
    {
        for (const auto &it: m_populacao) it->calc_fitness(_max_t);
    }

    void CGa::_do_sort()
    {
        m_populacao.sort();
    }

    void CGa::Ordena(const PHYS::IWorldPtr &world, atomic<bool> &stop_ga)
    {
        _do_measures(world, stop_ga);
        _do_calc_points();
        _do_sort();

#if 0
	m_carWinner = *(m_populacao.begin());
	if(m_melhores.size() == 0)
	{
		m_melhores.push_back(melhor_t(_geracao,m_carWinner));
	}
	else
	{
		if(m_melhores[m_melhores.size()-1].second.getGenesString() != m_carWinner.getGenesString())
		{
			m_melhores.push_back(melhor_t(_geracao,m_carWinner));
		}
	}
#endif
    }

    void CGa::_do_elitism()
    {
        lst_car_t::iterator it;
        size_t i;
        for (it = m_populacao.begin(), i = 0;
             it != m_populacao.end() && i < _elitismo;
             ++it, i++)
        {
            m_nova.push_back((*it)->clone());
        }
    }

    void CGa::_do_alienism()
    {
        for (size_t i = 0; i < _alienismo; i++)
        {
            m_nova.push_back(CCarFactory().createRandomCar());
        }
    }

    void CGa::_do_manual_include()
    {
        if (!_strId2Include.empty())
        {
            m_nova.push_back(_carFactory->createCarFromGenes(_strId2Include));
            _strId2Include.clear();
        }
    }

    void CGa::_1Select()
    {
        // A população já está ordenada.
        _do_elitism();
        _do_alienism();
        _do_manual_include();
    }

    void CGa::_2Crossover()
    {
        size_t i;
        const size_t nSize = m_populacao.size();
        const size_t nMaxIndex = nSize - 1;

        while (m_nova.size() < _populacao)
        {
            constexpr size_t zero = 0;
            // Escolha dos pais:
            size_t nId1 = random.rand_int(zero, nSize);
            size_t nId2 = random.rand_int(zero, nSize);

            // Clamp no range permitido
            nId1 = std::min(nMaxIndex, nId1);
            nId2 = std::min(nMaxIndex, nId2);

            if (nId1 == nId2)
                continue;

            // Faz crossover?
            auto itCar1 = m_populacao.begin();
            for (i = 0; i < nId1; i++, ++itCar1) {}
            auto itCar2 = m_populacao.begin();
            for (i = 0; i < nId1; i++, ++itCar2) {}
            const auto cross_point = random.rand_int(1, GENES - 2);
            m_nova.push_back((*itCar1)->crossover(*itCar2, cross_point));
            m_nova.push_back((*itCar2)->crossover(*itCar1, cross_point));
        }
    }

    void CGa::_3Mutate()
    {
        const size_t first_new = _elitismo + _alienismo;
        const size_t nova_len = m_nova.size();
        for (auto i = first_new; i < nova_len; i++)
            m_nova[i]->mutate();
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
            ranges::move(m_nova, std::back_inserter(m_populacao));
            _geracao++;
        }

        m_nova.clear();
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
