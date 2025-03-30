//
// Created by marcos on 3/28/25.
//
#include <main_menu.h>

#include "GaInfoDlg.h"

namespace GUI
{
    void CMainMenu::DrawMenu(CGaParamsDlg &dlgParams, CEditorChaoDlg &dlgEditorChao)
    {
        ImGui::BeginMainMenuBar();
        dlgEditorChao.OnInitDialog();
        dlgParams.OnInitDialog();

        bool bShowEditor = false;
        bool bShowGaParams = false;
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As..")) {}
            if (ImGui::MenuItem("Exit"))
            {
                exit(0);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Copy", "Ctrl+C"))
            {
                _view->OnEditCopy();
            }
            if (ImGui::MenuItem("Paste", "Ctrl+V"))
            {
                _view->OnEditPaste();
            }
            if (ImGui::MenuItem("Ambiente"))
            {
                bShowEditor = true;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Simulação"))
        {
            if (ImGui::MenuItem("Play"))
            {
                _view->OnSimulaPlay();
            }
            if (ImGui::MenuItem("Reset"))
            {
                _view->OnSimulaReset();
            }
            if (ImGui::BeginMenu("Velocidade"))
            {
                unsigned current_vel = _view->getVelocidade();
                if (ImGui::MenuItem("1x", nullptr, current_vel == 1))
                {
                    _view->setVelocidade(1);
                }
                if (ImGui::MenuItem("2x", nullptr, current_vel == 2))
                {
                    _view->setVelocidade(2);
                }
                if (ImGui::MenuItem("4x", nullptr, current_vel == 4))
                {
                    _view->setVelocidade(4);
                }
                if (ImGui::MenuItem("10x", nullptr, current_vel == 10))
                {
                    _view->setVelocidade(10);
                }
                if (ImGui::MenuItem("100x"))
                {
                    _view->setVelocidade(100);
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Repetir"))
            {
                _view->OnSimulaRepetir();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("GA"))
        {
            const string sIniciarGa = _view->isGaRunning() ? "Parar GA" : "Iniciar GA...";
            if (ImGui::MenuItem(sIniciarGa.c_str()))
            {
                bShowGaParams = true;
            }
            if (ImGui::BeginMenu("Mostrar atual"))
            {
                if (ImGui::MenuItem("Melhor")) {}
                if (ImGui::MenuItem("Qualquer")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Follow car\tF", nullptr, _view->isFollowCar()))
            {
                _view->toggleFollowCar();
            }
            if (ImGui::MenuItem("Draw Ground Debug", nullptr, _view->isDebugGround()))
            {
                _view->toggleDrawDebugGround();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Show..."))
            {
                _view->ShowHelp();
            }
            if (ImGui::BeginMenu("Shortcuts..."))
            {
                ImGui::MenuItem("Zoom in/out\t+/-");
                ImGui::MenuItem("Pan\t<-/->");
                ImGui::MenuItem("Show help\tF1");
                ImGui::MenuItem("New random car\tN");
                ImGui::MenuItem("Repeat current car\tR");
                ImGui::MenuItem("Follow car\tF");
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (bShowEditor) dlgEditorChao.show();
        if (bShowGaParams) _view->OnGaIniciarga();
        ImGui::EndMainMenuBar();
    }

    void CMainMenu::_show_info()
    {
        ImGui::Begin("Info");
        ImGui::Text("Dead reason: %s", _view->getDeadReason().c_str());
        ImGui::Text("Time: %f", _view->GetDocument()->GetCar()->getT());
        ImGui::Text("Genes: %s", _view->GetDocument()->GetCar()->getGenes().c_str());
        ImGui::End();
    }

    void CMainMenu::_show_ga_info(CGaInfoDlg &ga_info_dlg)
    {
        CGaInfo gaInfo;
        ga_info_dlg.OnInitDialog(&gaInfo);
    }

    void CMainMenu::_show_help()
    {
        constexpr ImGuiTableFlags flags1 = ImGuiTableFlags_Borders
                                           | ImGuiTableFlags_NoBordersInBodyUntilResize
                                           | ImGuiTableFlags_NoHostExtendX;

        ImGui::Begin("Help");
        if (ImGui::BeginTable("help table", 2, flags1))
        {
            ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("F1");
            ImGui::TableNextColumn();
            ImGui::Text("Show Help");
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("<-/->");
            ImGui::TableNextColumn();
            ImGui::Text("Horizontal Pan");
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("+/-");
            ImGui::TableNextColumn();
            ImGui::Text("Zoom");
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("N");
            ImGui::TableNextColumn();
            ImGui::Text("New random car");
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("R");
            ImGui::TableNextColumn();
            ImGui::Text("Repeat current car");
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("F");
            ImGui::TableNextColumn();
            ImGui::Text("Follow car");
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Q");
            ImGui::TableNextColumn();
            ImGui::Text("Quit");

            ImGui::EndTable();
        }
        ImGui::End();
    }

    CMainMenu::CMainMenu(IGaBox2dViewPtr &view)
    : _view(view)
    {
    }

    void CMainMenu::draw(CGaParamsDlg &dlgParams,
                            CEditorChaoDlg &dlgEditorChao,
                            CGaInfoDlg &dlgGaInfoDlg)
    {
        DrawMenu(dlgParams, dlgEditorChao);

        ////////////////////////////////////
        /// Show Help
        if (_view->isShowHelp())
        {
            _show_help();
        }

        _show_info();
        _show_ga_info(dlgGaInfoDlg);
    }
}
