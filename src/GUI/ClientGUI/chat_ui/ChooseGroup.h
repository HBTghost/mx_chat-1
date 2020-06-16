#pragma once
#include "Account.hpp"

// ChooseGroup dialog

class ChooseGroup : public CDialog
{
	DECLARE_DYNAMIC(ChooseGroup)

public:
	ChooseGroup(CWnd* pParent = nullptr);   // standard constructor
	ChooseGroup(AccountManagement* accMa, Account* account, std::vector<std::wstring> groups_name);
	virtual ~ChooseGroup();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ChooseGroup };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	AccountManagement* accMa;
	Account* account;
	std::vector < std::wstring> groups_name;
	afx_msg void OnBnClickedBtnStartChatGroup();
	afx_msg void OnBnClickedBtnOutGroup();
	afx_msg void OnBnClickedBtnModifyGroup();
};
