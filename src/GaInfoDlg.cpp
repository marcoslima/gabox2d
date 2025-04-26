#include <imgui.h>
#include <GaInfoDlg.h>
#include <GaBox2dView.h>
#include <ranges>
#include <imgui-sfml/imgui-SFML.h>


namespace GUI
{
    CGaInfoDlg::CGaInfoDlg()
    : _status(0, 0.0, 0.0, "", {}, {})
    {
    }

    void CGaInfoDlg::_render_top() {
        ImGui::Text("Geração: %lu", _status.generation);
        ImGui::Text("População: %ld", _status.population.size());
        ImGui::Text("Gens p/s: %f", _status.gps);
        ImGui::Text("Melhor fitness: %f", _status.bestFitness);
        // ImGui::Text("Melhor genoma: %s", _status.bestGenes.c_str());
        ImGui::Text("Melhor genoma: "); ImGui::SameLine(); _bestGenoma.render();
        ImGui::Text("Histórico: %ld", _status.best_history.size());
    }

    void CGaInfoDlg::_render_population()
    {
        if (_status.population.empty()) return;

        _populationGenomas.clear();
        _populationGenomas.reserve(_status.population.size());

        constexpr auto flags = ImGuiTableFlags_Resizable;

        ImGui::BeginGroup();
        ImGui::BeginTable("Genomas", 2, flags);
        ImGui::TableSetupColumn("Pts");
        ImGui::TableSetupColumn("Genoma", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        for (const auto& individual : _status.population)
        {
            _populationGenomas.emplace_back(CCarDef::bits::len());
            _populationGenomas.back().set(individual.second);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%.01f", individual.first);
            ImGui::TableNextColumn();
            // ImGui::Text("%s", individual.second.c_str());
            _populationGenomas.back().render();
        }
        ImGui::EndTable();
        ImGui::EndGroup();
    }

    void CGaInfoDlg::_render_history()
    {
        if (_status.best_history.empty()) return;

        _historyGenomas.clear();
        _historyGenomas.reserve(_status.best_history.size());

        constexpr auto flags = ImGuiTableFlags_Resizable;

        ImGui::BeginGroup();
        ImGui::BeginTable("Genomas_history", 3, flags);
        ImGui::TableSetupColumn("Generation");
        ImGui::TableSetupColumn("Fitness");
        ImGui::TableSetupColumn("Genoma", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        for (const auto& individual : views::reverse(_status.best_history))
        {
            _historyGenomas.emplace_back(CCarDef::bits::len());
            _historyGenomas.back().set(individual.second);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%ld", individual.first);
            ImGui::TableNextColumn();
            ImGui::Text("%.1f", 1000000.0f);
            ImGui::TableNextColumn();
            _historyGenomas.back().render();
        }
        ImGui::EndTable();
        ImGui::EndGroup();
    }

    void CGaInfoDlg::_render_bottom()
    {
        ImGui::BeginTabBar("genomas", ImGuiTabBarFlags_None);

        if (ImGui::BeginTabItem("Population"))
        {
            _render_population();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Historico"))
        {
            _render_history();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    void CGaInfoDlg::render()
    {
        _bestGenoma.set(_status.bestGenes);

        ImGui::Begin("GA Info");
        ImGui::BeginGroup();

        _render_top();
        _render_bottom();

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

