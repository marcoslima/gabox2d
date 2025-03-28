// GaParamsDlg.cpp : implementation file
//
#include <imgui.h>
#include "GaParamsDlg.h"
#include "imgui_input8.h"

namespace GUI
{
    // CGaParamsDlg dialog
    CGaParamsDlg::CGaParamsDlg(IGaBox2dView &view)
        : m_params{60, 75, 2, 65, 0, 10, 60}
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
        InputUint8("Population", &m_params.m_nPopulacao);
        ImGui::SliderFloat("Crossover (%)", &m_params.m_fCrossover, 0.0f, 100.0f);
        InputUint8("Elitism", &m_params.m_nElitismo);
        ImGui::SliderFloat("Mutation (%)", &m_params.m_fMutacao, 0.0f, 100.0f);
        ImGui::EndGroup();
    }

    void CGaParamsDlg::RenderRightGroup()
    {
        ImGui::BeginGroup();
        InputUint8("Alienism", &m_params.m_nAlienismo);
        InputUint8("Mutation Interval", &m_params.m_nMutInt);
        ImGui::InputFloat("Max Time", &m_params.m_fMaxT);
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
            _view.startGa(m_params);
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
