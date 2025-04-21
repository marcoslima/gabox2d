#include <genoma_image.h>
#include <imgui.h>


CGenomaImage::CGenomaImage(const unsigned int width)
{
    _texture.create(width, 1);
    _sprite.setTexture(_texture);
}

void CGenomaImage::set(const std::string &genes)
{
    const auto width = genes.size();
    if (width == 0) return;
    std::vector<sf::Uint8> pixels;
    pixels.reserve(width * 4);
    for (const auto &c : genes)
    {
        const auto value = c == '1' ? 0xff : 0x0;
        pixels.push_back(value);
        pixels.push_back(value);
        pixels.push_back(value);
        pixels.push_back(0xff);
    }
    _texture.update(pixels.data(), width, 1, 0, 0);
    _sprite.setTexture(_texture, true);
    _sprite.setScale(1.0f, 12.0f);
}

void CGenomaImage::render()
{
    ImGui::Image(_sprite);
}
