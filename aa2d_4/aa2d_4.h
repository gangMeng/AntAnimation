// aa2d_4.h : main header file for the aa2d_4 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Caa2d_4App:
// See aa2d_4.cpp for the implementation of this class
//

class Caa2d_4App : public CWinApp
{
public:
	Caa2d_4App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Caa2d_4App theApp;