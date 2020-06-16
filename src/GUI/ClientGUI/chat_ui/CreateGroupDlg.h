#pragma once
#include "Account.hpp"
#include <string>

// CreateGroupDlg dialog

class CreateGroupDlg : public CDialog
{
	DECLARE_DYNAMIC(CreateGroupDlg)

public:
	CreateGroupDlg(CWnd* pParent = nullptr);   // standard constructor
	CreateGroupDlg(AccountManagement* accMa, Account* account, std::vector<std::wstring> members = {});
	CreateGroupDlg(AccountManagement* accMa, Account* account, std::wstring group_name);
	virtual ~CreateGroupDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CreateGroupDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	CStatic arrow;
	Account* account;
	AccountManagement* accMa;
	std::vector<Group> groups;
	std::vector<std::wstring> friends;
	std::vector<std::wstring> members;
	CListCtrl list_members;
	CListCtrl list_friends;
	void SetArrowFont();
	void SetSwitchBox();
	afx_msg void OnNMDblclkListFriends(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkListMembers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnUpdateGroupName();
	CEdit groupname;
	Group group;
	afx_msg void OnBnClickedBtnCreateGroup();
};
