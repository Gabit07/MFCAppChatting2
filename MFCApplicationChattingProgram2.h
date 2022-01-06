
// MFCApplicationChattingProgram2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCApplicationChattingProgram2App:
// See MFCApplicationChattingProgram2.cpp for the implementation of this class
//

class CMFCApplicationChattingProgram2App : public CWinApp
{
public:
	CMFCApplicationChattingProgram2App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplicationChattingProgram2App theApp;
