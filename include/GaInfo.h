#pragma once
#include <string>

#include <ga.h>

namespace GUI
{
    // Informações sobre o GA:
    class CGaInfo
    {
        // CRITICAL_SECTION _cs;
    public:
        lst_car_t	m_populacao;
        unsigned	m_geracao;
        double		m_gps;		// Gerações por segundo
        double		m_maxD;
        double		m_maxT;
        double		m_maxVm;
        CEnv		m_envdef;

        // Lista com o histórico dos melhores.
        // Ela só é alimentada mediante um pedido em m_reqMelhores;
        bool			m_reqMelhores;
        GA::vec_melhores_t	m_vecMelhores;

        // Requisição de extinção em massa:
        bool	m_reqExtincao;

        // Requsição de inclusão de indivíduo:
        bool	m_reqIncludeId;
        string	m_strId2Include;

        CGaInfo();
        ~CGaInfo();

        void Lock(void);
        void Release(void);
    };
}