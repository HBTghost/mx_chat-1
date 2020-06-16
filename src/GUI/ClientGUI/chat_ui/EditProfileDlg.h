#pragma once
#include "Account.hpp"

// EditProfileDlg dialog

class EditProfileDlg : public CDialog
{
	DECLARE_DYNAMIC(EditProfileDlg)

public:
	EditProfileDlg(CWnd* pParent = nullptr);   // standard constructor
	EditProfileDlg(AccountManagement* accMa, Account* account);
	virtual ~EditProfileDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EditProfileDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnEditPassword();
	Account* account;
	AccountManagement* accMa;
};
