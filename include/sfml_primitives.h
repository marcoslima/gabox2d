#pragma once

#include <SFML/Graphics.hpp>
#include <Pen.h>
#include <SolidBrush.h>

void DrawDashedCircle(sf::RenderWindow& window, const sf::Vector2f& center,
                     float radius, const CPen &pen,
                     float dashLength = 0.5f, float gapLength = 0.5f);

void DrawTickLine(sf::RenderWindow &window,
                  const sf::Vector2f p1,
                  const sf::Vector2f p2,
                  const CPen &pen);

void DrawLine(sf::RenderWindow &window,
              const sf::Vector2f p1,
              const sf::Vector2f p2,
              const sf::Color color);
