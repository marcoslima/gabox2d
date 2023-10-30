#pragma once

namespace MODEL
{
class CCarDef
{
public:
	class CCircle
	{
	public:
		double x;
		double y;
		double raio;
	};

	class CBody
	{
	public:
		double densidade;
		double friccao;
		double elasticidade;
	};

	class CRoda
	{
	public:
		CCircle c;
		CBody	b;
	public:
		CRoda();
		CRoda(double x, double y, double r, double dens, double fric, double elas);
	};

public:
	CRoda	_roda1		;
	CRoda	_roda2		;
	CRoda	_peso1		;
	CRoda	_peso2		;
	double	_torque[4]	;
	double	_freq  [6]	;
	double	_damp  [6]	;

public:
	CCarDef(void);
	~CCarDef(void);
};

};//namespace MODEL
