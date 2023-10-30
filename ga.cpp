#include <stdafx.h>
#include <algorithm>
using namespace std;
#include <mersenne.h>

#include "ga.h"

MTRand mrand;

float random(float aMin, float aMax)
{
	return aMin + mrand.rand(aMax-aMin);
}

vec_vecs_t CreateGround(void)
{
	vec_vecs_t ret;
	ret.push_back(b2Vec2(-100,1));
	ret.push_back(b2Vec2(4,1));

	double dx,dy,ldy = 0;
	double lm = 1,m;
	for(double i = 10.0f; i < 500;i+=0)
	{
		dx = mrand.randNorm(0,5.0/3.0)+2.5+1.3;
//		dx = random(1.3,6.0);

		m = mrand.randNorm(0.0,1.0/3.0);
//		m = random(-.5,.5);
		dy = ldy + (dx * (lm + m));// + (0.0001*i*i);
		
		ret.push_back(b2Vec2(i,dy+sin((i-10)*.02)*9));

		i += dx;
		ldy = dy;
		lm = m;
	}

	return ret;
}

b2World *CreateWorld(vec_vecs_t ground)
{
	b2World *pWorld;

	b2AABB ab2AABB;
	ab2AABB.lowerBound.Set(-100.0f, -100.0f);
	ab2AABB.upperBound.Set(500.0f, 1500.0f);

	b2Vec2 gravity(0.0f, -10.0f);
	bool doSleep = true;
	pWorld = new b2World(ab2AABB,gravity,doSleep);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 0.0f);	

	b2Body *pGround = pWorld->CreateBody(&groundBodyDef);

	b2PolygonDef groundShapeDef;
	groundShapeDef.SetAsBox(30.0f, 1.0f);
	groundShapeDef.friction = 1.0;
	groundShapeDef.restitution = 0.0;

	groundShapeDef.vertexCount = 4;

	size_t k,nSize = ground.size();
	b2Vec2 pt;
	b2Vec2 lpt = ground[0];
	for(k = 1; k < nSize; k++)
	{
		pt = ground[k];
		groundShapeDef.vertices[0] = lpt;
		groundShapeDef.vertices[1].Set(lpt.x,-100);
		groundShapeDef.vertices[2].Set(pt.x,-100);
		groundShapeDef.vertices[3] = pt;

		pGround->CreateShape(&groundShapeDef);

		lpt = pt;
	}

	// Limitadores dos boundary's:
	groundShapeDef.vertices[0].Set(500,-100);
	groundShapeDef.vertices[1].Set(500, 100);
	groundShapeDef.vertices[2].Set(498, 100);
	groundShapeDef.vertices[3].Set(498,-100);
	pGround->CreateShape(&groundShapeDef);

	groundShapeDef.vertices[0].Set( 500, 100);
	groundShapeDef.vertices[1].Set(-100, 100);
	groundShapeDef.vertices[2].Set(-100,  98);
	groundShapeDef.vertices[3].Set( 500,  98);
	pGround->CreateShape(&groundShapeDef);

	groundShapeDef.vertices[0].Set(-100, 100);
	groundShapeDef.vertices[1].Set(-100,-100);
	groundShapeDef.vertices[2].Set(- 50,-100);
	groundShapeDef.vertices[3].Set(- 50, 100);
	pGround->CreateShape(&groundShapeDef);





	return pWorld;
}


void CGa::_cria_populacao(int nCount)
{
	m_populacao.clear();

	for(int i = 0; i < nCount; i++)
		m_populacao.push_back(CCar());
}

void CGa::setParams(	int		nPopulacao	, 
						int		nElitismo	, 
						double	crossover	, 
						double	mutacao		,
						int		nAlienismo	,
						int		nMutInt		,
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
	_cria_populacao(_populacao);

	_maxDistancia = 0;
	_maxVm		  = 0;
	_maxT		  = 0;
	_bMassExtintion = false;
	_strId2Include.Empty();
}


bool pred( CCar left, CCar right)
{
   return left.getPontuacao() > right.getPontuacao();
}


void CGa::Ordena(b2World *pWorld)
{
	int i;
	double pts;

	// Ordenamos (medições)
	for(i = 0; i < _populacao; i++)
	{
		m_populacao[i].Medir(pWorld,_max_t);
		_maxVm			= __max(_maxVm			,m_populacao[i].m_vm		);
		_maxDistancia	= __max(_maxDistancia	,m_populacao[i].m_distancia );
		_maxT			= __max(_maxT			,m_populacao[i]._t			);
	}

	// Ajustamos os pontos
	double c1,c2,v,d,t;
	for(i = 0; i < _populacao; i++)
	{
		c1 = m_populacao[i].m_contatoR1					;
		c2 = m_populacao[i].m_contatoR2					;
		v  = m_populacao[i].m_vm		 / _maxVm		;
		d  = m_populacao[i].m_distancia  / _maxDistancia;
		t  = m_populacao[i]._t			 / _maxT		; 

		pts =10000* v*v*d*d*c1*c1*c2*c2*t*t;

		m_populacao[i].setPontos(pts);
	}

	sort(m_populacao.begin(), m_populacao.end(),pred);
	m_carWinner = m_populacao[0];

	return;
}

void CGa::Select(void)
{
	size_t i;
	CString strGens;

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

	// Select:
	int nHalf = _populacao/2;
	for(i = 0; i < nHalf; i++)
	{
		m_pais.push_back(m_populacao[i].getGenesCString());
	}
	return;
}

void CGa::Crossover(void)
{
	size_t i, nId1, nId2, nSize = m_pais.size();

	while(m_nova.size() < _populacao)
	{
		nId1 = mrand.randInt(nSize-1);
		nId2 = mrand.randInt(nSize-1);
		if(nId1 == nId2)
			continue;
		
		// Faz crossover?
		if(mrand.randInt(100) < _crossover)
		{
			// Crossover:
			// Escolhemos um ponto aleatório para o crossover:
			int nCross;
			nCross = 1 + mrand.randInt(GENES-2);

			char tmp;
			for(i = nCross; i < GENES; i++)
			{
				tmp = m_pais[nId1].GetAt(i);
				m_pais[nId1].SetAt(i,m_pais[nId2].GetAt(i));
				m_pais[nId2].SetAt(i,tmp);
			}
		}

		m_nova.push_back(m_pais[nId1]);
		m_nova.push_back(m_pais[nId2]);
	}

	m_pais.clear();
	return;
}

void CGa::Mutate(void)
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
			g = m_nova[i].GetAt(nMut) + mrand.randInt(_mut_int) * (mrand.randInt(1)?(1):(-1));

			if(g < 'A') g = 'A';
			if(g > 'Z') g = 'Z';

			m_nova[i].SetAt(nMut,g);
		}
	}
}

void CGa::AdvanceGeneration(void)
{
	// Se for para fazer extinção em massa, criamos aleatórios no lugar:
	if(_bMassExtintion)
	{
		_cria_populacao(_populacao);
		_bMassExtintion = false;
	}
	else
	{
		// Nova geração:
		m_populacao.clear();


		
		size_t i,nSize = m_nova.size();
		for(i = 0; i < nSize; i++)
		{
			m_populacao.push_back(CCar(m_nova[i]));
		}
	}

	m_nova.clear();
	return;
}

void CGa::MassExtintionEvent(void)
{
	_bMassExtintion = true;	
}

void CGa::IncludeId(CString strGenes)
{
	_strId2Include = strGenes;
}
