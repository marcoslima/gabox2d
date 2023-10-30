// WndPreviewChao.cpp : implementation file
//

#include "stdafx.h"
#include "GaBox2d.h"
#include "WndPreviewChao.h"
#include <float.h>
#include <DcIntegrity.h>


// CWndPreviewChao

IMPLEMENT_DYNAMIC(CWndPreviewChao, CWnd)
CWndPreviewChao::CWndPreviewChao()
{
}

CWndPreviewChao::~CWndPreviewChao()
{
}


BEGIN_MESSAGE_MAP(CWndPreviewChao, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CWndPreviewChao message handlers


void CWndPreviewChao::OnPaint()
{
	CPaintDC dc(this);
	CDcIntegrity dci(&dc);

	CRect rcClient;
	GetClientRect(rcClient);

	CBrush bshSky;
	bshSky.CreateSolidBrush(RGB(100,100,255));
	dc.SelectObject(&bshSky);

	dc.Rectangle(rcClient);

	CRect rcClip(rcClient);
	rcClip.DeflateRect(1,1);
	dc.BeginPath();
	dc.MoveTo(rcClip.TopLeft());
	dc.LineTo(rcClip.right,rcClip.top);
	dc.LineTo(rcClip.right,rcClip.bottom);
	dc.LineTo(rcClip.left,rcClip.bottom);
	dc.LineTo(rcClip.TopLeft());
	dc.EndPath();
	dc.SelectClipPath(RGN_COPY);

	dc.SetMapMode(MM_ISOTROPIC);

	dc.SetViewportExt(rcClient.Size());
	dc.SetWindowExt(6000,-6000);
	dc.SetWindowOrg(0,0);
	dc.SetViewportOrg(rcClient.CenterPoint());


	size_t i,nSize = m_vecGround.size();
	if(nSize == 0)
		return;
	int fx,fy;
	fx = 100;
	fy = 100;

	CPoint *pPoints = new CPoint[nSize+2];
	for(i = 0; i < nSize; i++)
	{
		pPoints[i+2].x = (LONG)(m_vecGround[i].x*fx);
		pPoints[i+2].y = (LONG)(m_vecGround[i].y*fy);
	}

	pPoints[0] = CPoint((int)(m_vecGround[nSize-1].x*fx),-200*fy);
	pPoints[1] = CPoint((int)(m_vecGround[      0].x*fx),-200*fy);

	CBrush bshGround;
	bshGround.CreateSolidBrush(RGB(200,255,200));
	dc.SelectObject(&bshGround);

	dc.Polygon(pPoints,(int)nSize+2);

	delete pPoints;
}
