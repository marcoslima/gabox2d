#include "GrCar.h"

#include <cmath>
#include <random>
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include "Pen.h"
#include "SolidBrush.h"
#include "assets.h"
#include <sfml_primitives.h>

namespace GUI
{
    CGrCar::CGrCar() = default;

    CGrCar::~CGrCar() = default;

    void CGrCar::draw(void *pParams) const
    {
        Draw(*static_cast<sf::RenderWindow *>(pParams));
    }

    void fill_circle(CGrCar::gr_circle_t &circle, const float center_x, const float center_y, const float radius)
    {
        circle.center.x = center_x;
        circle.center.y = center_y;
        circle.radius = radius;
    }

    void fill_roda(CGrCar::gr_roda_t &roda, const float center_x, const float center_y, const float radius,
                   const float angle, const bool touch, const float friction, const float density,
                   const float restitution)
    {
        fill_circle(roda.circle, center_x, center_y, radius);
        roda.angle = angle;
        roda.touch = touch;
        roda.friction = friction;
        roda.density = density;
        roda.restitution = restitution;
    }

    void fill_peso(CGrCar::gr_peso_t &peso, const float center_x, const float center_y, const float radius,
                   const bool broke)
    {
        fill_circle(peso.circle, center_x, center_y, radius);
        peso.broke = broke;
    }

    void CGrCar::setRoda1(const float center_x, const float center_y, const float radius, const float angle,
                          const bool touch, const float friction, const float density, const float restitution)
    {
        fill_roda(_roda1, center_x, center_y, radius, angle, touch, friction, density, restitution);
    }

    void CGrCar::setRoda2(const float center_x, const float center_y, const float radius, const float angle,
                          const bool touch, const float friction, const float density, const float restitution)
    {
        fill_roda(_roda2, center_x, center_y, radius, angle, touch, friction, density, restitution);
    }

    void CGrCar::setPeso1(const float center_x, const float center_y, const float radius, const bool broke)
    {
        fill_peso(_peso1, center_x, center_y, radius, broke);
    }

    void CGrCar::setPeso2(const float center_x, const float center_y, const float radius, const bool broke)
    {
        fill_peso(_peso2, center_x, center_y, radius, broke);
    }

    void CGrCar::setCenter(const float center_x, const float center_y)
    {
        _cm.x = center_x;
        _cm.y = center_y;
    }

    void CGrCar::setBroke(const bool broke)
    {
        _broke = broke;
    }

    void CGrCar::setShowRodaParams(const bool show)
    {
        _showRodaParams = show;
    }

    gr_car_ptr_t CGrCar::clone()
    {
        auto car = std::make_shared<CGrCar>();
        car->_roda1 = _roda1;
        car->_roda2 = _roda2;
        car->_peso1 = _peso1;
        car->_peso2 = _peso2;
        car->_cm = _cm;
        car->_broke = _broke;
        return car;
    }

    std::pair<sf::Vector2f, sf::Vector2f> calc_tick_line_points(const CGrCar::gr_circle_t &c, const float &angle)
    {
        const auto p1 = c.center;
        const auto vx = c.radius * cos(angle);
        const auto vy = c.radius * sin(angle);
        const auto v = sf::Vector2f(vx, vy);
        const auto p2 = p1 + v;
        return std::make_pair(p1, p2);
    }

    void DrawRoda(sf::RenderWindow &window,
                  const CGrCar::gr_circle_t &c,
                  const float angle,
                  const CPen &pen,
                  const CSolidBrush &brush,
                  const bool draw_angle = true)
    {
        const auto radius = c.radius - pen.getWidth();
        sf::CircleShape circle_shape(radius);
        circle_shape.setPosition(c.center.x - radius, c.center.y - radius); // Position é canto superior esquerdo.
        pen.apply(circle_shape);
        brush.apply(circle_shape);
        window.draw(circle_shape);

        if (!draw_angle) return;

        const auto points = calc_tick_line_points(c, angle);
        DrawTickLine(window, points.first, points.second, pen);
    }

    void DrawPeso(sf::RenderWindow &window,
                  const CGrCar::gr_circle_t &c,
                  const CPen &pen)
    {
        DrawDashedCircle(window, c.center, c.radius, pen, 0.2f, 0.2f);
    }

    void CGrCar::drawRodaParams(sf::Text &text, const gr_roda_t &roda, sf::RenderWindow &window)
    {
        constexpr auto xfactor = 1.3f;
        text.setFillColor(sf::Color::White);
        text.setPosition(roda.circle.center.x + roda.circle.radius * xfactor, roda.circle.center.y + 1);
        text.setString("f: " + std::to_string(roda.friction));
        window.draw(text);

        text.setPosition(roda.circle.center.x + roda.circle.radius * xfactor, roda.circle.center.y + 0.5f);
        text.setString("d: " + std::to_string(roda.density));
        window.draw(text);

        text.setPosition(roda.circle.center.x + roda.circle.radius * xfactor, roda.circle.center.y + 0);
        text.setString("r: " + std::to_string(roda.restitution));
        window.draw(text);
    }

    void CGrCar::Draw(sf::RenderWindow &window) const
    {
        // Rodas 1 e 2
        const CSolidBrush bshRoda(sf::Color(128, 128, 128));
        const CSolidBrush bshRodaC(sf::Color(255, 255, 255));

        const CPen penRoda(sf::Color(64, 64, 64), 0.2);
        const CPen penRodaC(sf::Color(0, 0, 0), 0.2);

        auto text_font = sf::Font();
        // load it from linux file:
        text_font.loadFromFile(TTF_FONT_FILE);
        sf::Text text;
        text.setFont(text_font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        text.setScale(0.05f, -0.05f);
        text.setOrigin(12, 12);

        // Roda 1
        const CSolidBrush *pBsh = _roda1.touch ? &bshRodaC : &bshRoda;
        const CPen *pPen = _roda1.touch ? &penRodaC : &penRoda;
        DrawRoda(window, _roda1.circle, _roda1.angle, *pPen, *pBsh);

        // Roda 2
        pBsh = _roda2.touch ? &bshRodaC : &bshRoda;
        pPen = _roda2.touch ? &penRodaC : &penRoda;
        DrawRoda(window, _roda2.circle, _roda2.angle, *pPen, *pBsh);

        // Pesos 1 e 2
        const CPen penPeso(sf::Color(255, 0, 0), 0.2);

        DrawPeso(window, _peso1.circle, penPeso);
        DrawPeso(window, _peso2.circle, penPeso);

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
        CPen penCm(sf::Color(0, 0, 0), 0.08);
        sf::Color crCm(0, 0, 0);
        CSolidBrush bshCm(sf::Color::Transparent);
        constexpr float radius1 = 0.3f;
        constexpr float radius2 = 0.5f;

        DrawLine(window,
                 sf::Vector2f(_cm.x - radius2, _cm.y - radius2),
                 sf::Vector2f(_cm.x + radius2, _cm.y + radius2), crCm);
        DrawLine(window,
                 sf::Vector2f(_cm.x - radius2, _cm.y + radius2),
                 sf::Vector2f(_cm.x + radius2, _cm.y - radius2), crCm);

        sf::CircleShape shapeCm(radius1);
        shapeCm.setPosition(_cm.x - radius1, _cm.y - radius1);
        penCm.apply(shapeCm);
        bshCm.apply(shapeCm);
        window.draw(shapeCm);

        shapeCm.setRadius(radius2);
        shapeCm.setPosition(_cm.x - radius2, _cm.y - radius2);
        window.draw(shapeCm);

        text.setCharacterSize(10);

        if (_showRodaParams)
        {
            drawRodaParams(text, _roda1, window);
            drawRodaParams(text, _roda2, window);
        }
    }
}
