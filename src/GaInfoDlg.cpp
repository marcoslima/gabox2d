#include <imgui.h>
#include <GaInfoDlg.h>
#include <GaBox2dView.h>


namespace GUI
{
    CGaInfoDlg::CGaInfoDlg()
    : _status(0, 0.0, 0.0, "", {}, {})
    {
    }

    void CGaInfoDlg::render()
    {
        ImGui::Begin("GA Info");
        ImGui::BeginGroup();
        ImGui::Text("Geração: %lu", _status.generation);
        ImGui::Text("População: %ld", _status.population.size());
        ImGui::Text("Gens p/s: %f", _status.gps);
        ImGui::Text("Melhor fitness: %f", _status.bestFitness);
        ImGui::Text("Melhor genes: %s", _status.bestGenes.c_str());
        ImGui::Text("Histórico: %ld", _status.best_history.size());

        if (!_status.population.empty())
        {
            constexpr auto flags = ImGuiTableFlags_Resizable;
            ImGui::BeginTable("Genes", 2, flags);
            ImGui::TableSetupColumn("Pts");
            ImGui::TableSetupColumn("Genes", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            for (const auto& gene : _status.population)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%.01f", gene.first);
                ImGui::TableNextColumn();
                ImGui::Text("%s", gene.second.c_str());
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

