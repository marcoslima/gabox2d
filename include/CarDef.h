#pragma once

namespace MODEL
{
class CCarDef
{
public:
	using circle_params_t = struct tagCircleParams
	{
		float x;
		float y;
		float raio;
	};

	using body_params_t = struct tagBodyParams
	{
		float densidade;
		float friccao;
		float elasticidade;
	};

	class CRodaParams
	{
	public:
		circle_params_t circle;
		body_params_t	body;

		CRodaParams();
		CRodaParams(float x, float y, float r, float dens, float fric, float elas);
	};

	CRodaParams	_roda1;
	CRodaParams	_roda2;
	CRodaParams	_peso1;
	CRodaParams	_peso2;
	float	_torque[4];
	float	_freq  [6];
	float	_damp  [6];

	CCarDef();
	~CCarDef();

	bool operator==(const CCarDef &other) const;
};

}
