#include "StdAfx.h"
#include ".\cardef.h"
namespace MODEL
{
CCarDef::CCarDef(void)
{
}

CCarDef::~CCarDef(void)
{
}

CCarDef::CRoda::CRoda()
{
	c.x		= 0.0;
	c.y		= 0.0;
	c.raio	= 1.0;
	b.densidade		= 1.0;
	b.elasticidade	= 1.0;
	b.friccao		= 1.0;
}

CCarDef::CRoda::CRoda(double x, double y, double r, double dens, double fric, double elas)
{
	c.x		= x;
	c.y		= y;
	c.raio	= r;
	b.densidade		= dens;
	b.elasticidade	= elas;
	b.friccao		= fric;
}

};//namespace MODEL