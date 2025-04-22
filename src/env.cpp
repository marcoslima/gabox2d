#include <ranges>
#include <cmath>
#include <env.h>
#include <CRandom.h>
#include <env/env.pb.h>


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
    	const auto pb_env = make_unique<Env::v1::env>();
    	pb_env->set_seed(_seed);
    	pb_env->set_dxm(_dxm);
    	pb_env->set_dxs(_dxs);
    	pb_env->set_dxo(_dxo);
    	pb_env->set_dym(_dym);
    	pb_env->set_dys(_dys);
    	pb_env->set_dyo(_dyo);
    	pb_env->set_phi(_phi);
    	pb_env->set_omega(_omega);
    	pb_env->set_a(_a);
    	pb_env->mutable_data()->set_tlx(env_data.tlx);
    	pb_env->mutable_data()->set_tly(env_data.tly);
    	pb_env->mutable_data()->set_brx(env_data.brx);
    	pb_env->mutable_data()->set_bry(env_data.bry);
    	for (const auto &point : env_data.ground)
    	{
    		auto pb_point = make_unique<Env::v1::vec2f>();
    		pb_point->set_x(point.x);
    		pb_point->set_y(point.y);
    		pb_env->mutable_data()->mutable_ground()->AddAllocated(pb_point.release());
    	}

		string serialized_data;
		pb_env->SerializeToString(&serialized_data);
        return serialized_data;
    }

    void CEnv::set(const string& sParams)
    {
		Env::v1::env pb_env;
    	if (!pb_env.ParseFromString(sParams))
    	{
    		cout << "Failed to parse serialized environment" << endl;
    	}
    	_seed = pb_env.seed();
    	_dxm = pb_env.dxm();
    	_dxs = pb_env.dxs();
    	_dxo = pb_env.dxo();
    	_dym = pb_env.dym();
    	_dys = pb_env.dys();
    	_dyo = pb_env.dyo();
    	_phi = pb_env.phi();
    	_omega = pb_env.omega();
    	_a = pb_env.a();
    	env_data.tlx = pb_env.data().tlx();
    	env_data.tly = pb_env.data().tly();
    	env_data.brx = pb_env.data().brx();
    	env_data.bry = pb_env.data().bry();
    	env_data.ground.clear();
    	for (const auto &point : pb_env.data().ground())
		{
			vec2f_t thisPoint(point.x(), point.y());
			env_data.ground.emplace_back(thisPoint);
		}
		_update_env_data();
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
