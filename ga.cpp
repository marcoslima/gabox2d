#include <stdafx.h>
#include <algorithm>
using namespace std;
#include <mersenne.h>

#include "ga.h"

MTRand mrand;

namespace GA
{


float random(float aMin, float aMax)
{
	return (float)(aMin + mrand.rand(aMax-aMin));
}

CGa::CGa()
{
	InitializeCriticalSection(&m_cs);
}
CGa::~CGa()
{
	DeleteCriticalSection(&m_cs);
}


void CGa::_cria_populacao()
{
	m_populacao.clear();

	for(size_t i = 0; i < _populacao; i++)
		m_populacao.push_back(CCar());
}

void CGa::setParams(	size_t	nPopulacao	, 
						size_t	nElitismo	, 
						double	crossover	, 
						double	mutacao		,
						size_t	nAlienismo	,
						size_t	nMutInt		,
						double	dMaxT		)
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
	_strId2Include.Empty();
	_geracao	  = 1;
	_alpha		  = 10;
}


bool pred(const CCar& left, const CCar& right)
{
   return left.getPontuacao() < right.getPontuacao();
}

void CGa::Ordena(b2World *pWorld, HANDLE hStop)
{
	size_t i;
	double pts;

	// Ordenamos (medições)
	for(i = 0; i < _populacao; i++)
	{
		m_populacao[i].Medir(pWorld,_max_t);

		if(WaitForSingleObject(hStop,0) == WAIT_OBJECT_0)break;
	}

	// Ajustamos os pontos
	double c1,c2,v,d,t;
	double p1,p2,p3,p4,p5;
	double	pmax	= -1,
			pmedio	=  0;

	for(i = 0; i < _populacao; i++)
	{
		c1 = m_populacao[i].m_contatoR1		;
		c2 = m_populacao[i].m_contatoR2		;
		v  = m_populacao[i].m_vm			;
		d  = m_populacao[i].m_distancia		;
		t  = m_populacao[i].getT()			; 

		/*
			A pontuação é meio difícil por que, para ser absoluta, não pode
			depender da população.
			Mas se não depender, é muito difícil normalizar as partes (c1, c2, v, d e t)
			Sem normalizar, a distância, por exemplo, que pode ter valores grandes,
			será mais importante que os outros parâmetros de avaliação.

			Para resolver isso, vamos fazer o fitness como sendo a distância
			euclidiana de um vetor composto pelos parâmetros de avaliação à um 
			vetor constante ideal.
			
			O vetor será (c1, c2, v, d, t).
			O vetor objetivo ideal será: (t_max,t_max,1000,1000,0).
			Ou seja, 
				. o tempo de contato das rodas é o máximo possível
				. A velocidade é a máxima possível
				. A distância percorrida é a máxima possível
				. O tempo gasto é o mínimo. no caso nem é possível, pois é zero.

			Para não gastar um sqrt à toa, faremos o quadrado da distância.
		*/

		p1 = _max_t - c1;
		p2 = _max_t - c2;
		p3 = 1000 - v;
		p4 = 1000 - d;
		p5 = t; // 0 - t = -t, mas como será ao quadrado, deixa t mesmo.

		pts = sqrt(p1*p1 + p2*p2 + p3*p3 + p4*p4 + p5*p5);
		pmax = __max(pmax,pts);

		// Se quebrou, vale um décimo de um que não quebrou:
		if(m_populacao[i].m_bDead) pts *= 10;
		
		m_populacao[i].setPontos(pts);
		pmedio += pts;
	}

	if(_alpha > 1.0)
	{
		// Normalização
		pmedio /= pmax;

		// Media
		pmedio /= _populacao;

		double F,R;

		for(i = 0; i < _populacao; i++)
		{
			// Pontos normalizado:
			pts = m_populacao[i].getPontuacao()/pmax;

			if(pts >= pmedio)
			{
				R = pts-pmedio;
				F = pmedio + pow(R,_alpha);
				m_populacao[i].setPontos(F);	
			}
			else
			{
				R = pmedio-pts;
				F = pmedio - pow(R,_alpha);
				m_populacao[i].setPontos(F);	
			}
		}

		_alpha -= 0.05;
	}
	

	sort(m_populacao.begin(), m_populacao.end(),pred);

	m_carWinner = m_populacao[0];
	if(m_melhores.size() == 0)
	{
		m_melhores.push_back(melhor_t(_geracao,m_carWinner));
	}
	else
	{
		if(m_melhores[m_melhores.size()-1].second.getGenesCString() != m_carWinner.getGenesCString())
		{
			m_melhores.push_back(melhor_t(_geracao,m_carWinner));
		}
	}

	return;
}

void CGa::_1Select(void)
{
	size_t i;

	// A população já está ordenada.
	// Elitismo:
	for(i = 0; i < _elitismo; i++)
	{
		m_nova.push_back(m_populacao[i].getGenesCString());
	}

	// Alienismo:
	for(i = 0; i < _alienismo; i++)
	{
		m_nova.push_back((CCar()).getGenesCString());
	}

	// Inclusão arbitrária:
	if(!_strId2Include.IsEmpty())
	{
		m_nova.push_back(_strId2Include);
		_strId2Include.Empty();
	}

	return;
}

void CGa::_2Crossover(void)
{
	size_t i, nId1, nId2, nSize = m_populacao.size();
	double nStdev = nSize / 1.0;
	size_t nMaxIndex = nSize - 1;

	while(m_nova.size() < _populacao)
	{
		// Escolha dos pais:
		nId1 = (size_t)fabs(mrand.randNorm(0,nStdev));
		nId2 = (size_t)fabs(mrand.randNorm(0,nStdev));

		// Clamp no range permitido
		nId1 = __min(nMaxIndex,nId1);
		nId2 = __min(nMaxIndex,nId2);


		if(nId1 == nId2)
			continue;
		
		// Faz crossover?
		if(mrand.randInt(100) < _crossover)
		{
			// Crossover:
			// Escolhemos um ponto aleatório para o crossover:
			size_t nCross;
			nCross = 1 + mrand.randInt(GENES-2);

			char tmp;
			for(i = nCross; i < GENES; i++)
			{
				tmp = m_populacao[nId1].getGene(i);
				m_populacao[nId1].setGene(i,m_populacao[nId2].getGene(i));
				m_populacao[nId2].setGene(i,tmp);
			}
		}

		m_nova.push_back(m_populacao[nId1].getGenesCString());
		m_nova.push_back(m_populacao[nId2].getGenesCString());
	}

	return;
}

void CGa::_3Mutate(void)
{
	int nMut;
	char g;
	size_t i,nSize = m_nova.size();
	for(i = (_elitismo + _alienismo) ; i < nSize; i++)
	{
		if(mrand.rand(100.0) < _mutacao)
		{
			// Mutação:
			// Ponto da mutação:
			nMut = mrand.randInt(GENES-1);

			// Intensidade e direção da mutação:
			g = (char)(m_nova[i].GetAt(nMut) + mrand.randInt((MTRand::uint32)_mut_int) * (mrand.randInt(1)?(1):(-1)));

			if(g < 'A') g = 'A';
			if(g > 'Z') g = 'Z';

			m_nova[i].SetAt(nMut,g);
		}
	}
}

void CGa::_4AdvanceGeneration(void)
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
		size_t i,nSize = m_nova.size();
		for(i = 0; i < nSize && i < _populacao; i++)
		{
			m_populacao.push_back(CCar(m_nova[i]));
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

void CGa::IncludeId(CString strGenes)
{
	_strId2Include = strGenes;
}

void CGa::Step(void)
{
	_1Select();
	_2Crossover();
	_3Mutate();
	_4AdvanceGeneration();
}

void CGa::CopyPopulacao(vec_car_t *pTarget)
{
	*pTarget = m_populacao;

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
							"set xlabel \"Geração\";\n"
							"set ylabel \"Pontuação\";\n"
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


};//namespace GA