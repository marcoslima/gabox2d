#include "GrCar.h"

#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include "Pen.h"
#include "SolidBrush.h"


namespace GUI
{
    CGrCar::CGrCar() = default;

    CGrCar::~CGrCar() = default;

    void DrawLine(sf::RenderWindow &window,
                  const sf::Vector2f p1,
                  const sf::Vector2f p2,
                  const CPen &pen)
    {
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = p1;
        line[0].color = pen.getColor();
        line[1].position = p2;
        line[1].color = pen.getColor();

        window.draw(line);
    }

    void DrawRoda(sf::RenderWindow &window,
                  const CGrCar::circle_t &c,
                  float angle,
                  const CPen &pen,
                  const CSolidBrush &brush)
    {
        //	RectF rcBall(c.c.X-c.r,c.c.Y-c.r,2*c.r,2*c.r);
        //	pGr->FillEllipse(pBrush,rcBall);
        //	pGr->DrawEllipse(pPen,rcBall);
        //
        //	if(angle < 2*M_PI)
        //		pGr->DrawLine(pPen,c.c,PointF(c.c.X + c.r * cos(angle), c.c.Y + c.r * sin(angle)));
        sf::CircleShape circle_shape(c.r);
        // circle_shape.setPosition(center - WorldToLogical(b2Vec2(r, r)));
        pen.apply(circle_shape);
        brush.apply(circle_shape);
    }

    void CGrCar::Draw(sf::RenderWindow &window) const
    {
        // Rodas 1 e 2
        const CSolidBrush bshRoda(sf::Color(128, 128, 128));
        const CSolidBrush bshRodaC(sf::Color(255, 255, 255));

        const CPen penRoda(sf::Color(64, 64, 64), 0.3);
        const CPen penRodaC(sf::Color(0, 0, 0), 0.3);

        // Roda 1
        const CSolidBrush *pBsh = (_roda1.touch) ? &bshRodaC : &bshRoda;
        const CPen *pPen = (_roda1.touch) ? &penRodaC : &penRoda;
        DrawRoda(window, _roda1.c, _roda1.angle, *pPen, *pBsh);

        // Roda 2
        pBsh = (_roda2.touch) ? (&bshRodaC) : (&bshRoda);
        pPen = (_roda2.touch) ? (&penRodaC) : (&penRoda);
        DrawRoda(window, _roda2.c, _roda2.angle, *pPen, *pBsh);

        // Pesos 1 e 2
        const CSolidBrush bshNull(sf::Color(0, 0, 0, 0));
        const CPen penPeso(sf::Color(255, 0, 0), 0.3);
        // penPeso.SetDashStyle(DashStyleDot);

        DrawRoda(window, _peso1.c, 3 * M_PI, penPeso, bshNull);
        DrawRoda(window, _peso2.c, 3 * M_PI, penPeso, bshNull);

        // Joints:
        const CPen penJoint(sf::Color(200, 200, 200), 0.3);

        if (!_broke)
        {
            DrawLine(window, _peso1.c.c, _peso2.c.c, penJoint);
            DrawLine(window, _roda1.c.c, _peso2.c.c, penJoint);
            DrawLine(window, _roda2.c.c, _peso1.c.c, penJoint);
            DrawLine(window, _roda1.c.c, _roda2.c.c, penJoint);
            DrawLine(window, _roda1.c.c, _peso1.c.c, penJoint);
            DrawLine(window, _roda2.c.c, _peso2.c.c, penJoint);
        }


        // Centro de massa:
        CPen penCm(sf::Color(0, 0, 0), 0);

        DrawLine(window,PointF(_cm.x - 0.5, _cm.y - 0.5),
                 PointF(_cm.x + 0.5, _cm.y + 0.5), penCm);
        DrawLine(window,PointF(_cm.x - 0.5, _cm.y + 0.5),
                 PointF(_cm.x + 0.5, _cm.y - 0.5), penCm);

        sf::CircleShape shapeCm(0.5);
        shapeCm.setPosition(_cm.x, _cm.y);
        penCm.apply(shapeCm);
        window.draw(shapeCm);

        shapeCm.setRadius(0.8f);
        window.draw(shapeCm);
    }
}; //namespace GUI
