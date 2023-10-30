#include <stdafx.h>
#include "car.h"


CCar::CCar()
:	  CGaCar()
	, CPhysCar()
	, CGrCar()
{
}

CCar::CCar(const char* szGenes)
: CGaCar(szGenes)
{
}

void CCar::beginSimulate(b2World *pWorld)
{
	// Desinstanciamento
	_destroy();

	// Decodificamos os genes (genes -> carro | string -> CCarDef)
	_decode();

	// Instanciamento
	_create(pWorld,_carro);

	// Inicializamos a simulação física:
	_phys_begin_simulate();
}

void CCar::endSimulate(void)
{
	// Liberamos os recursos da phys:
	_phys_end_simulate();
}

void CCar::CreateCar(const char* szGenes)
{
	if(_pWorld && _pRoda1)
		_destroy();

	_init();
	CGaCar::CreateCar(szGenes);
}

void CCar::DestroyCar(void)
{
	_destroy();
}

bool CCar::doStep(void)
{
	bool bRet = _simulation_step();

	Phys2Gr();

	return bRet;
}

void TranslateCircle(b2Body* pRoda, GUI::CGrCar::circle_t& grCircle)
{
	b2CircleShape *circle = (b2CircleShape*)pRoda->GetShapeList();
	b2Vec2	pos  = pRoda->GetPosition() + circle->GetLocalPosition();

	grCircle.c = PointF(pos.x,pos.y);
	grCircle.r = circle->GetRadius();
}

void TranslateRoda(b2Body* pRoda, GUI::CGrCar::roda_t& grRoda, bool bContact)
{
	TranslateCircle(pRoda,grRoda.c);
	grRoda.angle = pRoda->GetAngle();
	grRoda.touch = bContact;
}

void TranslatePeso(b2Body* pPeso, GUI::CGrCar::peso_t& grPeso, bool bBroke)
{
	TranslateCircle(pPeso,grPeso.c);
	grPeso.broke = bBroke;
}

void CCar::Phys2Gr(void)
{
	TranslateRoda(_pRoda1,_roda1,m_bContactR1);
	TranslateRoda(_pRoda2,_roda2,m_bContactR2);
	TranslatePeso(_pPeso1,_peso1,m_bDead);
	TranslatePeso(_pPeso2,_peso2,m_bDead);
	b2Vec2 cm = getCenter();
	_cm = PointF(cm.x,cm.y);
	_broke = m_bDead;
}

void CCar::Medir(b2World* pWorld, double max_t)
{
	float32 x0,x;
	int k;
	
	beginSimulate(pWorld);
	x0 = getCenter().x;
	for(k = 0;  _t < max_t; k++)
	{
		if(!doStep())
			break;
	}
	x = getCenter().x;
	m_t = _t;
	Destroy();

	m_distancia = x - x0;
}