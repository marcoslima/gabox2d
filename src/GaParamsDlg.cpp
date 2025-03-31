#include <imgui.h>
#include "GaParamsDlg.h"
#include "imgui_input8.h"

namespace GUI
{
    // CGaParamsDlg dialog
    CGaParamsDlg::CGaParamsDlg(const callback_t &fn_callback)
        : BasicModalDlg("Parâmetros do GA")
        , _callback(fn_callback)
    {}

    void CGaParamsDlg::RenderLeftGroup()
    {
        ImGui::BeginGroup();
        InputUint8("Population", &m_nPopulacao);
        ImGui::SliderFloat("Crossover (%)", &m_fCrossover, 0.0f, 100.0f);
        InputUint8("Elitism", &m_nElitismo);
        ImGui::SliderFloat("Mutation (%)", &m_fMutacao, 0.0f, 100.0f);
        ImGui::EndGroup();
    }

    void CGaParamsDlg::RenderRightGroup()
    {
        ImGui::BeginGroup();
        InputUint8("Alienism", &m_nAlienismo);
        InputUint8("Mutation Interval", &m_nMutInt);
        ImGui::InputFloat("Max Time", &m_fMaxT);
        ImGui::EndGroup();
    }

    void CGaParamsDlg::RenderButtons()
    {
        if (ImGui::Button("Cancelar"))
        {
            Hide();
        }

        ImGui::SameLine();

        if (ImGui::Button("Iniciar"))
        {
            _callback(*this);
            Hide();
        }
    }

    void CGaParamsDlg::RenderContent()
    {
        RenderLeftGroup();
        ImGui::SameLine();
        RenderRightGroup();
        RenderButtons();
    }
}
