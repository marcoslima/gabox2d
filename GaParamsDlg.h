#pragma once


// CGaParamsDlg dialog

class CGaParamsDlg : public CDialog
{
	DECLARE_DYNAMIC(CGaParamsDlg)

public:
	CGaParamsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGaParamsDlg();

// Dialog Data
	enum { IDD = IDD_PARAMETROS_GA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_nPopulacao;
	CString m_strCrossover;
	UINT m_nElitismo;
	CString m_strMutacao;
	UINT m_nAlienismo;
	UINT m_nMutInt;
	double m_dMaxT;
};
