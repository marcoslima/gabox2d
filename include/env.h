#pragma once

#include <vector>
#include <string>
#include <vec2f_t.h>

using namespace std;

#define ENV_HEADER "1.0"

namespace MODEL
{
    typedef vector<vec2f_t> vec_vecs_t;
    struct env_data_t
    {
        float tlx;
        float tly;
        float brx;
        float bry;
        vec_vecs_t ground;
    };

    // Classe que guarda as características do ambiente
    class CEnv
    {
    public:
        // Parâmetros de criação:
        int _seed;
        float _dxm;
        float _dxs;
        float _dxo;
        float _dym;
        float _dys;
        float _dyo;

        float _phi;
        float _omega;
        float _a;

        // Dados do ambiente (inclusive ground, já calculado)
        env_data_t env_data;

        CEnv();

        void set(unsigned seed,
                 float dxm, float dxs, float dxo,
                 float dym, float dys, float dyo,
                 float phi, float omega, float a,
                 float tlx, float tly,
                 float brx, float bry);

        [[nodiscard]] string get() const;
        static void set(const string &sParams);
        void _update_env_data();
    };
}