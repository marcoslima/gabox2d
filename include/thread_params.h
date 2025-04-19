#pragma once
#include <atomic>
#include <ga_params.h>
#include <env.h>
using namespace std;

// Parâmetros para o thread:
class CThreadParams
{
public:
    atomic<bool> m_bStopGa;
    ga_params_t m_Params;
    MODEL::env_data_t m_env_data;
};
