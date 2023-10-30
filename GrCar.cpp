#include "StdAfx.h"
#include ".\grcar.h"
namespace GUI
{
CGrCar::CGrCar(void)
{
}

CGrCar::~CGrCar(void)
{
}

void DrawRoda(Graphics* pGr, CGrCar::circle_t& c, double angle, Pen* pPen, SolidBrush *pBrush)
{
	RectF rcBall(c.c.X-c.r,c.c.Y-c.r,2*c.r,2*c.r);
	pGr->FillEllipse(pBrush,rcBall);
	pGr->DrawEllipse(pPen,rcBall);

	if(angle < 2*M_PI)
		pGr->DrawLine(pPen,c.c,PointF(c.c.X + c.r * cos(angle), c.c.Y + c.r * sin(angle)));
}

void CGrCar::Draw(Graphics* pGr)
{
	// Rodas 1 e 2
	SolidBrush	bshRoda(Color(128,128,128)),
				bshRodaC(Color(255,255,255));
	
	Pen penRoda (Color(64,64,64),0.3),
		penRodaC(Color(0,0,0),0.3);

	SolidBrush *pBsh;
	Pen *pPen;

	// Roda 1
	pBsh = (_roda1.touch)?(&bshRodaC):(&bshRoda);
	pPen = (_roda1.touch)?(&penRodaC):(&penRoda);
	DrawRoda(pGr,_roda1.c,_roda1.angle,pPen,pBsh);

	// Roda 2
	pBsh = (_roda2.touch)?(&bshRodaC):(&bshRoda);
	pPen = (_roda2.touch)?(&penRodaC):(&penRoda);
	DrawRoda(pGr,_roda2.c,_roda2.angle,pPen,pBsh);

	// Pesos 1 e 2
	SolidBrush bshNull(Color(0,0,0,0));
	Pen penPeso(Color(255,0,0),0.3);
	penPeso.SetDashStyle(DashStyleDot);

	DrawRoda(pGr,_peso1.c,3*M_PI,&penPeso,&bshNull);
	DrawRoda(pGr,_peso2.c,3*M_PI,&penPeso,&bshNull);

	// Joints:
	Pen penJoint(Color(200,200,200),0.3);

	if(!_broke)
	{
		pGr->DrawLine(&penJoint,_peso1.c.c,_peso2.c.c);
		pGr->DrawLine(&penJoint,_roda1.c.c,_peso2.c.c);
		pGr->DrawLine(&penJoint,_roda2.c.c,_peso1.c.c);
		pGr->DrawLine(&penJoint,_roda1.c.c,_roda2.c.c);
		pGr->DrawLine(&penJoint,_roda1.c.c,_peso1.c.c);
		pGr->DrawLine(&penJoint,_roda2.c.c,_peso2.c.c);
	}


	// Centro de massa:
	Pen penCm(Color(0,0,0),0);

	PointF ptCm = _cm;
	pGr->DrawLine(&penCm,PointF(ptCm.X-0.5,ptCm.Y-0.5),
						 PointF(ptCm.X+0.5,ptCm.Y+0.5));
	pGr->DrawLine(&penCm,PointF(ptCm.X-0.5,ptCm.Y+0.5),
						 PointF(ptCm.X+0.5,ptCm.Y-0.5));

	RectF rcCm(ptCm.X-.25,ptCm.Y-.25,.5,.5);
	pGr->DrawEllipse(&penCm,rcCm);
	rcCm.Inflate(.3,.3);
	pGr->DrawEllipse(&penCm,rcCm);
}

};//namespace GUI