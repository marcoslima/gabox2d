//
// Created by marcos on 11/3/24.
//

#ifndef CSOLIDBRUSH_H
#define CSOLIDBRUSH_H

#include <SFML/Graphics.hpp>

class CSolidBrush {
private:
    sf::Color m_color;
public:
    explicit CSolidBrush(const sf::Color color) : m_color(color) {}
    ~CSolidBrush() = default;

    void setColor(const sf::Color color) { m_color = color; }

    void apply(sf::Shape& shape) const {
        shape.setFillColor(m_color);
    }
};



#endif //CSOLIDBRUSH_H
