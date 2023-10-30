#pragma once


// CCpuInfoDlg dialog

class CCpuInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CCpuInfoDlg)

public:
	CCpuInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCpuInfoDlg();

// Dialog Data
	enum { IDD = IDD_CPUINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strInfo;
	virtual BOOL OnInitDialog();
};
