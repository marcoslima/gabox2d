// IdInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GaBox2d.h"
#include "IdInfoDlg.h"
#include ".\idinfodlg.h"
#include <devutils.h>
using namespace DevUtils;

namespace GUI
{

// CIdInfoDlg dialog

IMPLEMENT_DYNAMIC(CIdInfoDlg, CDialog)
CIdInfoDlg::CIdInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIdInfoDlg::IDD, pParent)
	, m_nGeracao(0)
	, m_strPontuacao(_T(""))
	, m_strTempo(_T(""))
	, m_strGenes(_T(""))
{
}

CIdInfoDlg::~CIdInfoDlg()
{
}

void CIdInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GERACAO, m_nGeracao);
	DDX_Text(pDX, IDC_PONTUACAO, m_strPontuacao);
	DDX_Text(pDX, IDC_TEMPO, m_strTempo);
	DDX_Text(pDX, IDC_IDATUAL, m_strGenes);
}


BEGIN_MESSAGE_MAP(CIdInfoDlg, CDialog)
END_MESSAGE_MAP()


// CIdInfoDlg message handlers
BOOL CIdInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_fnt.CreateFont(-10,0,0,0,0,0,0,0,0,0,0,0,0,"Courier New");
	SetFont(&m_fnt,TRUE);

	return TRUE;
}

void CIdInfoDlg::set(UINT nGeracao		, 
					double dPontuacao	, 
					double dTempo		,
					string strGenes		)
{
	if(nGeracao != -1)
		m_nGeracao = nGeracao;
	if(dPontuacao != -1)
		m_strPontuacao = dbl2str(dPontuacao,"%.0f");

	m_strTempo = dbl2str(dTempo,"%0.02f");
	m_strGenes = strGenes.c_str();
	UpdateData(FALSE);
}

};//namespace GUI