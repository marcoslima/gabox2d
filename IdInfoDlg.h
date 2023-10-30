#ifndef __IDINFODLG_H__
#define __IDINFODLG_H__

#include <string>
using namespace std;

// CIdInfoDlg dialog
namespace GUI
{
class CIdInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CIdInfoDlg)

public:
	CIdInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIdInfoDlg();

// Dialog Data
	enum { IDD = IDD_ID_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CFont	m_fnt;
public:
	virtual BOOL OnInitDialog();
	UINT m_nGeracao;
	CString m_strPontuacao;
	CString m_strTempo;

	void set(UINT nGeracao		, 
			 double dPontuacao	, 
			 double dTempo		,
			 string	strGenes	);
	CString m_strGenes;
};

};//namespace GUI
#endif //__IDINFODLG_H__