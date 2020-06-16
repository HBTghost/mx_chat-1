
// ChatUiDlg.hpp : header file
//

#pragma once
#include "Account.hpp"
#include "ClientBackgroundService.h"

// ChatUiDlg dialog
class ChatUiDlg : public CDialog
{
// Construction
public:
	ChatUiDlg(CWnd* pParent = nullptr);	// standard constructor
	ChatUiDlg(CString _username);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT_UI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();


	afx_msg LRESULT OnFormLoginMsgHandler(WPARAM wParam, LPARAM lParam);
	HWND* GetHwnd() {
		HWND win = this->GetSafeHwnd();
		return &win;
	}
public:
	AccountManagement accMa;
	CStatic title;
	CString _username;
	CEdit username;
	CEdit password;
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnRegister();
	afx_msg void OnEnUpdateUsername();
	afx_msg void OnEnChangePassword();

	ClientBackgroundService *m_ClientService;
};
