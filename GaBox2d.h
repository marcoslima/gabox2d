// GaBox2d.h : main header file for the GaBox2d application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CGaBox2dApp:
// See GaBox2d.cpp for the implementation of this class
//

class CGaBox2dApp : public CWinApp
{
public:
	CGaBox2dApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGaBox2dApp theApp;