#include "env.h"
#include <sstream>
#include <cmath>
#include <random>

#include "CNormalSeededRandom.h"

using namespace std;

namespace MODEL
{
    void CEnv::set(const double seed,
                   const double dxm, const double dxs, const double dxo,
                   const double dym, const double dys, const double dyo,
                   const double phi, const double omega, const double a,
                   const double tlx, const double tly,
                   const double brx, const double bry)
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

    string CEnv::get()
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

    vec_vecs_t CEnv::get_vecs()
    {
        CNormalSeededRandom randNorm(_seed);

        vec_vecs_t GroundPoly;

        GroundPoly.clear();

        GroundPoly.emplace_back(_brx, _bry);
        GroundPoly.emplace_back(_tlx, _bry);

        GroundPoly.emplace_back(_tlx, 1);
        GroundPoly.emplace_back(4, 1);

        double ldy = 0;
        double lm = 0;
        double x = 10.0f;
        while(x < _brx)
        {
            const double dx = randNorm.random(_dxm, _dxs) + _dxo;
            const double m = randNorm.random(_dym, _dys) + _dyo;
            const double dy = ldy + (dx * (lm + m));
            const double y = dy + _a * sin(_omega * x + _phi);
            GroundPoly.emplace_back(x, y);

            x += fabs(dx);
            ldy = dy;
            lm = m;
        }

        return GroundPoly;
    }
}; //namespace MODEL
