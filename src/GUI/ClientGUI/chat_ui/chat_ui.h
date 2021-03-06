
// chat_ui.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "ClientBackgroundService.h"

// ChatUiApp:
// See chat_ui.cpp for the implementation of this class
//

class ChatUiApp : public CWinApp
{
public:
	ChatUiApp();

// Overrides
public:
	ClientBackgroundService m_ClientService;

	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern ChatUiApp theApp;
