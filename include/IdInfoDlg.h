#pragma once

#include <GaCar.h>
#include <genoma_image.h>
#include <IPanel.h>
#include <string>


using namespace std;


namespace GUI
{
    class CIdInfoDlg final : public IPanel
    {
        string m_strGeracao;
        string m_strPontuacao;
        string m_strTempo;
        string m_strGenes;
        string m_strDeathReason;
        CGenomaImage _currentGenoma{static_cast<unsigned int>(CCarDef::bits::len())};

    public:
        void render() override;

        void set(unsigned nGeracao,
                 float dPontuacao,
                 float dTempo,
                 const string &strGenes,
                 const string &strDeathReason);
    };
}
