#include <cmath>
#include <sfml_primitives.h>


void DrawDashedCircle(sf::RenderWindow& window, const sf::Vector2f& center,
                     const float radius, const CPen &pen,
                     const float dashLength, const float gapLength)
{
    constexpr float doublepi = 2.0f * static_cast<float>(M_PI);

    // Calculate the circumference and the number of segments
    const float circumference = doublepi * radius;

    float currentAngle = 0.0f;

    // Calculate how much angle each dash/gap covers
    const float dashAngle = dashLength / circumference * doublepi;
    const float gapAngle = gapLength / circumference * doublepi;

    // Draw the dashed circle
    bool isDash = true;

    while (currentAngle < doublepi)
    {
        float nextAngle;

        if (isDash)
            nextAngle = currentAngle + dashAngle;
        else
            nextAngle = currentAngle + gapAngle;

        if (nextAngle > doublepi)
            nextAngle = doublepi;

        if (isDash)
        {
            const auto p1 = center + sf::Vector2f(radius * cos(currentAngle), radius * sin(currentAngle));
            const auto p2 = center + sf::Vector2f(radius * cos(nextAngle), radius * sin(nextAngle));

            DrawTickLine(window, p1, p2, pen);
        }

        currentAngle = nextAngle;
        isDash = !isDash;
    }
}

void DrawTickLine(sf::RenderWindow &window,
                  const sf::Vector2f p1,
                  const sf::Vector2f p2,
                  const CPen &pen)
{
    const auto line_length = static_cast<float>(sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)));
    const auto line_height = pen.getWidth();
    const auto angle = atan2(p2.y - p1.y, p2.x - p1.x);

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
