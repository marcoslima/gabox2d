// GaBox2dView.cpp : implementation of the CGaBox2dView class
//

#include "stdafx.h"
#include "GaBox2d.h"

#include "GaBox2dDoc.h"
#include "GaBox2dView.h"
#include ".\gabox2dview.h"
#include <DcIntegrity.h>
#include "CpuInfoDlg.h"
#include <cronometro.h>
#include "EditorChaoDlg.h"
#include <MessageDlg.h>
#include "EvolucaoDlg.h"
#include <devutils.h>
using namespace DevUtils;
namespace GUI
{
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGaBox2dView

IMPLEMENT_DYNCREATE(CGaBox2dView, CScrollView)

BEGIN_MESSAGE_MAP(CGaBox2dView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_SIMULA_PLAY, OnSimulaPlay)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_SIMULA_PLAY, OnUpdateSimulaPlay)
	ON_COMMAND(ID_SIMULA_RESET, OnSimulaReset)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VELOCIDADE_MAIS, OnVelocidadeMais)
	ON_COMMAND(ID_VELOCIDADE_MENOS, OnVelocidadeMenos)
	ON_UPDATE_COMMAND_UI(ID_VELOCIDADE_MAIS, OnUpdateVelocidadeMais)
	ON_UPDATE_COMMAND_UI(ID_VELOCIDADE_MENOS, OnUpdateVelocidadeMenos)
	ON_COMMAND(ID_SIMULA_REPETIR, OnSimulaRepetir)
	ON_COMMAND(ID_VELOCIDADE_1X, OnVelocidade1x)
	ON_COMMAND(ID_VELOCIDADE_2X, OnVelocidade2x)
	ON_COMMAND(ID_VELOCIDADE_4X, OnVelocidade4x)
	ON_COMMAND(ID_VELOCIDADE_10X, OnVelocidade10x)
	ON_COMMAND(ID_VELOCIDADE_100X, OnVelocidade100x)
	ON_UPDATE_COMMAND_UI(ID_VELOCIDADE_1X, OnUpdateVelocidade1x)
	ON_UPDATE_COMMAND_UI(ID_VELOCIDADE_2X, OnUpdateVelocidade2x)
	ON_UPDATE_COMMAND_UI(ID_VELOCIDADE_4X, OnUpdateVelocidade4x)
	ON_UPDATE_COMMAND_UI(ID_VELOCIDADE_10X, OnUpdateVelocidade10x)
	ON_UPDATE_COMMAND_UI(ID_VELOCIDADE_100X, OnUpdateVelocidade100x)
	ON_COMMAND(ID_GA_INICIARGA, OnGaIniciarga)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_GA_INICIARGA, OnUpdateGaIniciarga)
	ON_UPDATE_COMMAND_UI(ID_MOSTRAR_MELHOR, OnUpdateMostrarMelhor)
	ON_UPDATE_COMMAND_UI(ID_MOSTRAR_QUALQUER, OnUpdateMostrarQualquer)
	ON_COMMAND(ID_MOSTRAR_MELHOR, OnMostrarMelhor)
	ON_COMMAND(ID_MOSTRAR_QUALQUER, OnMostrarQualquer)
	ON_COMMAND(ID_VIEW_CPUINFO, OnViewCpuinfo)
	ON_COMMAND(ID_INFORMA_GAGENES, OnInformaGagenes)
	ON_COMMAND(ID_INFORMA_INDIVIDUOATUAL, OnInformaIndividuoatual)
	ON_UPDATE_COMMAND_UI(ID_INFORMA_GAGENES, OnUpdateInformaGagenes)
	ON_UPDATE_COMMAND_UI(ID_INFORMA_INDIVIDUOATUAL, OnUpdateInformaIndividuoatual)
	ON_COMMAND(ID_GA_EXTINS, OnGaExtins)
	ON_COMMAND(ID_GA_COLAR, OnGaColar)
	ON_UPDATE_COMMAND_UI(ID_GA_EXTINS, OnUpdateGaExtins)
	ON_UPDATE_COMMAND_UI(ID_GA_COLAR, OnUpdateGaColar)
	ON_MESSAGE(IDM_GA_INFO,OnGaInfo)
	ON_MESSAGE(IDM_SIMULAR_GENE,OnSimularGene)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDITARCH, OnUpdateEditEditarch)
	ON_COMMAND(ID_INFORMA_EVOLU, OnInformaEvolu)
	ON_UPDATE_COMMAND_UI(ID_INFORMA_EVOLU, OnUpdateInformaEvolu)
	ON_COMMAND(ID_GA_MUDARPAR, OnGaMudarpar)
END_MESSAGE_MAP()

// CGaBox2dView construction/destruction

CGaBox2dView::CGaBox2dView()
{
	m_nSimTimer = 0;

	m_bmpBack.CreateBitmap(1,1,1,1,NULL);

	m_nVelocidade = 1	 ;
	m_bGaRunning  = false;

	m_bShowInfoId = true;
	m_bShowInfoGaGenes = false;
	m_bWaitingEvolucao = false;
	m_pdlgIdInfo = NULL;
	m_pdlgGaInfo = NULL;
}

CGaBox2dView::~CGaBox2dView()
{
	m_bGaRunning = false;
	SetEvent(_thread_params.m_hStopGa);
	WaitForSingleObject(_thread_params.m_hGaStopped,INFINITE);

	if(m_pdlgGaInfo != NULL)
		delete m_pdlgGaInfo;

	if(m_pdlgIdInfo != NULL)
		delete m_pdlgIdInfo;
}

BOOL CGaBox2dView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

// CGaBox2dView drawing
#pragma warning( disable : 4244 )
void DrawShape(CDC *pDc, b2Vec2 vPos, const b2Shape* shape, COLORREF crFill, COLORREF crCont)
{
	CBrush *pOldBrush, brushFill;
	CPen   *pOldPen, penCont;

	brushFill.CreateSolidBrush(crFill);
	penCont.CreatePen(PS_SOLID,0,crCont);


	pOldBrush = pDc->SelectObject(&brushFill);
	pOldPen   = pDc->SelectObject(&penCont);
	b2Body *pBody;
	float32 fAngle;
	switch (shape->GetType())
	{
	case e_circleShape:
		{
			const b2CircleShape* circle = (const b2CircleShape*)shape;
			b2Vec2 x = vPos + circle->GetLocalPosition();
			x *= 100;
			float32 r = circle->GetRadius();
			r *= 100;
			CRect rcBall(x.x-r,x.y-r,x.x+r,x.y+r);

			pDc->Ellipse(rcBall);
			
			pBody = ((b2Shape *)circle)->GetBody();
			if(pBody)
				fAngle = pBody->GetAngle();
			else
				fAngle = 0;

			pDc->MoveTo(x.x,x.y);
			pDc->LineTo(x.x + r * cos(fAngle), x.y + r * sin(fAngle));
		}

		break;

	case e_polygonShape:
		{
			const b2PolygonShape* poly = (const b2PolygonShape*)shape;
			b2Vec2 v;
			pDc->BeginPath();			
			for (int32 i = 0; i < poly->GetVertexCount(); ++i)
			{
				
				v = vPos + poly->GetVertices()[i];
				v *= 100;
				if(i == 0)
					pDc->MoveTo(v.x,v.y);
				else
					pDc->LineTo(v.x,v.y);
			}
			pDc->CloseFigure();
			pDc->EndPath();
			pDc->StrokeAndFillPath();
		}
		break;

	}
	pDc->SelectObject(pOldBrush);
	pDc->SelectObject(pOldPen);
}
#pragma warning( default: 4244 )


void CGaBox2dView::OnDraw(CDC* pSDc)
{
	CRect rcClient;
	GetClientRect(rcClient);
	int cx,cy;
	cx = rcClient.Width();
	cy = rcClient.Height();

	CDC dcBmp;
	CBitmap  *pOldBmp;
	dcBmp.CreateCompatibleDC(pSDc);
	pOldBmp = dcBmp.SelectObject(&m_bmpBack);
	dcBmp.FillSolidRect(rcClient,RGB(200,230,255));
	
	Draw(&dcBmp);
	dcBmp.SetMapMode(MM_TEXT);
	dcBmp.SetViewportOrg(0,0);
	dcBmp.SetWindowOrg(0,0);
	pSDc->BitBlt(0,0,cx,cy,&dcBmp,0,0,SRCCOPY);

	dcBmp.SelectObject(pOldBmp);
	dcBmp.DeleteDC();
}

void CGaBox2dView::Draw(CDC *pDc)
{
	CDcIntegrity dci(pDc);
	CGaBox2dDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CEnv env = pDoc->m_env;

	Graphics gr(pDc->m_hDC);

	Rect rcClient;
	PointF ptScrCenter; 
	{
		CRect rcc;
		CPoint ptc;
		GetClientRect(rcc);
		ptc = rcc.CenterPoint();

		rcClient = Rect(rcc.left,rcc.top,rcc.right,rcc.bottom);
		ptScrCenter = PointF(ptc.x,ptc.y);
	}
	Rect rcWorld(env._tlx,env._bry,env._brx-env._tlx,env._tly-env._bry);

	gr.SetClip(rcClient,CombineModeReplace);

	gr.FillRectangle(&SolidBrush(Color(0,0,0)),rcClient);

	// Ajustamos a transformação:
	// A transformação será tal que
	// T(x,y) = (a.x + b, c.y + d)
	// onde (x,y) está em world coordinates (wc) e
	// T(x,y) está em device coordinates (dc)
	//
	// sabemos as transformações dos seguintes pontos:
	// T(0,0) = (pc.x,pc.y) -> b = pc.x, d = pc.y
	// T(tl.x,tl.y) = (rc.l,rc.t)
	// T(br.x,br.y) = (rc.r,rc.b)
	//
	// a.tl_x + pc_x = rc_l
	// a.br_x + pc_x = rc.r
	//-----------------------
	// a(tl_x-br_x) = (rc_l-rc.r)
	//
	//      (rc_l-rc_r)
	// a = -------------
	//      (tl_x-br_x)
	//
	//      (rc_t-rc_b)
	// b = -------------
	//      (tl_y-br_y)
	////////////////////////////////////////////////////////////

//	double dZoom = pow(1.3,(double)m_zoom);
	double dZoom = 10.0;
	int cx,cy;
	PointF pos = pDoc->GetCenter();

	cx = ptScrCenter.X - (pos.X*dZoom);
	cy = ptScrCenter.Y + (pos.Y*dZoom);
	
	Matrix mt( dZoom	,	0		,
		       0		,	-dZoom	,
			   cx		,	cy		);
	gr.SetTransform(&mt);

	// World na cor de céu
	SolidBrush bshSky(Color(100,100,255));
	gr.FillRectangle(&bshSky,rcWorld);

	vec_vecs_t* pVg = &pDoc->m_vecGround;
	size_t i,nSize = pVg->size();
	if(nSize == 0)
		return;

	PointF *pPoints = new PointF[nSize];
	for(i = 0; i < nSize; i++)
	{
		pPoints[i].X = (*pVg)[i].x;
		pPoints[i].Y = (*pVg)[i].y;
	}

	Pen penGround(Color(0,0,0),0.1);
	SolidBrush bshGround(Color(32,128,32));
	gr.FillPolygon(&bshGround,pPoints,nSize);
	gr.DrawPolygon(&penGround,pPoints,nSize);
	delete pPoints;

	Pen penBorder(Color(255,0,0),5);
	gr.DrawRectangle(&penBorder,rcWorld);

	pDoc->GetCar().Draw(&gr);

	m_pdlgIdInfo->set(-1,-1,
		pDoc->GetCar().getT(),
		pDoc->GetCar().getGenes());


}

void CGaBox2dView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_fntSmall.CreateFont(-12,0,0,0,0,0,0,0,0,0,0,0,0,"Courier New");
	m_fntSupersmall.CreateFont(-8,0,0,0,0,0,0,0,0,0,0,0,0,"small fonts");

	m_pdlgIdInfo = new CIdInfoDlg(this);
	m_pdlgIdInfo->Create(CIdInfoDlg::IDD,this);
	m_pdlgIdInfo->ShowWindow(SW_SHOW);

	m_pdlgGaInfo = new CGaInfoDlg(this);
	m_pdlgGaInfo->m_pView = this;
	m_pdlgGaInfo->Create(CGaInfoDlg::IDD,this);
	m_pdlgGaInfo->ShowWindow(SW_SHOW);

}

 
// CGaBox2dView printing

BOOL CGaBox2dView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGaBox2dView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CGaBox2dView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


// CGaBox2dView diagnostics

#ifdef _DEBUG
void CGaBox2dView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGaBox2dView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGaBox2dDoc* CGaBox2dView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGaBox2dDoc)));
	return (CGaBox2dDoc*)m_pDocument;
}
#endif //_DEBUG


// CGaBox2dView message handlers

void CGaBox2dView::OnSimulaPlay()
{
	if(m_nSimTimer == 0)
	{
		// Ligar simulação:
		GetDocument()->BeginSimulation();
		m_nSimTimer = (UINT)SetTimer((UINT_PTR)1001,10,NULL);
	}
	else
	{
		KillTimer(m_nSimTimer);
		m_nSimTimer = 0;
		GetDocument()->EndSimulation();
	}
}

void CGaBox2dView::OnTimer(UINT nIDEvent)
{
	static bool bWorking = false;
	if(bWorking)
		return;
	bWorking = true;

	CGaBox2dDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		bWorking = false;
		return;
	}

	UINT k;
	for(k = 0; k < m_nVelocidade; k++)
	{
		if(!pDoc->GetCar().doStep())
		{
//			OnSimulaPlay();
//			break;
		}
	}
	Invalidate();

	CScrollView::OnTimer(nIDEvent);
	bWorking = false;
}

void CGaBox2dView::OnUpdateSimulaPlay(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nSimTimer != 0);
}

void CGaBox2dView::OnSimulaReset()
{
	CGaBox2dDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if(m_nSimTimer != 0)
		OnSimulaPlay();

	pDoc->GetCar().CreateCar();
	m_pdlgIdInfo->set(0,0,0,"");

	if(m_nSimTimer == 0)
		OnSimulaPlay();
}

void CGaBox2dView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	if(cx == 0 || cy == 0)
		return;

	m_bmpBack.DeleteObject();
	CDC *pDc = GetDC();
	CRect rcClient;
	GetClientRect(rcClient);

	m_bmpBack.CreateCompatibleBitmap(pDc,rcClient.Width(),rcClient.Height());
}

BOOL CGaBox2dView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CGaBox2dView::OnVelocidadeMais()
{
	m_nVelocidade++;
}

void CGaBox2dView::OnVelocidadeMenos()
{
	m_nVelocidade--;
}

void CGaBox2dView::OnUpdateVelocidadeMais(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_nVelocidade < 1000);
}

void CGaBox2dView::OnUpdateVelocidadeMenos(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_nVelocidade > 1);
}

void CGaBox2dView::OnSimulaRepetir()
{
	CGaBox2dDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CString strGenes;
	pDoc->GetCar().getGenes(strGenes);

	if(m_nSimTimer != 0)
		OnSimulaPlay();

	pDoc->GetCar().CreateCar(strGenes);

	if(m_nSimTimer == 0)
		OnSimulaPlay();
}

void CGaBox2dView::OnVelocidade1x()
{
	m_nVelocidade = 1;
}

void CGaBox2dView::OnVelocidade2x()
{
	m_nVelocidade = 2;
}

void CGaBox2dView::OnVelocidade4x()
{
	m_nVelocidade = 4;
}

void CGaBox2dView::OnVelocidade10x()
{
	m_nVelocidade = 10;
}

void CGaBox2dView::OnVelocidade100x()
{
	m_nVelocidade = 100;
}

void CGaBox2dView::OnUpdateVelocidade1x(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nVelocidade == 1);
}

void CGaBox2dView::OnUpdateVelocidade2x(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nVelocidade == 2);
}

void CGaBox2dView::OnUpdateVelocidade4x(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nVelocidade == 4);
}

void CGaBox2dView::OnUpdateVelocidade10x(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nVelocidade == 10);
}

void CGaBox2dView::OnUpdateVelocidade100x(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nVelocidade == 100);
}

void fnGa(void *pParam)
{
	CThreadParams *tp		= (CThreadParams *)pParam;
	HANDLE hStopGA			= tp->m_hStopGa		;
	HANDLE hGaStopped		= tp->m_hGaStopped	;
	HWND   hWndNotify		= tp->m_wndNotify	;
	ga_params_t gaParams	= tp->m_Params		;
	CGaInfo*	pGaInfo		= tp->m_pGaInfo		;
	b2World *pWorld			= PHYS::buildWorld(&tp->m_env) ;

	CGa ga;
	// Preparamos os parâmetros do GA:
	double cross,mut;
	gaParams.m_strMutacao.Replace(",",".");
	gaParams.m_strCrossover.Replace(",",".");

	cross = atof(gaParams.m_strCrossover);
	mut   = atof(gaParams.m_strMutacao);

	ga.setParams	(	gaParams.m_nPopulacao	,	// Número de indivíduos
						gaParams.m_nElitismo	,	// Tamanho do elitismo
						cross					,	// Probabilidade de crossover
						mut						,	// Probabilidade de mutação
						gaParams.m_nAlienismo	,	// Tamanho do alienismo
						gaParams.m_nMutInt		,	// Intensidade da mutação
						gaParams.m_dMaxT		);	// Tempo máximo à ser simulado

	ga.BeginEvolve();
	size_t nSize = ga.getPopulacaoLen();

	CCronometro crInfo,crGa;
	crInfo.Start();
	
	// Medição da velocidade gerações por segundo:
	double gps = -1;
	int nCount = 0;
	crGa.Start();

	while(WaitForSingleObject(hStopGA,0) == WAIT_TIMEOUT)
	{
		ga.Ordena(pWorld,hStopGA);
		if(crInfo.Get() > 250)
		{
			pGaInfo->Lock();
			pGaInfo->m_geracao = ga.getGeracao();
			pGaInfo->m_gps = gps;
			ga.CopyPopulacao(&pGaInfo->m_populacao);

			if(pGaInfo->m_reqMelhores)
			{
				pGaInfo->m_reqMelhores = false;
				pGaInfo->m_vecMelhores = ga.m_melhores;
			}
			if(pGaInfo->m_reqExtincao)
			{
				pGaInfo->m_reqExtincao = false;
				ga.MassExtinctionEvent();
			}

			pGaInfo->Release();
			crInfo.Start();
			PostMessage(hWndNotify,IDM_GA_INFO,0,0);
		}
		ga.Step();

		nCount++;
		if(nCount == 10)
		{
			gps = 10.0 / crGa.Get(CCronometro::uS);
			nCount = 0;
			crGa.Start();
		}

//		if(gaParams.m_b
	}

	delete pWorld;
	SetEvent(hGaStopped);
}

void CGaBox2dView::OnGaIniciarga()
{
	if(m_bGaRunning)
	{
		// Já está rodando, então é para parar:
		if(AfxMessageBox("Tem certeza de que quer parar o GA?",MB_YESNO) == IDNO)
			return;

		// Ok, vamos parar:
		CMessageDlg dlgMsg;
		dlgMsg.BeginMessage("Interrompendo GA...",this);
		SetEvent(_thread_params.m_hStopGa);
		WaitForSingleObject(_thread_params.m_hGaStopped,INFINITE);
		dlgMsg.EndMessage();

		m_bGaRunning = false;

		// Liberamos os eventos:
		CloseHandle(_thread_params.m_hStopGa    );
		CloseHandle(_thread_params.m_hGaStopped );

		// Pronto!
		return;
	}


	// Obtemos os parâmetros do GA:
	CGaParamsDlg dlgParams;
	if(dlgParams.DoModal() != IDOK)
	{
		return;
	}

	_thread_params.m_Params		= dlgParams;
	_thread_params.m_hStopGa    = CreateEvent(NULL,TRUE,FALSE,NULL);
	_thread_params.m_hGaStopped = CreateEvent(NULL,TRUE,FALSE,NULL);
	_thread_params.m_wndNotify  = m_hWnd;
	_thread_params.m_pGaInfo	= &m_GaInfo;
	_thread_params.m_env		= GetDocument()->m_env;

	m_bGaRunning = true;
	_beginthread(fnGa,0,(void *)&_thread_params);
}

void CGaBox2dView::OnEditPaste()
{
	CString buffer;
	COleDataObject	obj;

	if (obj.AttachClipboard()) 
	{
		if (obj.IsDataAvailable(CF_TEXT)) 
		{
			HGLOBAL hmem = obj.GetGlobalData(CF_TEXT);
			unsigned char *pBuf;
			size_t nSize;

			pBuf = (unsigned char *)::GlobalLock(hmem);
			nSize = ::GlobalSize(hmem);
			CMemFile sf(pBuf ,(UINT)nSize );

			LPSTR str = buffer.GetBufferSetLength((int)::GlobalSize(hmem));
			sf.Read(str,(UINT) ::GlobalSize(hmem));
			::GlobalUnlock(hmem);
			buffer.ReleaseBuffer();
		}
	}

	size_t i,nSize = buffer.GetLength();
	if(nSize != GENES)
		return;


	for(i = 0; i < nSize; i++)
	{
		if(buffer.GetAt((int)i) < 'A' || buffer.GetAt((int)i) > 'Z')
		{
			return;
		}
	}

	// Ok, podemos colar:
	CGaBox2dDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pdlgIdInfo->set(0,0,0,"nenhum");
	if(m_nSimTimer != 0)
		OnSimulaPlay();

	pDoc->GetCar().CreateCar(buffer);

	if(m_nSimTimer == 0)
		OnSimulaPlay();
}

void CGaBox2dView::OnUpdateGaIniciarga(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGaRunning);
}

void CGaBox2dView::OnUpdateMostrarMelhor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bGaRunning);
}

void CGaBox2dView::OnUpdateMostrarQualquer(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bGaRunning);
}

void CGaBox2dView::OnMostrarMelhor()
{
	CGaBox2dDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if(m_nSimTimer != 0)
		OnSimulaPlay();

	m_GaInfo.Lock();
	{
		if(m_GaInfo.m_populacao.size() == 0)
		{
			m_GaInfo.Release();
			AfxMessageBox("Não há ninguém para mostrar ainda.");
			return;
		}
		pDoc->GetCar().CreateCar(m_GaInfo.m_populacao.begin()->getGenesCString());
		m_pdlgIdInfo->set(	m_GaInfo.m_geracao,
							m_GaInfo.m_populacao.begin()->_pontos,
							m_GaInfo.m_populacao.begin()->getT(),
							m_GaInfo.m_populacao.begin()->getGenes());
	}
	m_GaInfo.Release();


	if(m_nSimTimer == 0)
		OnSimulaPlay();
}

void CGaBox2dView::OnMostrarQualquer()
{
	CGaBox2dDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if(m_nSimTimer != 0)
		OnSimulaPlay();

	m_GaInfo.Lock();
	{
		size_t nQq = rand()%m_GaInfo.m_populacao.size();
		lst_car_t::iterator it;
		for(int i = 0; i < nQq; i++) it++;

		pDoc->GetCar().CreateCar(it->getGenesCString());

		m_pdlgIdInfo->set(	m_GaInfo.m_geracao,
							it->_pontos,
							it->getT(),
							it->getGenes());

	}
	m_GaInfo.Release();

	if(m_nSimTimer == 0)
		OnSimulaPlay();
}

void CGaBox2dView::OnViewCpuinfo()
{
	CCpuInfoDlg dlgInfo;
	dlgInfo.DoModal();
}
void CGaBox2dView::OnInformaGagenes()
{
	m_bShowInfoGaGenes = !m_bShowInfoGaGenes;
	Invalidate();
}

void CGaBox2dView::OnInformaIndividuoatual()
{
	m_bShowInfoId = !m_bShowInfoId;
	Invalidate();
}

void CGaBox2dView::OnUpdateInformaGagenes(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowInfoGaGenes);
}

void CGaBox2dView::OnUpdateInformaIndividuoatual(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowInfoId);
}

void CGaBox2dView::OnGaExtins()
{
	m_GaInfo.Lock();
	m_GaInfo.m_reqExtincao = true;
	m_GaInfo.Release();
}

void CGaBox2dView::OnGaColar()
{
	CString buffer;
	COleDataObject	obj;

	if (obj.AttachClipboard()) 
	{
		if (obj.IsDataAvailable(CF_TEXT)) 
		{
			HGLOBAL hmem = obj.GetGlobalData(CF_TEXT);
			unsigned char *pBuf;
			size_t nSize;

			pBuf = (unsigned char *)::GlobalLock(hmem);
			nSize = ::GlobalSize(hmem);
			CMemFile sf(pBuf ,(UINT)nSize );

			LPSTR str = buffer.GetBufferSetLength((int)::GlobalSize(hmem));
			sf.Read(str,(UINT) ::GlobalSize(hmem));
			::GlobalUnlock(hmem);
			buffer.ReleaseBuffer();
		}
	}

	size_t i,nSize = buffer.GetLength();
	if(nSize != GENES)
		return;


	for(i = 0; i < nSize; i++)
	{
		if(buffer.GetAt((int)i) < 'A' || buffer.GetAt((int)i) > 'Z')
		{
			return;
		}
	}

	// Ok, podemos colar:
	m_GaInfo.Lock();
	m_GaInfo.m_reqIncludeId = true;
	m_GaInfo.m_strId2Include = buffer;
	m_GaInfo.Release();
}

void CGaBox2dView::OnUpdateGaExtins(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bGaRunning);
}

void CGaBox2dView::OnUpdateGaColar(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bGaRunning);
}

afx_msg	LRESULT CGaBox2dView::OnGaInfo(WPARAM wParma, LPARAM lParam)
{
	Invalidate();
	CGaBox2dDoc *pDoc = GetDocument();

	if(m_bWaitingEvolucao)
	{
		m_bWaitingEvolucao = false;

		CEvolucaoDlg dlgEvolucao;
		m_GaInfo.Lock();
		dlgEvolucao.m_vecCarros = m_GaInfo.m_vecMelhores;
		m_GaInfo.Release();
		dlgEvolucao.DoModal();
	}

	m_GaInfo.Lock();
	m_pdlgGaInfo->Refresh(&m_GaInfo);
	m_GaInfo.Release();

	return 0L;
}
void CGaBox2dView::OnUpdateEditEditarch(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bGaRunning);
}

void CGaBox2dView::OnInformaEvolu()
{
	// Marcamos o evento:
	m_GaInfo.Lock();
	m_GaInfo.m_reqMelhores = true;
	m_bWaitingEvolucao = true;
	m_GaInfo.Release();
}

void CGaBox2dView::OnUpdateInformaEvolu(CCmdUI *pCmdUI)
{
	if(m_bWaitingEvolucao)
	{
		pCmdUI->SetText("Evolução (em andamento...)");
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->SetText("Evolução");
		pCmdUI->Enable(TRUE);
	}
}

LRESULT CGaBox2dView::OnSimularGene(WPARAM wParam, LPARAM lParam)
{
	//TODO: Reativar função
#if 0
	if(m_nSimTimer != 0)
		OnSimulaPlay();

	GetDocument()->m_car.CreateCar((LPCSTR) lParam);
	m_pdlgIdInfo->set(	m_GaInfo.m_geracao,
						m_GaInfo.m_populacao[0]._pontos,
						m_GaInfo.m_populacao[0]._t,
						m_GaInfo.m_populacao[0].getGenes());

	OnSimulaPlay();

#endif
	return 0L;
}

void CGaBox2dView::OnGaMudarpar()
{
	CGaParamsDlg dlgParams;
	if(dlgParams.DoModal() != IDOK)
	{
		return;
	}
// TODO: Reativar
//	_thread_params.m_Params = dlgParams;
//	_thread_params.bParamsChanged = true;
}

};//namespace GUI
