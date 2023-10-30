#ifndef __EDITORCHAODLG_H__
#define __EDITORCHAODLG_H__

#include "ga.h"
#include "WndPreviewChao.h"
#include "afxcmn.h"

namespace GUI
{
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
	CEnv	m_World;
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
	CSpinButtonCtrl m_cSpinPhi;
	CSpinButtonCtrl m_cSpinOmega;
	CSpinButtonCtrl m_cSpinA;
	CSpinButtonCtrl m_cSpinDxm;
	CSpinButtonCtrl m_cSpinDxs;
	CSpinButtonCtrl m_cSpinDxo;
	CSpinButtonCtrl m_cSpinDym;
	CSpinButtonCtrl m_cSpinDys;
	CSpinButtonCtrl m_cSpinDyo;

	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeSeed();
	BOOL m_bAutoUpdate;
	CSliderCtrl m_cSldVert;
	CSliderCtrl m_cSldHorz;
	afx_msg void OnEnChangeDxMedia();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedZoomIn();
	afx_msg void OnBnClickedZoomOut();
	afx_msg void OnBnClickedAutoUpdate();
	afx_msg void OnEnChangeSinPhi();
	afx_msg void OnEnChangeSinOmega();
	afx_msg void OnEnChangeSinA();
	afx_msg void OnEnChangeDxStdev();
	afx_msg void OnEnChangeDxOffset();
	afx_msg void OnEnChangeDyMedia();
	afx_msg void OnEnChangeDyStdev();
	afx_msg void OnEnChangeDyOffset();
	afx_msg void OnBnClickedOk();
};
};//namespace GUI
#endif //__EDITORCHAODLG_H__