#ifndef __GAINFODLG_H__
#define __GAINFODLG_H__

#include "ga.h"
using namespace GA;

namespace GUI
{

// Informa��es sobre o GA:
class CGaInfo
{
public:
	lst_car_t	m_populacao;
	unsigned	m_geracao;
	double		m_gps;		// Gera��es por segundo
	double		m_maxD;
	double		m_maxT;
	double		m_maxVm;
	CEnv		m_envdef;

	// Lista com o hist�rico dos melhores.
	// Ela s� � alimentada mediante um pedido em m_reqMelhores;
	bool			m_reqMelhores;
	vec_melhores_t	m_vecMelhores;

	// Requisi��o de extin��o em massa:
	bool	m_reqExtincao;

	// Requsi��o de inclus�o de indiv�duo:
	bool	m_reqIncludeId;
	string	m_strId2Include;

public:
	CGaInfo();
	~CGaInfo();

	void Lock(void);
	void Release(void);

private:
	CRITICAL_SECTION _cs;
};

// CGaInfoDlg dialog
class CGaBox2dView;
class CGaInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CGaInfoDlg)

public:
	CGaInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGaInfoDlg();
	CWnd*	m_pView;
// Dialog Data
	enum { IDD = IDD_GA_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_nGeracao;
	UINT m_nPopulacao;
	MfcUtil::ListCtrlEx::CListCtrlEx	m_lstGenes;
	virtual BOOL OnInitDialog();
	void Refresh(CGaInfo* pInfo);
	afx_msg void OnNMDblclkGenes(NMHDR *pNMHDR, LRESULT *pResult);
};

};//namespace GUI

#endif //__GAINFODLG_H__