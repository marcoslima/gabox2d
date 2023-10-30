// EditorChaoDlg.cpp : implementation file
//
#if 0

#include "GaBox2d.h"
#include "EditorChaoDlg.h"
#include <iostream>
#include <fstream>
#include <devutils.h>
using namespace std;
using namespace DevUtils;

extern MTRand mrand;

namespace GUI
{



// CEditorChaoDlg dialog

IMPLEMENT_DYNAMIC(CEditorChaoDlg, CDialog)
CEditorChaoDlg::CEditorChaoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorChaoDlg::IDD, pParent)
	, m_strDxMedia(_T("0"))
	, m_strDxStdev(_T("10"))
	, m_strDxOffset(_T("50"))
	, m_strDyMedia(_T("0"))
	, m_strDyStdev(_T("3"))
	, m_strDyOffset(_T("0"))
	, m_strSinPhi(_T("0"))
	, m_strSinOmega(_T("0"))
	, m_strSinA(_T("0"))
	, m_strTlx(_T("-100"))
	, m_strTly(_T("500"))
	, m_strBrx(_T("500"))
	, m_strBry(_T("-50"))
	, m_nSeed(0)
	, m_bAutoUpdate(FALSE)
{
}

CEditorChaoDlg::~CEditorChaoDlg()
{
}

void CEditorChaoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DX_MEDIA, m_strDxMedia);
	DDX_Text(pDX, IDC_DX_STDEV, m_strDxStdev);
	DDX_Text(pDX, IDC_DX_OFFSET, m_strDxOffset);
	DDX_Text(pDX, IDC_DY_MEDIA, m_strDyMedia);
	DDX_Text(pDX, IDC_DY_STDEV, m_strDyStdev);
	DDX_Text(pDX, IDC_DY_OFFSET, m_strDyOffset);
	DDX_Text(pDX, IDC_SIN_PHI, m_strSinPhi);
	DDX_Text(pDX, IDC_SIN_OMEGA, m_strSinOmega);
	DDX_Text(pDX, IDC_SIN_A, m_strSinA);
	DDX_Text(pDX, IDC_TL_X, m_strTlx);
	DDX_Text(pDX, IDC_TL_Y, m_strTly);
	DDX_Text(pDX, IDC_BR_X, m_strBrx);
	DDX_Text(pDX, IDC_BR_Y, m_strBry);
	DDX_Text(pDX, IDC_SEED, m_nSeed);

	DDX_Check(pDX, IDC_AUTO_UPDATE, m_bAutoUpdate);

	DDX_Control(pDX, IDC_SPIN_SEED, m_cSpinSeed);
	DDX_Control(pDX, IDC_SPIN_PHI, m_cSpinPhi);
	DDX_Control(pDX, IDC_SPIN_OMEGA, m_cSpinOmega);
	DDX_Control(pDX, IDC_SPIN_A, m_cSpinA);
	DDX_Control(pDX, IDC_SPIN_DXM, m_cSpinDxm);
	DDX_Control(pDX, IDC_SPIN_DXS, m_cSpinDxs);
	DDX_Control(pDX, IDC_SPIN_DXO, m_cSpinDxo);
	DDX_Control(pDX, IDC_SPIN_DYM, m_cSpinDym);
	DDX_Control(pDX, IDC_SPIN_DYS, m_cSpinDys);
	DDX_Control(pDX, IDC_SPIN_DYO, m_cSpinDyo);

	DDX_Control(pDX, IDC_SLD_VERT, m_cSldVert);
	DDX_Control(pDX, IDC_SLD_HORZ, m_cSldHorz);
	DDX_Control(pDX, IDC_PREVIEW ,m_wndPreview);
}


BEGIN_MESSAGE_MAP(CEditorChaoDlg, CDialog)
	ON_BN_CLICKED(IDC_CRIAR, OnBnClickedCriar)
	ON_BN_CLICKED(IDC_SALVAR, OnBnClickedSalvar)
	ON_BN_CLICKED(IDC_LER, OnBnClickedLer)
	ON_EN_CHANGE(IDC_SEED, OnEnChangeSeed)
	ON_EN_CHANGE(IDC_DX_MEDIA, OnEnChangeDxMedia)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_ZOOM_IN, OnBnClickedZoomIn)
	ON_BN_CLICKED(IDC_ZOOM_OUT, OnBnClickedZoomOut)
	ON_BN_CLICKED(IDC_AUTO_UPDATE, OnBnClickedAutoUpdate)
	ON_EN_CHANGE(IDC_SIN_PHI, OnEnChangeSinPhi)
	ON_EN_CHANGE(IDC_SIN_OMEGA, OnEnChangeSinOmega)
	ON_EN_CHANGE(IDC_SIN_A, OnEnChangeSinA)
	ON_EN_CHANGE(IDC_DX_STDEV, OnEnChangeDxStdev)
	ON_EN_CHANGE(IDC_DX_OFFSET, OnEnChangeDxOffset)
	ON_EN_CHANGE(IDC_DY_MEDIA, OnEnChangeDyMedia)
	ON_EN_CHANGE(IDC_DY_STDEV, OnEnChangeDyStdev)
	ON_EN_CHANGE(IDC_DY_OFFSET, OnEnChangeDyOffset)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CEditorChaoDlg message handlers
void CEditorChaoDlg::OnBnClickedCriar()
{
	UpdateData();
	double  dxMedia		,
			dxStdev		,
			dxOffset	,
			dyMedia		,
			dyStdev		,
			dyOffset	,
			sinPhi		,
			sinOmega	,
			sinA		,
			tlx			,
			tly			,
			brx			,
			bry;

	dxMedia			= str2dbl(m_strDxMedia  )/10.0;
	dxStdev			= str2dbl(m_strDxStdev	)/10.0;
	dxOffset		= str2dbl(m_strDxOffset	)/10.0;
	dyMedia			= str2dbl(m_strDyMedia	)/10.0;
	dyStdev			= str2dbl(m_strDyStdev	)/10.0;
	dyOffset		= str2dbl(m_strDyOffset	)/10.0;
	sinPhi			= str2dbl(m_strSinPhi	);
	sinOmega		= str2dbl(m_strSinOmega	);
	if(sinOmega != 0) sinOmega = 1.0 / sinOmega;
	sinA			= str2dbl(m_strSinA		)/10.0;
	tlx				= str2dbl(m_strTlx		);
	tly				= str2dbl(m_strTly		);
	brx				= str2dbl(m_strBrx		);
	bry				= str2dbl(m_strBry		);

	m_World.set(m_nSeed,
				dxMedia,dxStdev,dxOffset,
				dyMedia,dyStdev,dyOffset,
				sinPhi,sinOmega,sinA,
				tlx,tly,brx,bry);

	m_vecVecs.clear();

	m_wndPreview.m_env = m_World;
	m_wndPreview.m_vecTl     = b2Vec2(tlx,tly);
	m_wndPreview.m_vecBr     = b2Vec2(brx,bry);


	m_wndPreview.Invalidate();
}

void CEditorChaoDlg::OnBnClickedSalvar()
{
	CString strFile;

	CFileDialog dlgFile (
		FALSE, 
		_T("ada"), 
		strFile, 
		OFN_ENABLESIZING|OFN_EXPLORER|OFN_LONGNAMES|OFN_NOREADONLYRETURN|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST, 
		_T("Arquivos de defini��o de ambiente (*.ada)|*.ada|Todos os arquivos (*.*)|*.*|"), 
		this );
	dlgFile.m_ofn.lpstrTitle = _T("Salvar configura��o de ambiente");

	if(dlgFile.DoModal() != IDOK) 
		return;

	strFile = dlgFile.GetPathName();

	UpdateData();

	ofstream fOut(strFile);
	fOut << m_World.get();
}

void CEditorChaoDlg::OnBnClickedLer()
{

	CString strFile;

	CFileDialog dlgFile (
		TRUE, 
		_T("ada"), 
		strFile, 
		OFN_ENABLESIZING|OFN_EXPLORER|OFN_LONGNAMES|OFN_NOREADONLYRETURN|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST, 
		_T("Arquivos de defini��o de ambiente (*.ada)|*.ada|Todos os arquivos (*.*)|*.*|"), 
		this );
	dlgFile.m_ofn.lpstrTitle = _T("Salvar configura��o de ambiente");

	if(dlgFile.DoModal() != IDOK) 
		return;

	strFile = dlgFile.GetPathName();

	string strParams;

	ifstream fIn(strFile);
	while(fIn)
		fIn >> strParams;

	UpdateData();
	m_World.set(strParams);

	m_strDxMedia  = dbl2str(m_World._dxm*10);
	m_strDxStdev  = dbl2str(m_World._dxs*10);
	m_strDxOffset = dbl2str(m_World._dxo*10);

	m_strDyMedia  = dbl2str(m_World._dym*10);
	m_strDyStdev  = dbl2str(m_World._dys*10);
	m_strDyOffset = dbl2str(m_World._dyo*10);

	m_strSinPhi   = dbl2str(m_World._phi	);
	m_strSinOmega = dbl2str((m_World._omega == 0)?(0):(1.0/m_World._omega));
	m_strSinA	  = dbl2str(m_World._a*10	);


	m_strTlx = dbl2str(m_World._tlx);
	m_strTly = dbl2str(m_World._tly);
	m_strBrx = dbl2str(m_World._brx);
	m_strBry = dbl2str(m_World._bry);

	m_nSeed = m_World._seed;

	UpdateData(FALSE);
	OnBnClickedCriar();
}

BOOL CEditorChaoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cSldVert.SetRange(-1000,1000,TRUE);
	m_cSldHorz.SetRange(-1000,1000,TRUE);

	m_cSpinSeed.SetRange32(0,INT_MAX);
	m_cSpinPhi.SetRange32(0,INT_MAX);
	m_cSpinOmega.SetRange32(0,INT_MAX);
	m_cSpinA  .SetRange32(0,INT_MAX);
	m_cSpinDxm.SetRange32(0,INT_MAX);
	m_cSpinDxs.SetRange32(0,INT_MAX);
	m_cSpinDxo.SetRange32(0,INT_MAX);
	m_cSpinDym.SetRange32(0,INT_MAX);
	m_cSpinDys.SetRange32(0,INT_MAX);
	m_cSpinDyo.SetRange32(0,INT_MAX);

	OnBnClickedCriar();
	return TRUE;
}

void CEditorChaoDlg::OnEnChangeSeed()
{
	if(m_bAutoUpdate)
	{
		UpdateData();
		OnBnClickedCriar();
	}
}

void CEditorChaoDlg::OnEnChangeDxMedia()
{
	if(m_bAutoUpdate) OnBnClickedCriar();
}

void CEditorChaoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(pScrollBar == (CScrollBar *)&m_cSldHorz)
	{
		UpdateData();
		m_wndPreview.SetCenter(m_cSldHorz.GetPos(),m_cSldVert.GetPos());
		m_wndPreview.Invalidate();
	}
	else
	{
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

void CEditorChaoDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(pScrollBar == (CScrollBar *)&m_cSldVert)
	{
		UpdateData();
		m_wndPreview.SetCenter(m_cSldHorz.GetPos(),m_cSldVert.GetPos());
		m_wndPreview.Invalidate();
	}
	else
	{
		CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}

void CEditorChaoDlg::OnBnClickedZoomIn()
{
	m_wndPreview.m_zoom++;
	m_wndPreview.Invalidate();
}

void CEditorChaoDlg::OnBnClickedZoomOut()
{
	m_wndPreview.m_zoom--;
	m_wndPreview.Invalidate();
}

void CEditorChaoDlg::OnBnClickedAutoUpdate()
{
	UpdateData();
}

void CEditorChaoDlg::OnEnChangeSinPhi()
{
	if(m_bAutoUpdate) OnBnClickedCriar();
}

void CEditorChaoDlg::OnEnChangeSinOmega()
{
	if(m_bAutoUpdate) OnBnClickedCriar();
}

void CEditorChaoDlg::OnEnChangeSinA()
{
	if(m_bAutoUpdate) OnBnClickedCriar();
}

void CEditorChaoDlg::OnEnChangeDxStdev()
{
	if(m_bAutoUpdate) OnBnClickedCriar();
}

void CEditorChaoDlg::OnEnChangeDxOffset()
{
	if(m_bAutoUpdate) OnBnClickedCriar();
}

void CEditorChaoDlg::OnEnChangeDyMedia()
{
	if(m_bAutoUpdate) OnBnClickedCriar();
}

void CEditorChaoDlg::OnEnChangeDyStdev()
{
	if(m_bAutoUpdate) OnBnClickedCriar();
}

void CEditorChaoDlg::OnEnChangeDyOffset()
{
	if(m_bAutoUpdate) OnBnClickedCriar();
}

void CEditorChaoDlg::OnBnClickedOk()
{
	OnOK();
}

};//namespace GUI

#endif