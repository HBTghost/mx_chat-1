// MxChatDLL.h : main header file for the MxChatDLL DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMxChatDLLApp
// See MxChatDLL.cpp for the implementation of this class
//

class CMxChatDLLApp : public CWinApp
{
public:
	CMxChatDLLApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
