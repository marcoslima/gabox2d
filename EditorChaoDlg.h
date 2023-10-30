#pragma once
#include "ga.h"
#include "WndPreviewChao.h"
#include "afxcmn.h"

// CEditorChaoDlg dialog

class CEditorChaoDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditorChaoDlg)

public:
	vec_vecs_t m_vecVecs;
	CWndPreviewChao	m_wndPreview;

public:
	CEditorChaoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditorChaoDlg();

// Dialog Data
	enum { IDD = IDD_EDITOR_CHAO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDxMedia;
	CString m_strDxStdev;
	CString m_strDxOffset;
	CString m_strDyMedia;
	CString m_strDyStdev;
	CString m_strDyOffset;
	CString m_strSinPhi;
	CString m_strSinOmega;
	CString m_strSinA;
	afx_msg void OnBnClickedCriar();
	afx_msg void OnBnClickedSalvar();
	afx_msg void OnBnClickedLer();
	CString m_strTlx;
	CString m_strTly;
	CString m_strBrx;
	CString m_strBry;
	UINT m_nSeed;
	CSpinButtonCtrl m_cSpinSeed;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeSeed();
	BOOL m_bAutoUpdate;
	CSliderCtrl m_cSldVert;
	CSliderCtrl m_cSldHorz;
	afx_msg void OnEnChangeDxMedia();
};
