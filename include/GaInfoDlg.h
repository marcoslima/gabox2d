#pragma once

#include <IPanel.h>
#include <ga_status.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>


namespace GUI
{
    class CGaInfoDlg final : public IPanel
    {
        ipc::GaStatus _status;
        sf::Texture _genesTexture;
        vector<sf::Texture> _populationTextures;

    public:
        CGaInfoDlg();

        sf::Sprite _get_genes_sprite(const string &genes, sf::Texture &texture);

        void render() override;
        void set(const ipc::GaStatus& status);

        ~CGaInfoDlg() override = default;
    };
}
