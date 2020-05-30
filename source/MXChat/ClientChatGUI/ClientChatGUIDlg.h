
// ClientChatGUIDlg.h : header file
//

#pragma once
#include "ClientSocket.h"
#include "ClientBackgroundService.h"
#include <iostream>
#include <xstring>

// CClientChatGUIDlg dialog
class CClientChatGUIDlg : public CDialogEx
{
// Construction
public:
	CClientChatGUIDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTCHATGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	LRESULT OnReceiveMsg(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	void OnPaintGoc();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_msg_chat_private;
	CEdit m_username;
	CEdit m_password;
	CEdit m_user_chat_private;
	afx_msg void OnBnClickedBtnSignup();
	afx_msg void OnBnClickedBtnSignin();
	afx_msg void OnBnClickedBtnChatprivate();
	afx_msg void OnBnClickedBtnSendPrivate();
	CString m_log_client;
};
