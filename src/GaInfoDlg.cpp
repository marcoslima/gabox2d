#include <imgui.h>
#include <GaInfoDlg.h>
#include <GaBox2dView.h>
#include <imgui-sfml/imgui-SFML.h>


namespace GUI
{
    CGaInfoDlg::CGaInfoDlg()
    : _status(0, 0.0, 0.0, "", {}, {})
    {
    }
    void make_image_from_genes(const string& genes, sf::Image& target)
    {
        const auto width = genes.size();
        vector<sf::Uint8> pixels;
        pixels.reserve(width * 4);
        for (const auto& c : genes)
        {
            const auto value = c == '1' ? 0xff : 0x0;
            pixels.push_back(value);
            pixels.push_back(value);
            pixels.push_back(value);
            pixels.push_back(0xff);
        }

        target.create(width, 1, pixels.data());
    }

    sf::Sprite CGaInfoDlg::_get_genes_sprite(const string& genes, sf::Texture& texture) // NOLINT(*-convert-member-functions-to-static)
    {
        sf::Sprite genesSprite;
        const auto width = genes.size();
        if (width > 0)
        {
            sf::Image bestGenes;
            make_image_from_genes(genes, bestGenes);
            texture.loadFromImage(bestGenes);
            genesSprite.setTexture(texture, true);
            genesSprite.setScale(1.0f, 12.0f);
        }

        return genesSprite;
    }

    void CGaInfoDlg::render()
    {
        const sf::Sprite bestSpr = _get_genes_sprite(_status.bestGenes, _genesTexture);

        ImGui::Begin("GA Info");
        ImGui::BeginGroup();
        ImGui::Text("Geração: %lu", _status.generation);
        ImGui::Text("População: %ld", _status.population.size());
        ImGui::Text("Gens p/s: %f", _status.gps);
        ImGui::Text("Melhor fitness: %f", _status.bestFitness);
        // ImGui::Text("Melhor genoma: %s", _status.bestGenes.c_str());
        ImGui::Text("Melhor genoma: "); ImGui::SameLine(); ImGui::Image(bestSpr);
        ImGui::Text("Histórico: %ld", _status.best_history.size());

        if (!_status.population.empty())
        {
            _populationTextures.clear();
            _populationTextures.reserve(_status.population.size());

            constexpr auto flags = ImGuiTableFlags_Resizable;
            ImGui::BeginTable("Genomas", 2, flags);
            ImGui::TableSetupColumn("Pts");
            ImGui::TableSetupColumn("Genoma", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            for (const auto& individual : _status.population)
            {
                sf::Texture texture;
                _populationTextures.push_back(std::move(texture));
                const auto genesSprite = _get_genes_sprite(
                    individual.second, _populationTextures.back());

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%.01f", individual.first);
                ImGui::TableNextColumn();
                // ImGui::Text("%s", individual.second.c_str());
                ImGui::Image(genesSprite);
            }
            ImGui::EndTable();
        }

        ImGui::EndGroup();
        ImGui::End();
    }

    void CGaInfoDlg::set(const ipc::GaStatus &status)
    {
        _status = status;
    }

    // void CGaInfoDlg::OnNMDblclkGenes(NMHDR *pn, LRESULT *pResult)
    // {
    //     *pResult = 0;
    //     NMITEMACTIVATE *pni = (NMITEMACTIVATE *) pn;
    //
    //     if (pni->iItem == -1)
    //         return;
    //     int iItem = pni->iItem;
    //
    //     CString strGenes = m_lstGenes.GetItemData(iItem, 1).GetString();
    //     char *szGenes = new char[strGenes.GetLength() + 5];
    //     strcpy(szGenes, strGenes);
    //
    //     m_pView->SendMessage(IDM_SIMULAR_GENE, 0, (LPARAM) szGenes);
    //
    //     delete szGenes;
    // }
}

