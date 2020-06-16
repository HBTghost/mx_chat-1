// AttachFile.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "AttachFile.h"


// AttachFile

IMPLEMENT_DYNAMIC(AttachFile, CFileDialog)

AttachFile::AttachFile(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{

}

AttachFile::~AttachFile()
{
}


BEGIN_MESSAGE_MAP(AttachFile, CFileDialog)
END_MESSAGE_MAP()



// AttachFile message handlers


