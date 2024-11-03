#pragma once

namespace MODEL
{
class CCarDef
{
public:
	class CCircle
	{
	public:
		float x;
		float y;
		float raio;
	};

	class CBody
	{
	public:
		float densidade;
		float friccao;
		float elasticidade;
	};

	class CRoda
	{
	public:
		CCircle c;
		CBody	b;
	public:
		CRoda();
		CRoda(float x, float y, float r, float dens, float fric, float elas);
	};

public:
	CRoda	_roda1		;
	CRoda	_roda2		;
	CRoda	_peso1		;
	CRoda	_peso2		;
	float	_torque[4]	;
	float	_freq  [6]	;
	float	_damp  [6]	;

public:
	CCarDef();
	~CCarDef();
};

};//namespace MODEL
