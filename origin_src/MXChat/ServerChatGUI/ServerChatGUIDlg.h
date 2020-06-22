
// ServerChatGUIDlg.h : header file
//

#pragma once
#include "ServerSocket.h"
#include "ServerBackgroundService.h"
#include <iostream>
#include <xstring>


// CServerChatGUIDlg dialog
class CServerChatGUIDlg : public CDialogEx
{
// Construction
public:
	LRESULT OnConnectedMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnSignupMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnDisConnectedMsg(WPARAM wParam, LPARAM lParam);
	CServerChatGUIDlg(CWnd* pParent = nullptr);	// standard constructor

	afx_msg LRESULT OnUserDefinedMessage(WPARAM wParam, LPARAM lParam);

	//LRESULT OnUserClientConnectMsg(WPARAM wParam, LPARAM lParam);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVERCHATGUI_DIALOG };
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
public:
	afx_msg void OnBnClickedButton1();
	CString m_strServerLog;
};
