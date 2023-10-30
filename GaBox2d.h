#ifndef __GABOX2D_H__
#define __GABOX2D_H__

// GaBox2d.h : main header file for the GaBox2d application
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#define IDM_GA_INFO			WM_USER + 1002
#define IDM_SIMULAR_GENE	WM_USER + 1003

#include <gdiplus.h>
using namespace Gdiplus;

namespace GUI
{
// CGaBox2dApp:
// See GaBox2d.cpp for the implementation of this class
//

class CGaBox2dApp : public CWinApp
{
public:
	CGaBox2dApp();

   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CGaBox2dApp theApp;

};//namespace GUI

#endif //__GABOX2D_H__