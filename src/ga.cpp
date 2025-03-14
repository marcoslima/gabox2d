#include <algorithm>
using namespace std;
#include "CCronometro.h"
#include "CRandom.h"
#include "ga.h"
#include <iostream>

#define VERIFY(x, msg) if(!(x)) {cout << "FAIL: " << msg << endl;}

namespace GA
{
	CRandom random(unsigned(time(nullptr)));

CGa::CGa()
	: _max_t(0),
	_populacao(0),
	_elitismo(0),
	_alienismo(0),
	_mut_int(0),
	_crossover(0),
	_mutacao(0),
	_geracao(0),
	_bMassExtintion(false),
	_nCount(0)
{
	// InitializeCriticalSection(&m_cs);
	_bLogOpenned = false;
}

CGa::~CGa()
{
	// DeleteCriticalSection(&m_cs);

	// if(_bLogOpenned)
	// 	_fileLog1.Close();
}


void CGa::_cria_populacao()
{
	m_populacao.clear();

	for(size_t i = 0; i < _populacao; i++)
		m_populacao.push_back(createRandomCar());
}

void CGa::setParams(	size_t	nPopulacao	, 
						size_t	nElitismo	, 
						float	crossover	, 
						float	mutacao		,
						size_t	nAlienismo	,
						size_t	nMutInt		,
						float	dMaxT		)
{
	_populacao = nPopulacao	;
	_elitismo  = nElitismo	;
	_crossover = crossover	;
	_mutacao   = mutacao	;
	_alienismo = nAlienismo	;
	_mut_int   = nMutInt	;
	_max_t	   = dMaxT		;
}

void CGa::BeginEvolve()
{
	// Cria a população:
	_cria_populacao();

	_bMassExtintion = false;
	_strId2Include.clear();
	_geracao	  = 1;

	// _fileLog1.Open("log1.txt",CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyWrite);
	_nCount = 0;
	_bLogOpenned = true;
}


bool pred(const CCar& left, const CCar& right)
{
   return left.getPontuacao() < right.getPontuacao();
}

void CGa::_do_measures(b2WorldId worldId, atomic<bool>& stop_ga)
{
	for (auto& car : m_populacao) 
	{
		try 
		{
			car.Medir(worldId, _max_t);
		} 
		catch (const std::exception& e) 
		{
			std::cerr << "CGa::Ordena:Medir: " << e.what() << '\n';
		}
	
		// if (stop_ga.load()) break;
	}
}

void CGa::_do_calc_points()
{
	for(auto it = m_populacao.begin(); it!= m_populacao.end(); it++) it->calc_fitness(_max_t);
}

void CGa::_do_sort()
{
	cout << "n: " << m_populacao.size() << endl;
	m_populacao.sort(pred);
}

void CGa::Ordena(b2WorldId worldId, atomic<bool>& stop_ga)
{
	cout << "measuring..." << endl;
	_do_measures(worldId, stop_ga);

	cout << "calculating points..." << endl;
	_do_calc_points();

	cout << "sorting..." << endl;
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
	for(it = m_populacao.begin(),i = 0;
		it!= m_populacao.end() && i < _elitismo;
		it++,i++)
	{
		m_nova.push_back(it->getGenes());
	}
}

void CGa::_do_alienism()
{
	for(size_t i = 0; i < _alienismo; i++)
	{
		m_nova.push_back(createRandomCar().getGenes());
	}
}

void CGa::_do_manual_include()
{
	if(!_strId2Include.empty())
	{
		m_nova.push_back(_strId2Include);
		_strId2Include.clear();
	}
}

void CGa::_1Select()
{
	// A população já está ordenada.
	_do_elitism();
	_do_alienism();
	_do_manual_include();
	return;
}

void CGa::_2Crossover()
{
	size_t i, nId1, nId2, nSize = m_populacao.size();
	double nStdev = nSize / 1.0;
	size_t nMaxIndex = nSize - 1;
	string str1, str2, strt;
	size_t nCross;
	size_t zero = 0;
	lst_car_t::iterator it;

	while(m_nova.size() < _populacao)
	{
		// Escolha dos pais:
		nId1 = random.rand_int(zero, nSize);
		nId2 = random.rand_int(zero, nSize);

		// Clamp no range permitido
		nId1 = std::min(nMaxIndex, nId1);
		nId2 = std::min(nMaxIndex, nId2);


		if(nId1 == nId2)
			continue;
		
		// Faz crossover?
		it = m_populacao.begin();for(i = 0; i < nId1; i++,it++);
		str1 = it->getGenes();

		it = m_populacao.begin();for(i = 0; i < nId1; i++,it++);
		str2 = it->getGenes();

		if(random.rand_int(0, 100) < _crossover)
		{
			nCross = 1 + random.rand_int(zero, static_cast<size_t>(GENES-2));
			strt = str1.substr(0, nCross);
			str1 = str2.substr(0, nCross) + str1.substr(GENES-nCross);
			str2 = strt + str2.substr(GENES-nCross);
		}

		VERIFY(str1.size() == GENES, "Crossover: str1.size() == GENES, found: " << str1.size() );
		VERIFY(str2.size() == GENES, "Crossover: str2.size() == GENES, found: " << str2.size() );
		m_nova.push_back(str1);
		m_nova.push_back(str2);
	}

	return;
}

bool _do_mutate(float _mutacao)
{
	return random.random(0.0, 100.0) < _mutacao;
}

void CGa::_3Mutate()
{
	size_t nMut;
	char g;
	size_t i,nSize = m_nova.size();
	size_t zero = 0;
	size_t max_gene = static_cast<size_t>(GENES-1);
	for(i = (_elitismo + _alienismo) ; i < nSize; i++)
	{
		if(! _do_mutate(_mutacao))
			continue;

		// Mutação:
		// Ponto da mutação:
		nMut = random.rand_int(zero, max_gene);

		// Intensidade e direção da mutação:
		char intensidade = random.discrete_random<char>(1, _mut_int);
		char direcao = random.discrete_random<char>(0, 1)?(1):(-1);
		char mutacao = intensidade * direcao;
		g = m_nova[i][nMut] + mutacao;

		g = std::clamp(g, 'A', 'Z');

		m_nova[i][nMut] = g;
	}
}

void CGa::_4AdvanceGeneration()
{
	// Nova geração:
	m_populacao.clear();

	// Se for para fazer extinção em massa, criamos aleatórios no lugar:
	if(_bMassExtintion)
	{
		_cria_populacao();
		_bMassExtintion = false;
		_geracao = 1;
	}
	else
	{
		const size_t nSize = m_nova.size();
		for(size_t i = 0; i < nSize && i < _populacao; i++)
		{
			m_populacao.push_back(createCarFromGenes(m_nova[i]));
		}
		_geracao++;
	}

	m_nova.clear();
}

void CGa::MassExtinctionEvent()
{
	_bMassExtintion = true;	
}

void CGa::IncludeId(const string& strGenes)
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


// TODO: Mover esta fun��o para um lugar mais apropriado
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
