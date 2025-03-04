// GaParamsDlg.cpp : implementation file
//
#include <imgui.h>
#include "GaParamsDlg.h"
#include "imgui_input8.h"

namespace GUI
{
    // CGaParamsDlg dialog
    CGaParamsDlg::CGaParamsDlg(CGaBox2dView &view)
        : params{60, 75, 2, 65, 0, 10, 60}
          , _view{view} {}

    void CGaParamsDlg::OnInitDialog()
    {
        if (ImGui::BeginPopupModal(_wndName, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            RenderDialog();
            ImGui::EndPopup();
        }
    }

    void CGaParamsDlg::show() const
    {
        ImGui::OpenPopup(_wndName);
    }

    void CGaParamsDlg::RenderLeftGroup()
    {
        ImGui::BeginGroup();
        InputUint8("Population", &params.m_nPopulacao);
        ImGui::SliderFloat("Crossover (%)", &params.m_fCrossover, 0.0f, 100.0f);
        InputUint8("Elitism", &params.m_nElitismo);
        ImGui::SliderFloat("Mutation (%)", &params.m_fMutacao, 0.0f, 100.0f);
        ImGui::EndGroup();
    }

    void CGaParamsDlg::RenderRightGroup()
    {
        ImGui::BeginGroup();
        InputUint8("Alienism", &params.m_nAlienismo);
        InputUint8("Mutation Interval", &params.m_nMutInt);
        ImGui::InputFloat("Max Time", &params.m_fMaxT);
        ImGui::EndGroup();
    }

    void CGaParamsDlg::RenderButtons() const
    {
        if (ImGui::Button("Cancelar"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Iniciar"))
        {
            _view._start_ga(*this);
            ImGui::CloseCurrentPopup();
        }
    }

    void CGaParamsDlg::RenderDialog()
    {
        RenderLeftGroup();

        ImGui::SameLine();

        RenderRightGroup();

        RenderButtons();
    }
}
