// IdInfoDlg.cpp : implementation file
//

#include "GaBox2d.h"
#include "IdInfoDlg.h"
#include "devutils.h"

namespace GUI
{

// CIdInfoDlg dialog
CIdInfoDlg::CIdInfoDlg()
	: m_nGeracao(0)
	, m_strPontuacao("")
	, m_strTempo(_T(""))
	, m_strGenes(_T(""))
{
}

CIdInfoDlg::~CIdInfoDlg()
{
}

// void CIdInfoDlg::DoDataExchange(CDataExchange* pDX)
// {
// 	CDialog::DoDataExchange(pDX);
// 	DDX_Text(pDX, IDC_GERACAO, m_nGeracao);
// 	DDX_Text(pDX, IDC_PONTUACAO, m_strPontuacao);
// 	DDX_Text(pDX, IDC_TEMPO, m_strTempo);
// 	DDX_Text(pDX, IDC_IDATUAL, m_strGenes);
// }


// CIdInfoDlg message handlers
bool CIdInfoDlg::OnInitDialog()
{
	// m_fnt.CreateFont(-10,0,0,0,0,0,0,0,0,0,0,0,0,"Courier New");
	// SetFont(&m_fnt,TRUE);
	return true;
}

void CIdInfoDlg::set(unsigned nGeracao		, 
					double 	  dPontuacao	, 
					double 	  dTempo		,
					string 	  strGenes		)
{
	if(nGeracao != -1)
		m_nGeracao = nGeracao;
	if(dPontuacao != -1)
	{
		m_strPontuacao = dbl2str(dPontuacao,"%.0f");
	}

	m_strTempo = dbl2str(dTempo,"%0.02f");
	m_strGenes = strGenes.c_str();
	// UpdateData(FALSE);
}

};//namespace GUI