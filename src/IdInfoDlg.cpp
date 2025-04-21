#include <imgui.h>
#include <IdInfoDlg.h>

namespace GUI
{
    void CIdInfoDlg::set(const unsigned nGeracao,
                         const float dPontuacao,
                         const float dTempo,
                         const string &strGenes,
                         const string &strDeathReason)
    {
        if (nGeracao != -1)
            m_strGeracao = std::to_string(nGeracao);
        else
            m_strGeracao = "none";

        if (dPontuacao != -1)
        {
            m_strPontuacao = std::to_string(dPontuacao);
        }
        else
        {
            m_strPontuacao = "none";
        }

        m_strTempo = std::to_string(dTempo);
        if (strGenes != m_strGenes)
        {
            _currentGenoma.set(strGenes);
        }
        m_strGenes = strGenes;
        m_strDeathReason = strDeathReason;
    }

    void CIdInfoDlg::render()
    {
        ImGui::Begin("Informações do atual");
        ImGui::Text("Dead reason: %s", m_strDeathReason.c_str());
        ImGui::SameLine();
        ImGui::Text("| Time: %s", m_strTempo.c_str());
        ImGui::SameLine();
        ImGui::Text("| Generation: %s", m_strGeracao.c_str());
        ImGui::SameLine();
        // ImGui::Text("Genes: %s", m_strGenes.c_str());
        ImGui::Text("| Genes: "); ImGui::SameLine(); _currentGenoma.render();
        ImGui::End();
    }
}