// GaBox2dView.cpp : implementation of the CGaBox2dView class
//

#include "stdafx.h"
#include "GaBox2d.h"

#include "GaBox2dDoc.h"
#include "GaBox2dView.h"
#include ".\gabox2dview.h"
#include <DcIntegrity.h>
#include "GaParamsDlg.h"
#include "CpuInfoDlg.h"
#include <cronometro.h>


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
END_MESSAGE_MAP()

// CGaBox2dView construction/destruction

CGaBox2dView::CGaBox2dView()
{
	m_nSimTimer = 0;

	m_bmpBack.CreateBitmap(1,1,1,1,NULL);

	m_nVelocidade = 1	 ;
	m_bGaRunning  = false;

	InitializeCriticalSection(&m_csInfoIds);

	m_bShowInfoId = true;
	m_bShowInfoGaGenes = false;
}

CGaBox2dView::~CGaBox2dView()
{
	m_bGaRunning = false;
	while(!m_bGaExited)
		Sleep(10);

	DeleteCriticalSection(&m_csInfoIds);
}

BOOL CGaBox2dView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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

	CRect rcClient;
	CPoint ptCenter;
	GetClientRect(rcClient);

	pDc->SelectObject(&m_fntSmall);

	CString strInfos;
	string sGenes;
	pDoc->m_car.getGenes(sGenes);

	if(true)
	{
		EnterCriticalSection(&m_csInfoIds);
		{
			strInfos.Format(
				"Individuo atual:\r\n"
				"Genes: %s\r\n"
				"R1(m,f,r): (%.2f,%.2f,%.2f)\r\n"
				"R2(m,f,r): (%.2f,%.2f,%.2f)\r\n"
				"m(P1,P2): (%.2f,%.2f)\r\n"
				"torq: %g\r\n"
				"contato(R1,R2): (%.2f,%.2f)\r\n"
				"(d, t, vm): (%.2f, %.2f, %.2f)\r\n"
				"trq(angulo,a,b,c,d): (%.2f,%.2f,%.2f,%.2f,%.2f)\r\n"
				,sGenes.c_str()
				,pDoc->m_car.getR1()->GetMass()
				,pDoc->m_car.getR1()->GetShapeList()->GetFriction()
				,pDoc->m_car.getR1()->GetShapeList()->GetRestitution()
				,pDoc->m_car.getR2()->GetMass()
				,pDoc->m_car.getR2()->GetShapeList()->GetFriction()
				,pDoc->m_car.getR2()->GetShapeList()->GetRestitution()
				,pDoc->m_car.getP1()->GetMass()
				,pDoc->m_car.getP2()->GetMass()
				,pDoc->m_car.getTorque()
				,pDoc->m_car.m_contatoR1
				,pDoc->m_car.m_contatoR2
				,pDoc->m_car.m_distancia
				,pDoc->m_car._t
				,pDoc->m_car.m_vm
				,pDoc->m_car._angle
				,pDoc->m_car._trqA
				,pDoc->m_car._trqB
				,pDoc->m_car._trqC
				,pDoc->m_car._trqD
				);

//				"Geracao: %d\r\n"
//				,m_nGeracao
		}
		LeaveCriticalSection(&m_csInfoIds);
	}
	else
	{
		strInfos.Format("Velocidade: %dx\r\n",m_nVelocidade);
	}

	CRect rcInfo(0,0,1,1);
	if(m_bShowInfoId)
	{
		pDc->DrawText(strInfos,rcInfo,DT_CALCRECT);
		pDc->DrawText(strInfos,rcInfo,0);
	}

	pDc->SelectObject(&m_fntSupersmall);
	CString strAllGenes;
	int maxLen = 0;
	CRect rcAllGenes;
	if(m_bShowInfoGaGenes)
	{
		EnterCriticalSection(&m_csInfoIds);
		{
			CString strLine;
			size_t i,nSize = m_vecInfoIds.size();
			for(i = 0; i < nSize; i++)
			{
				strLine.Format("%.4f|%s\r\n",m_vecInfoIds[i].dPoints, m_vecInfoIds[i].strGenes);
				maxLen = __max(maxLen, pDc->GetTextExtent(strLine).cx);
				strAllGenes += strLine;
			}
		}
		LeaveCriticalSection(&m_csInfoIds);
		
		rcAllGenes = CRect(rcClient.right - maxLen - 10,0,rcClient.right,1);
		pDc->DrawText(strAllGenes,rcAllGenes,DT_CALCRECT);
	}

	ptCenter = rcClient.CenterPoint();

	int nRes = pDc->SaveDC();
	pDc->SetMapMode(MM_ISOTROPIC);

	pDc->SetViewportExt(rcClient.Size());
	pDc->SetWindowExt(6000,-6000);

	b2Vec2 pos = pDoc->m_car.getCenter();
	pos *= 100;
	pDc->SetWindowOrg((int)pos.x,(int)pos.y);
	pDc->SetViewportOrg(ptCenter.x,ptCenter.y);


	pDoc->m_car.Draw(pDc);

	CPen penGround(PS_SOLID,10,RGB(0,100,0));
	CBrush bshGround;
	bshGround.CreateSolidBrush(RGB(160,160,100));
	pDc->SelectObject(&penGround);
	pDc->SelectObject(&bshGround);

	// Chão:
	size_t k,nSize = g_vecGroundPoints.size();
	CPoint *pPoints = new CPoint[nSize+2];
	for(k = 0; k < nSize; k++)
	{
		pPoints[k+2].x = (LONG)(g_vecGroundPoints[k].x*100);
		pPoints[k+2].y = (LONG)(g_vecGroundPoints[k].y*100);
	}

	pPoints[0] = CPoint((int)(g_vecGroundPoints[nSize-1].x*100),-20000);
	pPoints[1] = CPoint((int)(g_vecGroundPoints[0].x*100)      ,-20000);

	pDc->Polygon(pPoints,(int)nSize+2);

	delete pPoints;

	pDc->FrameRect(CRect(-9900,-9900,9900,9900),&CBrush(RGB(255,0,0)));

	if(false)
	{
		pDc->MoveTo(0,-10000);
		pDc->LineTo(0, 10000);

		pDc->MoveTo(-10000,0);
		pDc->LineTo( 10000,0);
	}

	pDc->RestoreDC(nRes);
	pDc->SetTextColor(RGB(64,128,64));
	pDc->SetBkMode(TRANSPARENT);

	if(m_bShowInfoGaGenes)
	{
		pDc->DrawText(strAllGenes,rcAllGenes,0);
	}

	// Informações do GA:
	// Canto inferior esquerdo:
	if(true && m_bGaRunning)
	{
		CRect rcGaInfo(0,0,1,1);
		CString strGaInfo;
		pDc->SetTextColor(RGB(0,0,0));
		pDc->SelectObject(m_fntSmall);

		strGaInfo.Format(
			"Geração: %d\r\n"
			"Max(d,t,vm): (%.2f,%.2f,%.2f)\r\n"
			"Pts = f(v,d,c1,c2,t): %.4f = f(%.2f,%.2f,%.2f,%.2f,%.2f)\r\n"
			, m_nGeracao
			, m_ga._maxDistancia
			, m_ga._maxT
			, m_ga._maxVm
			, m_ga.m_carWinner.getPontuacao()
			, m_ga.m_carWinner.m_vm
			, m_ga.m_carWinner.m_distancia
			, m_ga.m_carWinner.m_bContactR1
			, m_ga.m_carWinner.m_bContactR2
			, m_ga.m_carWinner.m_t
			);
		pDc->DrawText(strGaInfo,rcGaInfo,DT_CALCRECT);
		int cy = rcGaInfo.Height();
		rcGaInfo.OffsetRect(0,rcClient.bottom - cy);
		pDc->DrawText(strGaInfo,rcGaInfo,0);
	}

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
}


// CGaBox2dView printing

BOOL CGaBox2dView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGaBox2dView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGaBox2dView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
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
		GetDocument()->m_car.beginSimulate(GetDocument()->m_pWorld);
		m_nSimTimer = (UINT)SetTimer((UINT_PTR)1001,10,NULL);
	}
	else
	{
		KillTimer(m_nSimTimer);
		m_nSimTimer = 0;
		GetDocument()->m_car.endSimulate();
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
		if(!pDoc->m_car.doStep())
		{
			OnSimulaPlay();
			break;
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

	pDoc->m_car.CreateCar();

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
	pDoc->m_car.getGenes(strGenes);

	if(m_nSimTimer != 0)
		OnSimulaPlay();

	pDoc->m_car.CreateCar(strGenes);

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
	CGaBox2dView* pVw = (CGaBox2dView*)pParam;

	pVw->m_bGaRunning = true;
	pVw->m_bGaExited = false;

	b2World *pWorld = CreateWorld(g_vecGroundPoints);


	pVw->m_ga.BeginEvolve();
	size_t i,nSize = pVw->m_ga.m_populacao.size();
	int nGeracao = 0;
	CCronometro cr;
	cr.Start();

	while(pVw->m_bGaRunning)
	{
		pVw->m_ga.Ordena(pWorld);

		// Alimentamos a lista de genes para ser mostradas para o usuário,
		// mas somente à cada 200us para não perder tempo à toa.
		if(cr.Get() > 200)
		{
			EnterCriticalSection(&pVw->m_csInfoIds);
			{
				pVw->m_ga.m_populacao[0].getGenes(pVw->m_strMelhor);
				pVw->m_ga.m_populacao[rand()%nSize].getGenes(pVw->m_strQualquer);
				pVw->m_nGeracao = nGeracao;

				pVw->m_vecInfoIds.clear();
				id_info_t ii;
				for(i = 0; i < nSize; i++)
				{
					pVw->m_ga.m_populacao[i].getGenes(ii.strGenes);
					ii.dPoints = pVw->m_ga.m_populacao[i].getPontuacao();
					pVw->m_vecInfoIds.push_back(ii);
				}
			}
			LeaveCriticalSection(&pVw->m_csInfoIds);

			if(pVw->m_bGaRunning) pVw->Invalidate();

			// Reiniciamos a contagem do tempo
			cr.Start();
		}

		pVw->m_ga.Select();
		pVw->m_ga.Crossover();
		pVw->m_ga.Mutate();
		pVw->m_ga.AdvanceGeneration();

		nGeracao++;
	}

	delete pWorld;
	pVw->m_bGaExited = true;
}

void CGaBox2dView::OnGaIniciarga()
{
	CGaParamsDlg dlgParams;
	if(dlgParams.DoModal() != IDOK)
	{
		return;
	}
	double cross,mut;
	dlgParams.m_strMutacao.Replace(",",".");
	dlgParams.m_strCrossover.Replace(",",".");

	cross = atof(dlgParams.m_strCrossover);
	mut   = atof(dlgParams.m_strMutacao);

	m_ga.setParams(dlgParams.m_nPopulacao,dlgParams.m_nElitismo,cross,mut,dlgParams.m_nAlienismo,dlgParams.m_nMutInt,dlgParams.m_dMaxT);

	_beginthread(fnGa,0,(void *)this);


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

	if(m_nSimTimer != 0)
		OnSimulaPlay();

	pDoc->m_car.CreateCar(buffer);

	if(m_nSimTimer == 0)
		OnSimulaPlay();
}

void CGaBox2dView::OnUpdateGaIniciarga(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bGaRunning);
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

	EnterCriticalSection(&m_csInfoIds);
	{
		pDoc->m_car.CreateCar(m_strMelhor);
	}
	LeaveCriticalSection(&m_csInfoIds);


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

	EnterCriticalSection(&m_csInfoIds);
	{
		pDoc->m_car.CreateCar(m_strQualquer);
	}
	LeaveCriticalSection(&m_csInfoIds);


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
	m_ga.MassExtintionEvent();
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
	m_ga.IncludeId(buffer);
}

void CGaBox2dView::OnUpdateGaExtins(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bGaRunning);
}

void CGaBox2dView::OnUpdateGaColar(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bGaRunning);
}
