#include "env.h"
#include <ranges>
#include <sstream>
#include <cmath>
#include <iostream>
#include <random>

#include "CNormalSeededRandom.h"

using namespace std;

namespace MODEL
{
    CEnv::CEnv()
        : _seed(0)
          , _dxm(0), _dxs(1.0f), _dxo(5.0f)
          , _dym(0), _dys(0.3f), _dyo(0)
          , _phi(0), _omega(0), _a(0)
          , _tlx(-100), _tly(500)
          , _brx(500), _bry(-50) {}

    void CEnv::set(const unsigned seed,
                   const float dxm, const float dxs, const float dxo,
                   const float dym, const float dys, const float dyo,
                   const float phi, const float omega, const float a,
                   const float tlx, const float tly,
                   const float brx, const float bry)
    {
        _seed = static_cast<int>(seed);
        _dxm = dxm;
        _dxs = dxs;
        _dxo = dxo;
        _dym = dym;
        _dys = dys;
        _dyo = dyo;
        _phi = phi;
        _omega = omega;
        _a = a;
        _tlx = tlx;
        _tly = tly;
        _brx = brx;
        _bry = bry;
    }

    string CEnv::get() const
    {
        stringstream ss;
        ss << "environment_definition_" << ENV_HEADER << "{";

        ss << _seed << ",";
        ss << _dxm << ",";
        ss << _dxs << ",";
        ss << _dxo << ",";
        ss << _dym << ",";
        ss << _dys << ",";
        ss << _dyo << ",";
        ss << _phi << ",";
        ss << _omega << ",";
        ss << _a << ",";
        ss << _tlx << ",";
        ss << _tly << ",";
        ss << _brx << ",";
        ss << _bry << "}";
        return ss.str();
    }

    void CEnv::set(const string& sParams)
    {
#if 0
        RE re("environment_definition_(?P<ver>\\d+\\.\\d+){\\s*(?P<seed>\\d+),\\s*(?P<dxm>-*\\d*\\.*\\d*),\\s*(?P<dxs>-*\\d*\\.*\\d*),\\s*(?P<dxo>-*\\d*\\.*\\d*),\\s*(?P<dym>-*\\d*\\.*\\d*),\\s*(?P<dys>-*\\d*\\.*\\d*),\\s*(?P<dyo>-*\\d*\\.*\\d*),\\s*(?P<phi>-*\\d*\\.*\\d*),\\s*(?P<omega>-*\\d*\\.*\\d*),\\s*(?P<a>-*\\d*\\.*\\d*),\\s*(?P<tlx>-*\\d*\\.*\\d*),\\s*(?P<tly>-*\\d*\\.*\\d*),\\s*(?P<brx>-*\\d*\\.*\\d*),\\s*(?P<bry>-*\\d*\\.*\\d*)\\s*}");

        double dVer;
        re.FullMatch(StringPiece(sParams),
        	&dVer,
        	&_seed,
        	&_dxm,
        	&_dxs,
        	&_dxo,
        	&_dym	,
        	&_dys	,
        	&_dyo	,
        	&_phi	,
        	&_omega,
        	&_a	,
        	&_tlx,
        	&_tly,
        	&_brx,
        	&_bry);
#endif
    }

    vec_vecs_t CEnv::get_vecs() const
    {
        CNormalSeededRandom randNorm(_seed);

        vec_vecs_t cwvecs, ccwvecs;

        cwvecs.clear();

        cwvecs.emplace_back(_brx, _bry);
        cwvecs.emplace_back(_tlx, _bry);

        cwvecs.emplace_back(_tlx, 1);
        cwvecs.emplace_back(4, 1);

        float ldy = 0;
        float lm = 0;
        float x = 10.0f;
    	float y;
        while (x < _brx)
        {
            const float dx = randNorm.random(_dxm, _dxs) + _dxo;
            const float m = randNorm.random(_dym, _dys) + _dyo;
            const float dy = ldy + (dx * (lm + m));
            y = dy + _a * sin(_omega * x + _phi);

            cwvecs.emplace_back(x, y);

            x += fabs(dx);
            ldy = dy;
            lm = m;
        }
    	cwvecs.emplace_back(_brx, y);

    	return cwvecs;

    }
}
