#if 0

// WndShowCarro.cpp : implementation file
//

#include "stdafx.h"
#include "GaBox2d.h"
#include "WndShowCarro.h"
#include ".\wndshowcarro.h"
using namespace Gdiplus;

namespace GUI
{
// CWndShowCarro

IMPLEMENT_DYNAMIC(CWndShowCarro, CStatic)
CWndShowCarro::CWndShowCarro()
{
}

CWndShowCarro::~CWndShowCarro()
{
}


BEGIN_MESSAGE_MAP(CWndShowCarro, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CWndShowCarro message handlers


void CWndShowCarro::OnPaint()
{
	CPaintDC dc(this);

	Graphics gr(dc.m_hDC);

	Rect rcClient;
	PointF ptScrCenter; 
	{
		CRect rcc;
		CPoint ptc;
		GetClientRect(rcc);
		ptc = rcc.CenterPoint();

		rcClient = Rect(rcc.left,rcc.top,rcc.right,rcc.bottom);
		ptScrCenter = PointF((Gdiplus::REAL)ptc.x,(Gdiplus::REAL)ptc.y);
	}

	gr.SetClip(rcClient,CombineModeReplace);

	SolidBrush bshSky(Color(100,100,255));
	gr.FillRectangle(&bshSky,rcClient);

	Gdiplus::REAL dZoom = 10.0;

	Matrix mt( dZoom	,	0		,
		       0		,	-dZoom	,
			   (Gdiplus::REAL)ptScrCenter.X,	(Gdiplus::REAL)ptScrCenter.Y		);
	gr.SetTransform(&mt);

	// TODO: reativar o drawoffline
	//m_Carro.DrawOffline(&gr);
}
};//namespace GUI

#endif