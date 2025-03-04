#if 0


// EvolucaoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GaBox2d.h"
#include "EvolucaoDlg.h"
#include ".\evolucaodlg.h"

namespace GUI
{
// CEvolucaoDlg dialog

IMPLEMENT_DYNAMIC(CEvolucaoDlg, CDialog)
CEvolucaoDlg::CEvolucaoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEvolucaoDlg::IDD, pParent)
	, m_strGeracao(_T(""))
	, m_strMelhorias(_T(""))
{
}

CEvolucaoDlg::~CEvolucaoDlg()
{
}

void CEvolucaoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SGERACAO, m_cGeracao);
	DDX_Text(pDX, IDC_GERACAO, m_strGeracao);
	DDX_Control(pDX, IDC_CARRO,m_cCarro);
	DDX_Text(pDX, IDC_MELHORIAS, m_strMelhorias);
}


BEGIN_MESSAGE_MAP(CEvolucaoDlg, CDialog)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CEvolucaoDlg message handlers

BOOL CEvolucaoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_cGeracao.SetRangeMin(0,TRUE);
	m_cGeracao.SetRangeMax(m_vecCarros.size()-1,TRUE);

	if(m_vecCarros.size() > 2)
	{
		m_strMelhorias.Format("%d melhorias (%.2f gera��es p/ melhoria)", m_vecCarros.size() - 1,
			m_vecCarros[m_vecCarros.size()-1].first / (double)(m_vecCarros.size()-1));
		UpdateData(FALSE);
	}
	return TRUE;
}

void CEvolucaoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(pScrollBar == (CScrollBar *)&m_cGeracao)
	{
		UpdateData();
		int nItem = __min(m_cGeracao.GetPos(), m_vecCarros.size()-1);

		m_strGeracao.Format("%d",m_vecCarros[nItem].first);
		m_cCarro.m_Carro = m_vecCarros[nItem].second;
		m_cCarro.Invalidate();

		UpdateData(FALSE);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
};//namespace GUI

#endif