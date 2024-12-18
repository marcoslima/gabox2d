// GaBox2d.cpp : Defines the class behaviors for the application.
//

#include "GaBox2d.h"
#include "GaParamsDlg.h"
#include "EditorChaoDlg.h"
#include "GaBox2dView.h"
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>


using namespace std;


namespace GUI
{
    // CGaBox2dApp
    // CGaBox2dApp construction

    CGaBox2dApp::CGaBox2dApp() = default;

    void _show_help()
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

    void DrawMenu(CGaParamsDlg &dlgParams,
                  CEditorChaoDlg &dlgEditorChao,
                  CGaBox2dView &view)
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
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {view.OnEditCopy();}
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {view.OnEditPaste();}
            if (ImGui::MenuItem("Ambiente")){bShowEditor = true;}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Simulação"))
        {
            if (ImGui::MenuItem("Play")) {}
            if (ImGui::MenuItem("Reset"))
            {
                view.OnSimulaReset();
            }
            if (ImGui::BeginMenu("Velocidade"))
            {
                unsigned current_vel = view.getVelocidade();
                if (ImGui::MenuItem("1x", nullptr, current_vel == 1))
                {
                    view.setVelocidade(1);
                }
                if (ImGui::MenuItem("2x", nullptr, current_vel == 2))
                {
                    view.setVelocidade(2);
                }
                if (ImGui::MenuItem("4x", nullptr, current_vel == 4))
                {
                    view.setVelocidade(4);
                }
                if (ImGui::MenuItem("10x", nullptr, current_vel == 10))
                {
                    view.setVelocidade(10);
                }
                if (ImGui::MenuItem("100x"))
                {
                    view.setVelocidade(100);
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Repetir"))
            {
                view.OnSimulaRepetir();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("GA"))
        {
            if (ImGui::MenuItem("Iniciar GA..."))
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
            if (ImGui::MenuItem("Follow car\tF", nullptr, view.isFollowCar()))
            {
                view.toggleFollowCar();
            }
            if (ImGui::MenuItem("Draw Ground Debug", nullptr, view.isDebugGround()))
            {
                view.toggleDrawDebugGround();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Show..."))
            {
                view.ShowHelp();
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
        if (bShowGaParams) view.OnGaIniciarga();
        ImGui::EndMainMenuBar();
    }

    void _show_info(const CGaBox2dView &view)
    {
        ImGui::Begin("Info");
        ImGui::Text("Dead reason: %s", view.getDeadReason().c_str());
        ImGui::Text("Time: %f", view.GetDocument()->GetCar().getT());
        ImGui::Text("Genes: %s", view.GetDocument()->GetCar().getGenes());
        ImGui::End();
    }

    void DrawGui(CGaParamsDlg &dlgParams,
                 CEditorChaoDlg &dlgEditorChao,
                 CGaBox2dView &view)
    {
        DrawMenu(dlgParams, dlgEditorChao, view);

        ////////////////////////////////////
        /// Show Help
        if (view.isShowHelp())
        {
            _show_help();
        }

        _show_info(view);
    }

    int CGaBox2dApp::run()
    {
        screenWidth = 1920;
        screenHeight = 1080;
        sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "GaBox2d Reborned");
        window.setFramerateLimit(60);
        if (!ImGui::SFML::Init(window, true))
        {
            std::cout << "Error initializing ImGui-SFML!" << std::endl;
            return 1;
        }
        // ImGuiIO& io = ImGui::GetIO();

        auto doc = CGaBox2dDoc();
        auto env = CEnv();
        doc.OnNewDocument(env);
        auto view = CGaBox2dView();
        view.SetDocument(&doc);

        sf::Clock deltaClock;
        // bool bMouseDown = false;
        // sf::Vector2i ptMouse, lastPtMouse;
        CGaParamsDlg dlgParams(view);
        CEditorChaoDlg dlgEditorChao;

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                ImGui::SFML::ProcessEvent(window, event);

                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed)
                {
                    view.OnKeyPressed(event.key.code);
                }
                if (event.type == sf::Event::KeyReleased)
                {
                    view.OnKeyReleased(event.key.code);
                }
            }

            if (doc.m_bQuit)
            {
                window.close();
            }
            for (int i = 0; i < view.getVelocidade(); i++)
                doc.GetCar().doStep();

            ImGui::SFML::Update(window, deltaClock.restart());
            ImGui::ShowDemoWindow();

            window.clear();

            // VIEW.DRAW
            // dlgEditorChao.OnInitDialog();
            DrawGui(dlgParams, dlgEditorChao, view);
            // dlgParams.draw();
            // if(ImGui::Button("Open")) dlgParams.show();
            view.Draw(window);
            ImGui::SFML::Render(window);

            window.display();
        }

        ImGui::SFML::Shutdown();

        return 0;
    }


    // The one and only CGaBox2dApp object
    CGaBox2dApp theApp;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    return GUI::theApp.run();
}
