#pragma once

namespace MODEL
{
class CCarDef
{
public:
	class CCircleParams
	{
	public:
		float x;
		float y;
		float raio;
	};

	class CBodyParams
	{
	public:
		float densidade;
		float friccao;
		float elasticidade;
	};

	class CRodaParams
	{
	public:
		CCircleParams circle;
		CBodyParams	body;

		CRodaParams();
		CRodaParams(float x, float y, float r, float dens, float fric, float elas);
	};

	CRodaParams	_roda1		;
	CRodaParams	_roda2		;
	CRodaParams	_peso1		;
	CRodaParams	_peso2		;
	float	_torque[4]	;
	float	_freq  [6]	;
	float	_damp  [6]	;

public:
	CCarDef();
	~CCarDef();
};

}
