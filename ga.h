#ifndef __GA_H__
#define __GA_H__

#include <string>
#include <sstream>
#include <pcrecpp.h>
#include <lmFisica.h>
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
// Parâmetros para o algoritmo genético
private:
	double	_max_t		;	// Tempo máximo de simulação
	size_t	_populacao	;	// Número de indivíduos por geração
	size_t	_elitismo	;	// Quantos indivíduos vão para geração seguinte inalterados
	size_t	_alienismo	;	// Número de indivíuos randômicos inseridos à cada geração
	size_t	_mut_int	;	// A mutação será +/- _mut_int numa letra
	double	_crossover	;	// Percentual de probabilidade de ocorrer crossover
	double	_mutacao	;	// Percentual de probabilidade de ocorrer mutação
	CCar	m_carWinner ;	// Indivíduo mais adaptado da geração atual (Objeto CCar)

// Acumuladores do algoritmo
	size_t  _geracao	 ;	// Geração atual

	// Sinalização de extinção em massa:
	bool	_bMassExtintion;

	// Individuo para ser incluido na próxima geração:
	CString	_strId2Include;

	// Controle de acesso à estruturas internas da classe
	CRITICAL_SECTION	m_cs;

// Armazenamento dos indivíduos
private:
	// População (objetos CCar)
	lst_car_t	m_populacao;

	// Nova população (sequências de genes)
	vec_cstr_t	m_nova;

	vec_double_t	_vec_select,
					_vec_crossover,
					_vec_mutate,
					_vec_advance;
	CFile _fileLog1;
	size_t _nCount;
	bool _bLogOpenned;
public:
	// Histórico de melhoramentos
	vec_melhores_t	m_melhores;

// Funções internas
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

	// Especificação dos parãmetros do GA
	void setParams(	size_t	nPopulacao	= 90 , 
					size_t	nElitismo	=  1 , 
					double	crossover	= 70 , 
					double	mutacao		= 65 ,
					size_t	nAlienismo	=  0 ,
					size_t	nMutInt		=  5 ,
					double	dMax_t		= 60 );

	// Inicia o algoritmo
	void BeginEvolve(void);

	// Testa e ordena os indivíduos
	void Ordena(b2World *pWorld, HANDLE hStop);

	// Seleciona, cruza, muta e passa para geração seguinte
	void Step(void);

	// Comanda a extinção em massa para a próxima geração
	void MassExtinctionEvent(void);

	// Inclui um indivíduo na próxima geração
	void IncludeId(CString strGenes);

	// Copia a população para um vector
	void	CopyPopulacao(lst_car_t *pTarget);
	
	// Queries
	CCar	getWinner()			{return m_carWinner		;}
	size_t	getPopulacaoLen()	{return _populacao		;}
	size_t  getGeracao()		{return _geracao		;}
	bool OpenLogFile(void);
};

};//namespace GA

#endif //__GA_H__