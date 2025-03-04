#pragma once
#if 0

#include "WndShowCarro.h"

namespace GUI
{
// CEvolucaoDlg dialog
class CEvolucaoDlg : public CDialog
{
	DECLARE_DYNAMIC(CEvolucaoDlg)

public:
	CEvolucaoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEvolucaoDlg();

// Dialog Data
	enum { IDD = IDD_EVOLUCAO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_cGeracao;
	CString m_strGeracao;
	CWndShowCarro m_cCarro;
	vec_melhores_t	m_vecCarros;

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CString m_strMelhorias;
};
};//namespace GUI

#endif