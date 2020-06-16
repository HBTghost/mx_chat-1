#pragma once
#include "Account.hpp"

// EditPasswordDlg dialog

class EditPasswordDlg : public CDialog
{
	DECLARE_DYNAMIC(EditPasswordDlg)

public:
	EditPasswordDlg(CWnd* pParent = nullptr);   // standard constructor
	EditPasswordDlg(AccountManagement* accMa, Account* account);
	virtual ~EditPasswordDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EditPasswordDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	Account* account;
	AccountManagement* accMa;
	CEdit password_cur;
	CEdit password1;
	CEdit password2;
	afx_msg void OnEnChangeCurPass();
	afx_msg void OnEnChangePass1();
	afx_msg void OnEnChangePass2();
};
