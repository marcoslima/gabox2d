#include <ranges>
#include <sstream>
#include <cmath>
#include <env.h>
#include <CRandom.h>

using namespace std;

namespace MODEL
{
    CEnv::CEnv()
        : _seed(0)
          , _dxm(0), _dxs(1.0f), _dxo(5.0f)
          , _dym(0), _dys(0.3f), _dyo(0)
          , _phi(0), _omega(0), _a(0)
    {
	    env_data.tlx = -100;
	    env_data.tly = 500;
    	env_data.brx = 500;
    	env_data.bry = -50;
    	_update_env_data();
    }

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
        env_data.tlx = tlx;
        env_data.tly = tly;
        env_data.brx = brx;
        env_data.bry = bry;
    	_update_env_data();
    }

    string CEnv::get() const
    {
        return {"Not implemented yet"};
    }

    void CEnv::set(const string& sParams)
    {
    	// Not implemented yet
    }

    void CEnv::_update_env_data()
    {
        const CRandom random(_seed);

        vec_vecs_t cwvecs, ccwvecs;

        cwvecs.clear();

        cwvecs.emplace_back(env_data.brx, env_data.bry);
        cwvecs.emplace_back(env_data.tlx, env_data.bry);

        cwvecs.emplace_back(env_data.tlx, 1);
        cwvecs.emplace_back(4, 1);

        float ldy = 0;
        float lm = 0;
        float x = 10.0f;
    	float y;
        while (x < env_data.brx)
        {
            const float dx = random.normal_random(_dxm, _dxs) + _dxo;
            const float m = random.normal_random(_dym, _dys) + _dyo;
            const float dy = ldy + dx * (lm + m);
            y = dy + _a * sin(_omega * x + _phi);

            cwvecs.emplace_back(x, y);

            x += fabs(dx);
            ldy = dy;
            lm = m;
        }
    	cwvecs.emplace_back(env_data.brx, y);

    	// reverse copy
		env_data.ground.clear();
		for (auto & cwvec : std::ranges::reverse_view(cwvecs))
		{
			env_data.ground.push_back(cwvec);
		}
    }
}
