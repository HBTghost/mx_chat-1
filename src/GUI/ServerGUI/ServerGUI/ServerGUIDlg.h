
// ServerGUIDlg.h : header file
//

#pragma once
#include "Account.h"


// CServerGUIDlg dialog
class CServerGUIDlg : public CDialogEx
{
// Construction
public:
	CServerGUIDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVERGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	friend int initServer();
	HWND* GetHwnd() {
		HWND win = this->GetSafeHwnd();
		return &win; 
	}
	afx_msg LRESULT OnCommandIdTestMsg(WPARAM wParam, LPARAM lParam);


public:
	CString m_ServerIP;
	CString m_ServerPort;
	afx_msg void OnBnClickedButtonListen();


	CString m_ServerLogger;
};
