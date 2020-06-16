#pragma once


// AttachFile

class AttachFile : public CFileDialog
{
	DECLARE_DYNAMIC(AttachFile)

public:
	AttachFile(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = nullptr,
		LPCTSTR lpszFileName = nullptr,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = nullptr,
		CWnd* pParentWnd = nullptr);
	virtual ~AttachFile();

protected:
	DECLARE_MESSAGE_MAP()
};


