#pragma once
#include "Account.hpp"

// AddFriendDlg dialog

class AddFriendDlg : public CDialog
{
	DECLARE_DYNAMIC(AddFriendDlg)

public:
	AddFriendDlg(CWnd* pParent = nullptr);   // standard constructor
	AddFriendDlg(AccountManagement* accma, Account* account);
	virtual ~AddFriendDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddFriendDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	Account* account;
	AccountManagement* accMa;
};
