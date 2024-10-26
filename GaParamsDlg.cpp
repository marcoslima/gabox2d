// GaParamsDlg.cpp : implementation file
//
#include <imgui.h>
#include "GaBox2d.h"
#include "GaParamsDlg.h"
#include "imgui_input8.h"

namespace GUI
{
// CGaParamsDlg dialog
CGaParamsDlg::CGaParamsDlg()
	: ga_params_t(60, 75, 2, 65, 0, 10, 60)
{
}

void CGaParamsDlg::show()
{
	ImGui::OpenPopup("Parâmetros para o GA");
}

void CGaParamsDlg::draw()
{
	ImGui::BeginPopupModal("Parâmetros para o GA", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::BeginGroup();
		InputUint8("Population", &m_nPopulacao);
		ImGui::SliderFloat("Crossover (%)", &m_fCrossover, 0.0f, 100.0f);
		InputUint8("Elitism", &m_nElitismo);
		ImGui::SliderFloat("Mutation (%)", &m_fMutacao, 0.0f, 100.0f);
	ImGui::EndGroup();
	ImGui::SameLine();
	ImGui::BeginGroup();
		InputUint8("Alienism", &m_nAlienismo);
		InputUint8("Mutation Interval", &m_nMutInt);
		ImGui::InputFloat("Max Time", &m_fMaxT);
	ImGui::EndGroup();
	if(ImGui::Button("Cancelar"))
	{
		ImGui::CloseCurrentPopup();
	}
	ImGui::SameLine();
	if(ImGui::Button("Iniciar"))
	{
		ImGui::CloseCurrentPopup();
	}
	// ImGui::EndPopup();
}

};//namespace GUI