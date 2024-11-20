#include "GrCar.h"

#include <cmath>
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include "Pen.h"
#include "SolidBrush.h"

namespace GUI
{
    CGrCar::CGrCar() = default;

    CGrCar::~CGrCar() = default;

    void DrawTickLine(sf::RenderWindow &window,
                      const sf::Vector2f p1,
                      const sf::Vector2f p2,
                      const CPen &pen)
    {
        const auto line_length = static_cast<float>(sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)));
        const auto line_height = pen.getWidth();
        const auto angle = static_cast<float>(atan2(p2.y - p1.y, p2.x - p1.x));

        sf::RectangleShape line(sf::Vector2f(line_length, line_height));
        line.setOutlineColor(sf::Color::Transparent);
        line.setOutlineThickness(0);
        CSolidBrush(pen.getColor()).apply(line);
        line.setOrigin(line_length / 2.0f, line_height / 2.0f);
        line.setPosition((p1 + p2) / 2.0f);
        line.rotate(angle * 180.0f / static_cast<float>(M_PI));
        window.draw(line);
    }

    void DrawLine(sf::RenderWindow &window,
                  const sf::Vector2f p1,
                  const sf::Vector2f p2,
                  const sf::Color color)
    {
        // Draw a line from p1 to p2 with color
        const sf::Vertex line[] = {
            sf::Vertex(p1, color),
            sf::Vertex(p2, color)
        };
        window.draw(line, 2, sf::Lines);
    }

    void DrawRoda(sf::RenderWindow &window,
                  const CGrCar::gr_circle_t &c,
                  const float angle,
                  const CPen &pen,
                  const CSolidBrush &brush,
                  const bool draw_angle = true)
    {
        sf::CircleShape circle_shape(c.radius);
        circle_shape.setPosition(c.center.x - c.radius, c.center.y - c.radius); // Position é canto superior esquerdo.
        pen.apply(circle_shape);
        brush.apply(circle_shape);
        window.draw(circle_shape);

        if(!draw_angle) return;

        DrawTickLine(window,
            c.center, c.center + sf::Vector2f(c.radius * cos(angle),
                c.radius * sin(angle)),
                pen);
    }

    void CGrCar::Draw(sf::RenderWindow &window) const
    {
        // Rodas 1 e 2
        const CSolidBrush bshRoda(sf::Color(128, 128, 128));
        const CSolidBrush bshRodaC(sf::Color(255, 255, 255));

        const CPen penRoda(sf::Color(64, 64, 64), 0.2);
        const CPen penRodaC(sf::Color(0, 0, 0), 0.2);

        // Roda 1
        const CSolidBrush *pBsh = (_roda1.touch) ? &bshRodaC : &bshRoda;
        const CPen *pPen = (_roda1.touch) ? &penRodaC : &penRoda;
        DrawRoda(window, _roda1.circle, _roda1.angle, *pPen, *pBsh);

        // Roda 2
        pBsh = (_roda2.touch) ? (&bshRodaC) : (&bshRoda);
        pPen = (_roda2.touch) ? (&penRodaC) : (&penRoda);
        DrawRoda(window, _roda2.circle, _roda2.angle, *pPen, *pBsh);

        // Pesos 1 e 2
        const CSolidBrush bshNull(sf::Color(0, 0, 0, 0));
        const CPen penPeso(sf::Color(255, 0, 0), 0.2);
        // penPeso.SetDashStyle(DashStyleDot);

        DrawRoda(window, _peso1.circle, 3 * M_PI, penPeso, bshNull, false);
        DrawRoda(window, _peso2.circle, 3 * M_PI, penPeso, bshNull, false);

        // Joints:
        const CPen penJoint(sf::Color(150, 150, 150), 0.2f);

        if (!_broke)
        {
            DrawTickLine(window, _peso1.circle.center, _peso2.circle.center, penJoint);
            DrawTickLine(window, _roda1.circle.center, _peso2.circle.center, penJoint);
            DrawTickLine(window, _roda2.circle.center, _peso1.circle.center, penJoint);
            DrawTickLine(window, _roda1.circle.center, _roda2.circle.center, penJoint);
            DrawTickLine(window, _roda1.circle.center, _peso1.circle.center, penJoint);
            DrawTickLine(window, _roda2.circle.center, _peso2.circle.center, penJoint);
        }


        // Centro de massa:
        CPen penCm(sf::Color(0, 0, 0), 0.1);
        sf::Color crCm(0, 0, 0);
        CSolidBrush bshCm(sf::Color::Transparent);

        DrawLine(window,
                 PointF(_cm.x - 0.5f, _cm.y - 0.5f),
                 PointF(_cm.x + 0.5f, _cm.y + 0.5f), crCm);
        DrawLine(window,
                 PointF(_cm.x - 0.5f, _cm.y + 0.5f),
                 PointF(_cm.x + 0.5f, _cm.y - 0.5f), crCm);

        sf::CircleShape shapeCm(0.5f);
        shapeCm.setPosition(_cm.x - .5f, _cm.y - .5f);
        penCm.apply(shapeCm);
        bshCm.apply(shapeCm);
        window.draw(shapeCm);

        shapeCm.setRadius(0.8f);
        shapeCm.setPosition(_cm.x - .8f, _cm.y - .8f);
        window.draw(shapeCm);
    }
}
