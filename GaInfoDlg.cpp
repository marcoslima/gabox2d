// GaInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GaBox2d.h"
#include "GaInfoDlg.h"
#include ".\gainfodlg.h"
namespace GUI
{
// CGaInfoDlg dialog
IMPLEMENT_DYNAMIC(CGaInfoDlg, CDialog)
CGaInfoDlg::CGaInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGaInfoDlg::IDD, pParent)
	, m_nGeracao(0)
	, m_nPopulacao(0)
{
}

CGaInfoDlg::~CGaInfoDlg()
{
}

void CGaInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GERACAO, m_nGeracao);
	DDX_Text(pDX, IDC_POPULACAO, m_nPopulacao);
	DDX_Control(pDX,IDC_GENES,m_lstGenes);
}


BEGIN_MESSAGE_MAP(CGaInfoDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_GENES, OnNMDblclkGenes)
END_MESSAGE_MAP()


// CGaInfoDlg message handlers
BOOL CGaInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstGenes.InsertColumn(0,"Pts");
	m_lstGenes.InsertColumn(1,"Genes");
	m_lstGenes.AdjustColumns();

	return TRUE;
}



CGaInfo::CGaInfo()
{
	InitializeCriticalSection(&_cs);
	m_reqExtincao = false;
	m_reqMelhores = false;
	m_reqIncludeId = false;
}
CGaInfo::~CGaInfo()
{
	DeleteCriticalSection(&_cs);
}
void CGaInfo::Lock(void)
{
	EnterCriticalSection(&_cs);
}
void CGaInfo::Release(void)
{
	LeaveCriticalSection(&_cs);
}

void CGaInfoDlg::Refresh(CGaInfo* pInfo)
{
	UpdateData();
	m_nGeracao = pInfo->m_geracao;
	m_nPopulacao = (UINT)pInfo->m_populacao.size();

	m_lstGenes.SetRedraw(FALSE);
	m_lstGenes.DeleteAllItems();

	int iItem;
	lst_car_t::iterator it;
	for(it = pInfo->m_populacao.begin();
		it!= pInfo->m_populacao.end();
		it++)
	{
		iItem = m_lstGenes.InsertItem();
		m_lstGenes.SetItemData(iItem,0,(int)it->_pontos);
		m_lstGenes.SetItemData(iItem,1,it->_genes.c_str() );
	}
	m_lstGenes.AdjustColumns();
	m_lstGenes.SetRedraw(TRUE);

	UpdateData(FALSE);
}
void CGaInfoDlg::OnNMDblclkGenes(NMHDR *pn, LRESULT *pResult)
{
	*pResult = 0;
	NMITEMACTIVATE *pni = (NMITEMACTIVATE*)pn;

	if(pni->iItem == -1)
		return;
	int iItem = pni->iItem;

	CString strGenes = m_lstGenes.GetItemData(iItem,1).GetString();
	char* szGenes = new char[strGenes.GetLength()+5];
	strcpy(szGenes,strGenes);

	m_pView->SendMessage(IDM_SIMULAR_GENE,0,(LPARAM)szGenes);

	delete szGenes;
}

};//namespace GUI