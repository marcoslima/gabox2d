#pragma once

// GaBox2d.h : main header file for the GaBox2d application
#define IDM_GA_INFO			WM_USER + 1002
#define IDM_SIMULAR_GENE	WM_USER + 1003

namespace GUI
{
// CGaBox2dApp:
// See GaBox2d.cpp for the implementation of this class
//

class CGaBox2dApp
{
public:
	CGaBox2dApp();

    int run(void);
};

extern CGaBox2dApp theApp;

};//namespace GUI
