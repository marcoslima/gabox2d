#pragma once
#include <string>
using namespace std;

// CIdInfoDlg dialog
namespace GUI
{

class CIdInfoDlg
{
	public:
	// CIdInfoDlg(){};
	virtual ~CIdInfoDlg();

public:
	virtual bool OnInitDialog();
	unsigned m_nGeracao;
	string m_strPontuacao;
	string m_strTempo;

	void set(unsigned nGeracao	, 
			 double   dPontuacao, 
			 double   dTempo	,
			 string	  strGenes	);
	string m_strGenes;
};

};//namespace GUI
