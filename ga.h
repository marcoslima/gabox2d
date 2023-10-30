#ifndef __GA_H__
#define __GA_H__


#include "car.h"

typedef vector<b2Vec2> vec_vecs_t;

float random(float aMin, float aMax);
vec_vecs_t CreateGround(void);
b2World *CreateWorld(vec_vecs_t ground);
typedef vector<CString> vec_cstr_t;

class CGa
{
public:
	double	_max_t		;
	int		_populacao	;
	int		_elitismo	;
	int		_alienismo	;
	int		_mut_int	;
	double	_crossover	;
	double	_mutacao	;
	CString	m_strWinner	;
	CCar	m_carWinner ;

	double	_maxDistancia;
	double	_maxVm		 ;
	double	_maxT		 ;

	// Sinalização de extinção em massa:
	bool	_bMassExtintion;

	// Individuo para ser incluido na próxima geração:
	CString	_strId2Include;

private:
	void _cria_populacao(int nCount);

public:
	vec_car_t	m_populacao;
	vec_cstr_t	m_pais;
	vec_cstr_t	m_nova;

public:
	void setParams(	int		nPopulacao	= 90 , 
					int		nElitismo	=  1 , 
					double	crossover	= 70 , 
					double	mutacao		= 65 ,
					int		nAlienismo	=  0 ,
					int		nMutInt		=  5 ,
					double	dMax_t		= 60);
	void BeginEvolve(void);

	void Ordena(b2World *pWorld);
	void Select(void);
	void Crossover(void);
	void Mutate(void);
	void AdvanceGeneration(void);

	void MassExtintionEvent(void);
	void IncludeId(CString strGenes);
};







#endif //__GA_H__