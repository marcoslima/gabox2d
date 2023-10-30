// GaParamsDlg.cpp : implementation file
//
#include <imgui.h>
#include "GaBox2d.h"
#include "GaParamsDlg.h"


namespace GUI
{
// CGaParamsDlg dialog
CGaParamsDlg::CGaParamsDlg()
	: ga_params_t(60, 75, 2, 65, 0, 10, 60)
{
}

void CGaParamsDlg::show()
{
	ImGui::OpenPopup("GA Params");
}

void CGaParamsDlg::draw()
{
	if(ImGui::BeginPopupModal("GA Params", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputScalar("Population", ImGuiDataType_U64, &m_nPopulacao);
		ImGui::InputFloat("Crossover", &m_fCrossover);
		ImGui::InputScalar("Elitism", ImGuiDataType_U64, &m_nElitismo);
		ImGui::InputFloat("Mutation", &m_fMutacao);
		ImGui::InputScalar("Alienism", ImGuiDataType_U64, &m_nAlienismo);
		ImGui::InputScalar("Mutation Interval", ImGuiDataType_U64, &m_nMutInt);
		ImGui::InputFloat("Max Time", &m_fMaxT);
		if(ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

};//namespace GUI