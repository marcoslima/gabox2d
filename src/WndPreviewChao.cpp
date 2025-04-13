// WndPreviewChao.cpp : implementation file
//

#include "WndPreviewChao.h"

#include <SFML/Window/Event.hpp>

#include "lmmath.h"
using namespace lmmath;

namespace GUI
{
    // CWndPreviewChao


    CWndPreviewChao::CWndPreviewChao()
    {
        const string name = "Environment preview";
        m_window.create(sf::VideoMode(800, 600),
                        name,
                        sf::Style::Titlebar);
        m_window.setVisible(false);
        m_ptCenter = CPoint(0, 0);
        m_vecTl = b2Vec2(-1, -1);
        m_vecBr = b2Vec2(1, 1);
        m_zoom = 1.0;
    }

    void CWndPreviewChao::flush()
    {
        sf::Event event;
        while (m_window.pollEvent(event)) {}
        m_window.clear(sf::Color::Blue);
        m_window.display();
    }

    void CWndPreviewChao::show()
    {
        m_window.setVisible(true);
    }

    CWndPreviewChao::~CWndPreviewChao() {}


    // CWndPreviewChao message handlers

#define sign(a) ((a >= 0)?(1):(-1))

    /*
        Por exemplo, um espa�o orientado com y positivo para cima e x positivo para a direita

        Um ret�ngulo de altura negativa seria:

        -100,100

                    100,-100
        x,y = (-100, 100)
        w,h = ( 200,-200)

        Para normalizar o retângulo ele deve ser descrito posicionado nas coordenadas
        menores, e ter largura e altura positivas.

        Assim, o correto é posicioná-lo em -100,-100 com 200,200
        Então o y do exemplo deve ser adicionado da altura negativa
        e então a altura passa à ser positiva
    */
//
// void NormalizeRect(Rect& rc)
// {
// 	if(rc.Width < 0)
// 	{
// 		rc.X += rc.Width;
// 		rc.Width *= -1.0;
// 	}
// 	if(rc.Height < 0)
// 	{
// 		rc.Y += rc.Height;
// 		rc.Height *= -1.0;
// 	}
// }
//
// Point CenterPoint(const Rect& rc)
// {
// 	return Point(rc.GetLeft() + rc.Width  / 2.0,
// 				 rc.GetTop()  + rc.Height / 2.0);
// }
//
// void CWndPreviewChao::OnPaint()
// {
// 	CPaintDC dc(this);
// 	CDcIntegrity dci(&dc);
//
// 	Graphics gr(dc.m_hDC);
//
// 	Rect rcClient;
// 	Point ptScrCenter;
// 	{
// 		CRect rcc;
// 		CPoint ptc;
// 		GetClientRect(rcc);
// 		ptc = rcc.CenterPoint();
//
// 		rcClient = Rect(rcc.left,rcc.top,rcc.right,rcc.bottom);
// 		ptScrCenter = Point(ptc.x,ptc.y);
// 	}
// 	Rect rcWorld(m_vecTl.x,m_vecBr.y,m_vecBr.x-m_vecTl.x,m_vecTl.y-m_vecBr.y);
//
// 	gr.SetClip(rcClient,CombineModeReplace);
//
// 	gr.FillRectangle(&SolidBrush(Color(0,0,0)),rcClient);
//
// 	// Ajustamos a transforma��o:
// 	// A transforma��o ser� tal que
// 	// T(x,y) = (a.x + b, c.y + d)
// 	// onde (x,y) est� em world coordinates (wc) e
// 	// T(x,y) est� em device coordinates (dc)
// 	//
// 	// sabemos as transforma��es dos seguintes pontos:
// 	// T(0,0) = (pc.x,pc.y) -> b = pc.x, d = pc.y
// 	// T(tl.x,tl.y) = (rc.l,rc.t)
// 	// T(br.x,br.y) = (rc.r,rc.b)
// 	//
// 	// a.tl_x + pc_x = rc_l
// 	// a.br_x + pc_x = rc.r
// 	//-----------------------
// 	// a(tl_x-br_x) = (rc_l-rc.r)
// 	//
// 	//      (rc_l-rc_r)
// 	// a = -------------
// 	//      (tl_x-br_x)
// 	//
// 	//      (rc_t-rc_b)
// 	// b = -------------
// 	//      (tl_y-br_y)
// 	////////////////////////////////////////////////////////////
//
// 	double dZoom = pow(1.3,(double)m_zoom);
//
// 	// De -100 � 100 (do slider ctrl) a vizualiza��o
// 	// dever� ir de -width � width
// 	int cx,cy;
// 	cx = ptScrCenter.X - m_ptCenter.x * rcWorld.Width / 1000.0;
// 	cy = ptScrCenter.Y - m_ptCenter.y * rcWorld.Height / 1000.0;
// 	cx *= dZoom;
// 	cy *= dZoom;
//
// 	Matrix mt( dZoom	,	0		,
// 		       0		,	-dZoom	,
// 			   cx		,	cy		);
// 	gr.SetTransform(&mt);
//
// 	// World na cor de c�u
// 	SolidBrush bshSky(Color(100,100,255));
// 	gr.FillRectangle(&bshSky,rcWorld);
//
// 	vec_vecs_t vecGround = m_env.get_vecs();
// 	size_t i,nSize = vecGround.size();
// 	if(nSize == 0)
// 		return;
//
// 	PointF *pPoints = new PointF[nSize];
// 	for(i = 0; i < nSize; i++)
// 	{
// 		pPoints[i].X = vecGround[i].x;
// 		pPoints[i].Y = vecGround[i].y;
// 	}
//
// 	Pen penGround(Color(0,0,0));
// 	SolidBrush bshGround(Color(32,128,32));
// 	gr.FillPolygon(&bshGround,pPoints,nSize);
// 	gr.DrawPolygon(&penGround,pPoints,nSize);
// 	delete pPoints;
//
// 	Pen penBorder(Color(255,0,0),5);
// 	gr.DrawRectangle(&penBorder,rcWorld);
//
// 	Pen penAxis(Color(255,0,0),0);
// 	gr.DrawLine(&penAxis,0,-10,0,10);
// 	gr.DrawLine(&penAxis,-10,0,10,0);
// }
//
// void CWndPreviewChao::SetCenter(int x, int y)
// {
// 	m_ptCenter = CPoint(x,y);
// }

    void dummy()
    {
        unsigned int screenWidth = 800;
        unsigned int screenHeight = 600;
        sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Environment preview");
        window.setFramerateLimit(60);

        while (window.isOpen())
        {
            // sf::Event event;

            window.clear();

            // VIEW.DRAW

            // VIEW.UPDATE
            window.display();
        }
    }
}