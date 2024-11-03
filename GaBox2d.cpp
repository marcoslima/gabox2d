// GaBox2d.cpp : Defines the class behaviors for the application.
//

#include "GaBox2d.h"
//#include "GaBox2dDoc.h"
//#include "GaBox2dView.h"
#include "GaParamsDlg.h"
#include "GaBox2d.h"
#include "EditorChaoDlg.h"
#include "GaBox2dView.h"
#include <iostream>
using namespace std;

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>


namespace GUI
{
// CGaBox2dApp
// CGaBox2dApp construction

CGaBox2dApp::CGaBox2dApp()
{
}

void DrawMenu(CGaParamsDlg& dlgParams,
              CEditorChaoDlg& dlgEditorChao)
{
    ImGui::BeginMainMenuBar();
    dlgEditorChao.OnInitDialog();
    bool bShowEditor = false;
    if(ImGui::BeginMenu("File")) 
    {
        if(ImGui::MenuItem("New", "Ctrl+N")) {}
        if(ImGui::MenuItem("Open", "Ctrl+O")) {}
        if(ImGui::MenuItem("Save", "Ctrl+S")) {}
        if(ImGui::MenuItem("Save As..")) {}
        if(ImGui::MenuItem("Exit")) {
            exit(0);
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) 
    {
        if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
        if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
        if (ImGui::MenuItem("Ambiente")) {
            // dlgEditorChao.show();
            bShowEditor = true;
        }
        ImGui::EndMenu();
    }

    if(ImGui::BeginMenu("Simulação"))
    {
        if(ImGui::MenuItem("Play")) {}
        if(ImGui::MenuItem("Reset")) {}
        if(ImGui::BeginMenu("Velocidade")) 
        {
            if(ImGui::MenuItem("1x")) {}
            if(ImGui::MenuItem("2x")) {}
            if(ImGui::MenuItem("4x")) {}
            if(ImGui::MenuItem("10x")) {}
            if(ImGui::MenuItem("100x")) {}
            ImGui::EndMenu();
        }
        if(ImGui::MenuItem("Repetir")) {}
        ImGui::EndMenu();
    }

    if(ImGui::BeginMenu("GA"))
    {
        if(ImGui::MenuItem("Iniciar GA...")) 
        {
            dlgParams.show();
        }
        if(ImGui::BeginMenu("Mostrar atual")) 
        {
            if(ImGui::MenuItem("Melhor")) {}
            if(ImGui::MenuItem("Qualquer")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if(bShowEditor) dlgEditorChao.show();
    ImGui::EndMainMenuBar();
}

int CGaBox2dApp::run()
{
    uint64_t screenWidth = 1920;
    uint64_t screenHeight = 1080;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "GaBox2d Reborned");
    window.setFramerateLimit(60);
    if(!ImGui::SFML::Init(window, true))
    {
        std::cout << "Error initializing ImGui-SFML!" << std::endl;
        return 1;
    }
    ImGuiIO& io = ImGui::GetIO();

    auto doc = CGaBox2dDoc();
    auto view = CGaBox2dView();
    view.SetDocument(&doc);

    sf::Clock deltaClock;
    bool bMouseDown = false;
    sf::Vector2i ptMouse, lastPtMouse;
	CGaParamsDlg dlgParams;
    CEditorChaoDlg dlgEditorChao;

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if(event.type == sf::Event::KeyPressed)
            {
                view.OnKeyPressed(event.key.code);
            }
            if(event.type == sf::Event::KeyReleased)
            {
                view.OnKeyReleased(event.key.code);
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
		ImGui::ShowDemoWindow();
        window.clear();

        // VIEW.DRAW
        // dlgEditorChao.OnInitDialog();
        DrawMenu(dlgParams, dlgEditorChao);
		// dlgParams.draw();
		// if(ImGui::Button("Open")) dlgParams.show();

        ImGui::SFML::Render(window);
        window.display();

    }

    ImGui::SFML::Shutdown();
    
    return 0;
}


// The one and only CGaBox2dApp object
CGaBox2dApp theApp;


};//namespace GUI

int main(int argc, char* argv[])
{
	return GUI::theApp.run();
}
