#include "env.h"
#include <sstream>
#include <cmath>

// extern MTRand mrand;

using namespace std;

namespace MODEL
{

void CEnv::set(double seed,
			double dxm, double dxs, double dxo,
			double dym, double dys, double dyo,
			double phi, double omega, double a,
			double tlx, double tly,
			double brx, double bry)
{
	_seed	= (int)seed	;
	_dxm	= dxm	;
	_dxs	= dxs	;
	_dxo	= dxo	;
	_dym	= dym	;
	_dys	= dys	;
	_dyo	= dyo	;
	_phi	= phi	;
	_omega	= omega	;
	_a		= a		;
	_tlx	= tlx	;
	_tly	= tly	;
	_brx	= brx	;
	_bry	= bry	;
}

string	CEnv::get(void)
{
	stringstream ss;
	ss << "environment_definition_" << ENV_HEADER << "{";

	ss << _seed	<< ",";
	ss << _dxm	<< ",";
	ss << _dxs	<< ",";
	ss << _dxo	<< ",";
	ss << _dym	<< ",";
	ss << _dys	<< ",";
	ss << _dyo	<< ",";
	ss << _phi	<< ",";
	ss << _omega<< ",";
	ss << _a	<< ",";
	ss << _tlx	<< ",";
	ss << _tly	<< ",";
	ss << _brx	<< ",";
	ss << _bry	<< "}";
	return ss.str();
}

void CEnv::set(string sParams)
{
	// RE re("environment_definition_(?P<ver>\\d+\\.\\d+){\\s*(?P<seed>\\d+),\\s*(?P<dxm>-*\\d*\\.*\\d*),\\s*(?P<dxs>-*\\d*\\.*\\d*),\\s*(?P<dxo>-*\\d*\\.*\\d*),\\s*(?P<dym>-*\\d*\\.*\\d*),\\s*(?P<dys>-*\\d*\\.*\\d*),\\s*(?P<dyo>-*\\d*\\.*\\d*),\\s*(?P<phi>-*\\d*\\.*\\d*),\\s*(?P<omega>-*\\d*\\.*\\d*),\\s*(?P<a>-*\\d*\\.*\\d*),\\s*(?P<tlx>-*\\d*\\.*\\d*),\\s*(?P<tly>-*\\d*\\.*\\d*),\\s*(?P<brx>-*\\d*\\.*\\d*),\\s*(?P<bry>-*\\d*\\.*\\d*)\\s*}");

	double dVer;
	// re.FullMatch(StringPiece(sParams),
	// 	&dVer,
	// 	&_seed,
	// 	&_dxm,
	// 	&_dxs,
	// 	&_dxo,
	// 	&_dym	,
	// 	&_dys	,
	// 	&_dyo	,
	// 	&_phi	,
	// 	&_omega,
	// 	&_a	,
	// 	&_tlx,	
	// 	&_tly,	
	// 	&_brx,	
	// 	&_bry);
}

vec_vecs_t CEnv::get_vecs(void)
{
	vec_vecs_t	GroundPoly;

	GroundPoly.clear();

	GroundPoly.push_back(vec2_t(_brx,_bry));
	GroundPoly.push_back(vec2_t(_tlx,_bry));

	GroundPoly.push_back(vec2_t(_tlx,1));
	GroundPoly.push_back(vec2_t(4,1));
	
	double dx,dy,ldy = 0;
	double m,lm = 0;
	// mrand.seed(_seed);
	for(double i = 10.0f; i < _brx;i+=0)
	{
		// dx = mrand.randNorm(_dxm,_dxs)+_dxo;
		// m  = mrand.randNorm(_dym,_dys)+_dyo;
		dy = ldy + (dx * (lm + m));
		
		GroundPoly.push_back(vec2_t(i,dy + _a*sin(_omega * i + _phi)));

		i += fabs(dx);
		ldy = dy;
		lm = m;
	}

	return GroundPoly;
}

}; //namespace MODEL

