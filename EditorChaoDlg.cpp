// EditorChaoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GaBox2d.h"
#include "EditorChaoDlg.h"
#include <mersenne.h>
#include ".\editorchaodlg.h"

extern MTRand mrand;

// CEditorChaoDlg dialog

IMPLEMENT_DYNAMIC(CEditorChaoDlg, CDialog)
CEditorChaoDlg::CEditorChaoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorChaoDlg::IDD, pParent)
	, m_strDxMedia(_T("0"))
	, m_strDxStdev(_T("2"))
	, m_strDxOffset(_T("6"))
	, m_strDyMedia(_T("0"))
	, m_strDyStdev(_T("0,3"))
	, m_strDyOffset(_T("0"))
	, m_strSinPhi(_T("0"))
	, m_strSinOmega(_T("0"))
	, m_strSinA(_T("0"))
	, m_strTlx(_T("-100"))
	, m_strTly(_T("-500"))
	, m_strBrx(_T("500"))
	, m_strBry(_T("100"))
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
	DDX_Control(pDX, IDC_PREVIEW ,m_wndPreview);
	DDX_Text(pDX, IDC_TL_X, m_strTlx);
	DDX_Text(pDX, IDC_TL_Y, m_strTly);
	DDX_Text(pDX, IDC_BR_X, m_strBrx);
	DDX_Text(pDX, IDC_BR_Y, m_strBry);
	DDX_Text(pDX, IDC_SEED, m_nSeed);
	DDX_Control(pDX, IDC_SPIN_SEED, m_cSpinSeed);
	DDX_Check(pDX, IDC_AUTO_UPDATE, m_bAutoUpdate);
	DDX_Control(pDX, IDC_SLD_VERT, m_cSldVert);
	DDX_Control(pDX, IDC_SLD_HORZ, m_cSldHorz);
}


BEGIN_MESSAGE_MAP(CEditorChaoDlg, CDialog)
	ON_BN_CLICKED(IDC_CRIAR, OnBnClickedCriar)
	ON_BN_CLICKED(IDC_SALVAR, OnBnClickedSalvar)
	ON_BN_CLICKED(IDC_LER, OnBnClickedLer)
	ON_EN_CHANGE(IDC_SEED, OnEnChangeSeed)
	ON_EN_CHANGE(IDC_DX_MEDIA, OnEnChangeDxMedia)
END_MESSAGE_MAP()


// CEditorChaoDlg message handlers
double str2dbl(CString& str)
{
	str.Replace(",",".");
	return atof(str);
}

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


	dxMedia = str2dbl(m_strDxMedia);
	dxStdev			= str2dbl(m_strDxStdev	);
	dxOffset		= str2dbl(m_strDxOffset	);
	dyMedia			= str2dbl(m_strDyMedia	);
	dyStdev			= str2dbl(m_strDyStdev	);
	dyOffset		= str2dbl(m_strDyOffset	);
	sinPhi			= str2dbl(m_strSinPhi	);
	sinOmega		= str2dbl(m_strSinOmega	);
	sinA			= str2dbl(m_strSinA		);
	tlx				= str2dbl(m_strTlx		);
	tly				= str2dbl(m_strTly		);
	brx				= str2dbl(m_strBrx		);
	bry				= str2dbl(m_strBry		);

	m_vecVecs.clear();
	m_vecVecs.push_back(b2Vec2(-100,1));
	m_vecVecs.push_back(b2Vec2(4,1));

	double dx,dy,ldy = 0;
	double lm = 1,m;
	mrand.seed(m_nSeed);
	for(double i = 10.0f; i < 500;i+=0)
	{
		dx = mrand.randNorm(dxMedia,dxStdev)+dxOffset;
		m  = mrand.randNorm(dyMedia,dyStdev)+dyOffset;
		dy = ldy + (dx * (lm + m));
		
		m_vecVecs.push_back(b2Vec2(i,dy + sinA*sin(sinOmega * i + sinPhi)));

		i += dx;
		ldy = dy;
		lm = m;
	}

	m_wndPreview.m_vecGround = m_vecVecs;
	m_wndPreview.m_vecTl     = b2Vec2(tlx,tly);
	m_wndPreview.m_vecBr     = b2Vec2(brx,bry);

	m_wndPreview.Invalidate();
}

void CEditorChaoDlg::OnBnClickedSalvar()
{
}

void CEditorChaoDlg::OnBnClickedLer()
{
}

BOOL CEditorChaoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cSpinSeed.SetRange32(0,INT_MAX);
	m_cSldVert.SetRange(-100,100,TRUE);
	m_cSldHorz.SetRange(-100,100,TRUE);

	return TRUE;
}

void CEditorChaoDlg::OnEnChangeSeed()
{

}

void CEditorChaoDlg::OnEnChangeDxMedia()
{
	UpdateData();
	OnBnClickedCriar();
}
