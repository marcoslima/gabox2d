#ifndef __GAPARAMSDLG_H__
#define __GAPARAMSDLG_H__

namespace GUI
{
// CGaParamsDlg dialog
class ga_params_t
{
public:
	UINT	m_nPopulacao	;
	CString m_strCrossover	;
	UINT	m_nElitismo		;
	CString m_strMutacao	;
	UINT	m_nAlienismo	;
	UINT	m_nMutInt		;
	double	m_dMaxT			;

public:
	ga_params_t(
		UINT	nPopulacao	,
		CString strCrossover	,
		UINT	nElitismo		,
		CString strMutacao	,
		UINT	nAlienismo	,
		UINT	nMutInt		,
		double	dMaxT			)
	{
		m_nPopulacao	= nPopulacao	;
		m_strCrossover	= strCrossover	;
		m_nElitismo		= nElitismo		;
		m_strMutacao	= strMutacao	;
		m_nAlienismo	= nAlienismo	;
		m_nMutInt		= nMutInt		;
		m_dMaxT			= dMaxT			;
	}

	ga_params_t(){}
};

class CGaParamsDlg : public CDialog, public ga_params_t
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
};
};//namespace GUI
#endif //__GAPARAMSDLG_H__