#pragma once
#include <cstdint>

namespace GUI
{
// CGaBox2dApp:
// See GaBox2d.cpp for the implementation of this class
//

class CGaBox2dApp
{
	uint64_t screenWidth = 1920;
	uint64_t screenHeight = 1080;

public:
	CGaBox2dApp();

	int run();
};

extern CGaBox2dApp theApp;

};//namespace GUI
