#pragma once

#include <cstdint>
#include <main_menu.h>


namespace GUI
{
    class CGaBox2dApp
    {
        uint64_t screenWidth = 1920;
        uint64_t screenHeight = 1080;
        CMainMenuPtr _menu;
        IGaBox2dViewPtr _view;
        IGaBox2dDocPtr _doc;

    public:
        CGaBox2dApp();

        int run();
    };
}