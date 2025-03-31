#include <GaBox2dApp.h>
#include "EditorChaoDlg.h"
#include "GaBox2dView.h"
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <main_menu.h>

namespace GUI
{
    CGaBox2dApp::CGaBox2dApp()
    {
        _doc = std::make_shared<CGaBox2dDoc>();
        _view = std::make_shared<CGaBox2dView>();
        _menu = std::make_shared<CMainMenu>(_view);
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

        auto env = CEnv();
        _doc->OnNewDocument(env);
        _view->SetDocument(_doc);

        sf::Clock deltaClock;
        // bool bMouseDown = false;
        // sf::Vector2i ptMouse, lastPtMouse;

        while (window.isOpen())
        {
            sf::Event event{};
            while (window.pollEvent(event))
            {
                ImGui::SFML::ProcessEvent(window, event);

                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed)
                {
                    _view->OnKeyPressed(&event.key.code);
                }
                if (event.type == sf::Event::KeyReleased)
                {
                    _view->OnKeyReleased(&event.key.code);
                }
            }

            if (_doc->isQuit())
            {
                window.close();
            }

            if (_doc->isSimulating())
            {
                for (int i = 0; i < _view->getVelocidade(); i++)
                    _doc->GetCar()->doStep();
            }

            ImGui::SFML::Update(window, deltaClock.restart());
            ImGui::ShowDemoWindow();

            window.clear();

            // VIEW.DRAW
            // dlgEditorChao.OnInitDialog();
            _menu->draw();

            // dlgParams.draw();
            // if(ImGui::Button("Open")) dlgParams.show();
            _view->draw(&window);
            ImGui::SFML::Render(window);

            window.display();
        }

        ImGui::SFML::Shutdown();

        return 0;
    }


};