#pragma once
#include <SFML/Graphics.hpp>
#include <imgui-sfml/imgui-SFML.h>


class CGenomaImage
{
    sf::Texture _texture;
    sf::Sprite _sprite;

public:
    CGenomaImage(unsigned int width);
    ~CGenomaImage() = default;

    void set(const std::string& genes);
    void render();
};