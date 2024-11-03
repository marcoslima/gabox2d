//
// Created by marcos on 11/3/24.
//

#ifndef CPEN_H
#define CPEN_H

#include <SFML/Graphics.hpp>

class CPen {
private:
    sf::Color m_color;
    float m_width;

public:
    CPen(const sf::Color color, const float width) : m_color(color), m_width(width) {}
    ~CPen() = default;

    void setColor(const sf::Color color) { m_color = color; }
    void setWidth(const float width) { m_width = width; }
    [[nodiscard]] sf::Color getColor() const { return m_color; }

    void apply(sf::Shape& shape) const {
        shape.setOutlineColor(m_color);
        shape.setOutlineThickness(m_width);
    }
};



#endif //CPEN_H
