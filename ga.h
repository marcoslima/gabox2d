#ifndef __GA_H__
#define __GA_H__

#include <string>
#include <sstream>
#include <pcrecpp.h>
#include "lmFisica.h"
using namespace pcrecpp;
using namespace std;
using namespace LmFisica;

#include "car.h"
#include "env.h"

typedef vector<double> vec_double_t;

namespace GA
{

float random(float aMin, float aMax);
typedef vector<CString> vec_cstr_t;
typedef pair<size_t,CCar> melhor_t;
typedef vector<melhor_t> vec_melhores_t;


class CGa
{
// Par�metros para o algoritmo gen�tico
private:
	double	_max_t		;	// Tempo m�ximo de simula��o
	size_t	_populacao	;	// N�mero de indiv�duos por gera��o
	size_t	_elitismo	;	// Quantos indiv�duos v�o para gera��o seguinte inalterados
	size_t	_alienismo	;	// N�mero de indiv�uos rand�micos inseridos � cada gera��o
	size_t	_mut_int	;	// A muta��o ser� +/- _mut_int numa letra
	double	_crossover	;	// Percentual de probabilidade de ocorrer crossover
	double	_mutacao	;	// Percentual de probabilidade de ocorrer muta��o
	CCar	m_carWinner ;	// Indiv�duo mais adaptado da gera��o atual (Objeto CCar)

// Acumuladores do algoritmo
	size_t  _geracao	 ;	// Gera��o atual

	// Sinaliza��o de extin��o em massa:
	bool	_bMassExtintion;

	// Individuo para ser incluido na pr�xima gera��o:
	CString	_strId2Include;

	// Controle de acesso � estruturas internas da classe
	CRITICAL_SECTION	m_cs;

// Armazenamento dos indiv�duos
private:
	// Popula��o (objetos CCar)
	lst_car_t	m_populacao;

	// Nova popula��o (sequ�ncias de genes)
	vec_cstr_t	m_nova;

	vec_double_t	_vec_select,
					_vec_crossover,
					_vec_mutate,
					_vec_advance;
	CFile _fileLog1;
	size_t _nCount;
	bool _bLogOpenned;
public:
	// Hist�rico de melhoramentos
	vec_melhores_t	m_melhores;

// Fun��es internas
private:
	void _cria_populacao(void);
	void _1Select(void);
	void _2Crossover(void);
	void _3Mutate(void);
	void _4AdvanceGeneration(void);

// Interface
public:
	// Construtor/destrutor
	CGa();
	~CGa();

	// Especifica��o dos par�metros do GA
	void setParams(	size_t	nPopulacao	= 90 , 
					size_t	nElitismo	=  1 , 
					double	crossover	= 70 , 
					double	mutacao		= 65 ,
					size_t	nAlienismo	=  0 ,
					size_t	nMutInt		=  5 ,
					double	dMax_t		= 60 );

	// Inicia o algoritmo
	void BeginEvolve(void);

	// Testa e ordena os indiv�duos
	void Ordena(b2World *pWorld, HANDLE hStop);

	// Seleciona, cruza, muta e passa para gera��o seguinte
	void Step(void);

	// Comanda a extin��o em massa para a pr�xima gera��o
	void MassExtinctionEvent(void);

	// Inclui um indiv�duo na pr�xima gera��o
	void IncludeId(CString strGenes);

	// Copia a popula��o para um vector
	void	CopyPopulacao(lst_car_t *pTarget);
	
	// Queries
	CCar	getWinner()			{return m_carWinner		;}
	size_t	getPopulacaoLen()	{return _populacao		;}
	size_t  getGeracao()		{return _geracao		;}
	bool OpenLogFile(void);
};

};//namespace GA

#endif //__GA_H__