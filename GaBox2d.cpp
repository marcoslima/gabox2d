// GaBox2d.cpp : Defines the class behaviors for the application.
//

#include "GaBox2d.h"
//#include "GaBox2dDoc.h"
//#include "GaBox2dView.h"
#include "GaParamsDlg.h"
#include "GaBox2d.h"
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

int CGaBox2dApp::run()
{
    uint64_t screenWidth = 1920;
    uint64_t screenHeight = 1080;
    double aspectRatio = (double)screenWidth / (double)screenHeight;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "GaBox2d Reborned");
    window.setFramerateLimit(60);
    if(!ImGui::SFML::Init(window, true))
    {
        std::cout << "Error initializing ImGui-SFML!" << std::endl;
        return 1;
    }
    ImGuiIO& io = ImGui::GetIO();

    sf::Clock deltaClock;
    bool bMouseDown = false;
    sf::Vector2i ptMouse, lastPtMouse;
	CGaParamsDlg dlgParams;
    while (window.isOpen()) 
    {

        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
		ImGui::ShowDemoWindow();
        window.clear();
		// VIEW.DRAW
		dlgParams.draw();
		if(ImGui::Button("Open")) dlgParams.show();
        ImGui::SFML::Render(window);
        window.display();

    }

    ImGui::SFML::Shutdown();
}


// The one and only CGaBox2dApp object
CGaBox2dApp theApp;


};//namespace GUI

int main(int argc, char* argv[])
{
	return GUI::theApp.run();
}
