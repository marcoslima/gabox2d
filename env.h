#pragma once

#include <vector>
#include <string>
using namespace std;

#define ENV_HEADER "1.0"

namespace MODEL
{

class vec2_t
{
public:
	double x;
	double y;
	vec2_t(const double ax, const double ay){x = ax; y = ay;}
};

typedef vector<vec2_t> vec_vecs_t;
// Classe que guarda as características do ambiente
class CEnv
{
public:
	// Parâmetros de criação:
	int		_seed;
	double	_dxm;
	double	_dxs;
	double	_dxo;
	double	_dym;
	double	_dys;
	double	_dyo;

	double	_phi;
	double	_omega;
	double	_a;

	float	_tlx;
	float	_tly;
	float	_brx;
	float	_bry;

public:
	void	set(double seed,
				double dxm, double dxs, double dxo,
				double dym, double dys, double dyo,
				double phi, double omega, double a,
				double tlx, double tly,
				double brx, double bry);
	string	get(void);

	static void	set(const string& sParams);
	vec_vecs_t get_vecs(void);

};

};//namespace MODEL
