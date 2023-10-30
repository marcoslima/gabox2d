#include <stdafx.h>
#include <algorithm>
using namespace std;
#include <mersenne.h>
#include <cronometro.h>
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
	_bLogOpenned = false;
}
CGa::~CGa()
{
	DeleteCriticalSection(&m_cs);
	if(_bLogOpenned)
		_fileLog1.Close();
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

	_fileLog1.Open("log1.txt",CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyWrite);
	_nCount = 0;
	_bLogOpenned = true;
}


bool pred(const CCar& left, const CCar& right)
{
   return left.getPontuacao() < right.getPontuacao();
}

void CGa::Ordena(b2World *pWorld, HANDLE hStop)
{

	double pts;

	// Ordenamos (medições)
	lst_car_t::iterator it;
	for(it = m_populacao.begin();
		it!= m_populacao.end();
		it++)
	{
		it->Medir(pWorld,_max_t);

		if(WaitForSingleObject(hStop,0) == WAIT_OBJECT_0)break;
	}

	// Ajustamos os pontos
	double c1,c2,v,d,t;
	double p1,p2,p3,p4,p5;
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

		pts = (p1*p1 + p2*p2 + p3*p3 + p4*p4 + p5*p5);
		
		// Se quebrou, vale um décimo de um que não quebrou:
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
	lst_car_t::iterator it;
	for(it = m_populacao.begin(),i = 0;
		it!= m_populacao.end() && i < _elitismo;
		it++,i++)
	{
		m_nova.push_back(CString("__")+it->getGenesCString());
	}

	// Alienismo:
	for(i = 0; i < _alienismo; i++)
	{
		m_nova.push_back(CString("__")+(CCar()).getGenesCString());
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
	size_t i,nId1, nId2, nSize = m_populacao.size();
	double nStdev = nSize / 1.0;
	size_t nMaxIndex = nSize - 1;
	CString str1, str2, strt;
	size_t nCross;
	lst_car_t::iterator it;

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
		it = m_populacao.begin();for(i = 0; i < nId1; i++,it++);
		str1 = it->getGenesCString();

		it = m_populacao.begin();for(i = 0; i < nId1; i++,it++);
		str2 = it->getGenesCString();

		if(mrand.randInt(100) < _crossover)
		{
			nCross = 1 + mrand.randInt(GENES-2);
			strt = str1.Left(nCross);
			str1 = str2.Left(nCross) + str1.Right(GENES-nCross);
			str2 = strt + str2.Right(GENES-nCross);
		}

		m_nova.push_back(str1);
		m_nova.push_back(str2);
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

			VERIFY(m_nova[i].Left(2) != "__");

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
			if(m_nova[i].Left(2) == "__")
				m_populacao.push_back(CCar(m_nova[i].Right(m_nova[i].GetLength()-2)));
			else
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
	CCronometro cr;

	double select,crossover,mutate,advance;
	cr.Start();
	_1Select();
	select =cr.Get();
	cr.Start();
	_2Crossover();
	crossover = cr.Get();
	cr.Start();
	_3Mutate();
	mutate = cr.Get();
	cr.Start();
	_4AdvanceGeneration();
	advance = cr.Get();

	double m,s;
	CString strLine,strPart;
	_vec_select.push_back(select);
	_vec_crossover.push_back(crossover);
	_vec_mutate.push_back(mutate);
	_vec_advance.push_back(advance);

	m = Media<double,double>(_vec_select);
	s = StdDev<double,double>(_vec_select);
	Significativos(m,s);

	strPart.Format("%f ± %f, ",m,s);
	strLine += strPart;

	m = Media<double,double>(_vec_crossover);
	s = StdDev<double,double>(_vec_crossover);
	Significativos(m,s);

	strPart.Format("%f ± %f, ",m,s);
	strLine += strPart;

	m = Media<double,double>(_vec_mutate);
	s = StdDev<double,double>(_vec_mutate);
	Significativos(m,s);

	strPart.Format("%f ± %f, ",m,s);
	strLine += strPart;

	m = Media<double,double>(_vec_advance);
	s = StdDev<double,double>(_vec_advance);
	Significativos(m,s);

	strPart.Format("%f ± %f\r\n",m,s);
	strLine += strPart;

	_fileLog1.Write(strLine,strLine.GetLength());
}

void CGa::CopyPopulacao(lst_car_t *pTarget)
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