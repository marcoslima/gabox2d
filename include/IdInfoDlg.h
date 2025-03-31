#pragma once

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
public:
	void render() override;
	void set(unsigned nGeracao      ,
			 float   dPontuacao	    ,
			 float   dTempo		    ,
			 const string &strGenes	,
			 const string &strDeathReason);
};

}
