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
        float x;
        float y;

        vec2_t(const float ax, const float ay)
        {
            x = ax;
            y = ay;
        }
    };

    typedef vector<vec2_t> vec_vecs_t;

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

        float _tlx;
        float _tly;
        float _brx;
        float _bry;

    public:
        CEnv();

        void set(unsigned seed,
                 float dxm, float dxs, float dxo,
                 float dym, float dys, float dyo,
                 float phi, float omega, float a,
                 float tlx, float tly,
                 float brx, float bry);

        [[nodiscard]] string get() const;

        static void set(const string &sParams);

        [[nodiscard]] vec_vecs_t get_vecs() const;
    };
}; //namespace MODEL
