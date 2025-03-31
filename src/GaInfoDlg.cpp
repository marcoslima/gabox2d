#include <imgui.h>
#include <GaInfoDlg.h>
#include <GaBox2dView.h>


namespace GUI
{
    CGaInfoDlg::CGaInfoDlg(const IGaBox2dViewPtr &view)
        : _wndName("GA Info")
        , _view(view)
        , m_nGeracao(0)
        , m_nPopulacao(0) {}

    CGaInfoDlg::~CGaInfoDlg() {}
    
    void CGaInfoDlg::OnInitDialog()
    {
        ImGui::BeginPopup(_wndName, ImGuiWindowFlags_Popup);
        Render();
    }

    void CGaInfoDlg::Render()
    {
        const ipc::GaStatus& status = _view->getCurrentStatus();
        ImGui::BeginGroup();
        ImGui::Text("Geração: %d", status.generation);
        ImGui::Text("População: %ld", status.population.size());
        ImGui::Text("Gens p/s: %f", status.gps);
        ImGui::Text("Melhor fitness: %f", status.bestFitness);
        ImGui::Text("Melhor genes: %s", status.bestGenes.c_str());
        ImGui::Text("Histórico: %ld", status.best_history.size());

        constexpr auto flags = ImGuiTableFlags_Resizable;
        ImGui::BeginTable("Genes", 2, flags);
        ImGui::TableSetupColumn("Pts");
        ImGui::TableSetupColumn("Genes", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        for (const auto& gene : status.population)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%.01f", gene.first);
            ImGui::TableNextColumn();
            ImGui::Text("%s", gene.second.c_str());
        }
        ImGui::EndTable();

        ImGui::EndGroup();
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

