// CpuInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GaBox2d.h"
#include "CpuInfoDlg.h"
#include "cpuid.h"


// CCpuInfoDlg dialog

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

	_p_info info;
	_cpuid(&info);

	m_strInfo.Format(
		"Cpu: %s\r\n"		
		"Modelo: %s\r\n"		
		"Familia: %d\r\n"		
		"Modelo: %d\r\n"		
		"Stepping:%d\r\n"		
		, info.v_name		
		, info.model_name	
		, info.family		
		, info.model		
		, info.stepping		
		);

	if(info.feature & _CPU_FEATURE_MMX) 
	{
		m_strInfo += "\r\nMMX Ok!";
	}
	else
	{
		m_strInfo += "\r\nnão tem MMX";
	}

    if(info.feature & _CPU_FEATURE_SSE) 
	{
		m_strInfo += "\r\nSSE Ok!";
	}
	else
	{
		m_strInfo += "\r\nnão tem SSE";
	}

    if(info.feature & _CPU_FEATURE_SSE2) 
	{
		m_strInfo += "\r\nSSE2 Ok!";
	}
	else
	{
		m_strInfo += "\r\nnão tem SSE2";
	}

    if(info.feature & _CPU_FEATURE_3DNOW) 
	{
		m_strInfo += "\r\n'3d Now' Ok!";
	}
	else
	{
		m_strInfo += "\r\nnão tem '3d Now'";
	}


	UpdateData(FALSE);
	return TRUE;
}
