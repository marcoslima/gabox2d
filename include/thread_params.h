#pragma once
#include <atomic>
#include <ga_params.h>
#include <env.h>
using namespace std;

// Parâmetros para o thread:
class CThreadParams
{
public:
    atomic<bool>	m_bStopGa	;
    ga_params_t		m_Params	;
    MODEL::CEnv			m_env		;

    // ga_params_t		m_Params	;
    // CGaInfo*		m_pGaInfo	;
};
