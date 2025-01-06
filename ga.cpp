#include <algorithm>
using namespace std;
#include "CCronometro.h"
#include "CRandom.h"
#include "ga.h"

namespace GA
{

CRandom random{unsigned(time(0))};

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
		m_populacao.push_back(CCar());
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

void CGa::BeginEvolve(void)
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

void CGa::Ordena(b2WorldId worldId, atomic<bool>& stop_ga)
{

	float pts;

	// Ordenamos (medições)
	lst_car_t::iterator it;
	for(it = m_populacao.begin();
		it!= m_populacao.end();
		it++)
	{
		it->Medir(worldId, _max_t);

		if(stop_ga.load()) break;
	}

	// Ajustamos os pontos
	float c1,c2,v,d,t;
	float p1,p2,p3,p4,p5;
	for(it = m_populacao.begin();
		it!= m_populacao.end();
		it++)
	{
		c1 = it->m_contatoR1	;
		c2 = it->m_contatoR2	;
		v  = it->m_vm			;
		d  = it->m_distancia	;
		t  = it->getT()			; 

		/*
			A pontua��o � meio dif�cil por que, para ser absoluta, n�o pode
			depender da popula��o.
			Mas se n�o depender, � muito dif�cil normalizar as partes (c1, c2, v, d e t)
			Sem normalizar, a dist�ncia, por exemplo, que pode ter valores grandes,
			ser� mais importante que os outros par�metros de avalia��o.

			Para resolver isso, vamos fazer o fitness como sendo a dist�ncia
			euclidiana de um vetor composto pelos par�metros de avalia��o � um 
			vetor constante ideal.
			
			O vetor ser� (c1, c2, v, d, t).
			O vetor objetivo ideal ser�: (t_max,t_max,1000,1000,0).
			Ou seja, 
				. o tempo de contato das rodas � o m�ximo poss�vel
				. A velocidade � a m�xima poss�vel
				. A dist�ncia percorrida � a m�xima poss�vel
				. O tempo gasto � o m�nimo. no caso nem � poss�vel, pois � zero.

			Para n�o gastar um sqrt � toa, faremos o quadrado da dist�ncia.
		*/

		p1 = _max_t - c1;
		p2 = _max_t - c2;
		p3 = 1000 - v;
		p4 = 1000 - d;
		p5 = t; // 0 - t = -t, mas como ser� ao quadrado, deixa t mesmo.

		pts = (p1*p1 + p2*p2 + p3*p3 + p4*p4 + p5*p5);
		
		// Se quebrou, vale um d�cimo de um que n�o quebrou:
		if(it->m_bDead) pts *= 10;

		it->setPontos(pts);
	}

	m_populacao.sort(pred);

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

	return;
}

void CGa::_1Select(void)
{
	size_t i;

	// A popula��o j� est� ordenada.
	// Elitismo:
	lst_car_t::iterator it;
	for(it = m_populacao.begin(),i = 0;
		it!= m_populacao.end() && i < _elitismo;
		it++,i++)
	{
		m_nova.push_back(string("__")+it->getGenesString());
	}

	// Alienismo:
	for(i = 0; i < _alienismo; i++)
	{
		m_nova.push_back(string("__")+(CCar()).getGenesString());
	}

	// Inclus�o arbitr�ria:
	if(!_strId2Include.empty())
	{
		m_nova.push_back(_strId2Include);
		_strId2Include.clear();
	}

	return;
}

void CGa::_2Crossover(void)
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
		str1 = it->getGenesString();

		it = m_populacao.begin();for(i = 0; i < nId1; i++,it++);
		str2 = it->getGenesString();

		if(random.rand_int(0, 100) < _crossover)
		{
			nCross = 1 + random.rand_int(zero, static_cast<size_t>(GENES-2));
			strt = str1.substr(0, nCross);
			str1 = str2.substr(0, nCross) + str1.substr(GENES-nCross);
			str2 = strt + str2.substr(GENES-nCross);
		}

		m_nova.push_back(str1);
		m_nova.push_back(str2);
	}

	return;
}

bool _do_mutate(float _mutacao)
{
	return random.random(0.0, 100.0) < _mutacao;
}

void CGa::_3Mutate(void)
{
	int nMut;
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

		// VERIFY(m_nova[i].Left(2) != "__");

		// Intensidade e direção da mutação:
		char intensidade = random.discrete_random<char>(1, _mut_int);
		char direcao = random.discrete_random<char>(0, 1)?(1):(-1);
		char mutacao = intensidade * direcao;
		g = m_nova[i][nMut] + mutacao;

		g = std::clamp(g, 'A', 'Z');

		m_nova[i][nMut] = g;
	}
}

void CGa::_4AdvanceGeneration(void)
{
	// Nova gera��o:
	m_populacao.clear();

	// Se for para fazer extin��o em massa, criamos aleat�rios no lugar:
	if(_bMassExtintion)
	{
		_cria_populacao();
		_bMassExtintion = false;
		_geracao = 1;
	}
	else
	{
		size_t i,nSize = m_nova.size();
		for(i = 0; i < nSize && i < _populacao; i++)
		{
			if(m_nova[i].substr(0, 2) == "__")
				m_populacao.push_back(CCar(m_nova[i].substr(m_nova[i].size()-2).c_str()));
			else
				m_populacao.push_back(CCar(m_nova[i].c_str()));
		}
		_geracao++;
	}

	m_nova.clear();
	return;
}

void CGa::MassExtinctionEvent(void)
{
	_bMassExtintion = true;	
}

void CGa::IncludeId(string strGenes)
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

void CGa::CopyPopulacao(lst_car_t *pTarget)
{
	*pTarget = m_populacao;

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
