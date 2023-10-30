// CpuInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GaBox2d.h"
#include "CpuInfoDlg.h"
//#include <cpuid.h>


// CCpuInfoDlg dialog
namespace GUI
{
IMPLEMENT_DYNAMIC(CCpuInfoDlg, CDialog)
CCpuInfoDlg::CCpuInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCpuInfoDlg::IDD, pParent)
	, m_strInfo(_T(""))
{
}

CCpuInfoDlg::~CCpuInfoDlg()
{
}

void CCpuInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INFO, m_strInfo);
}


BEGIN_MESSAGE_MAP(CCpuInfoDlg, CDialog)
END_MESSAGE_MAP()


// CCpuInfoDlg message handlers


BOOL CCpuInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

};//namespace GUI