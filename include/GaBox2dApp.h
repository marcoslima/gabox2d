#pragma once

#include <cstdint>


namespace GUI
{
    class CGaBox2dApp
    {
        uint64_t screenWidth = 1920;
        uint64_t screenHeight = 1080;

    public:
        CGaBox2dApp();

        int run();
    };
};
