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
	CreateGroupDlg(std::wstring username, std::vector<std::wstring> members, std::vector<std::wstring> friends);
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
	Account* account;
	AccountManagement* accMa;
	std::vector<Group> groups;
	std::wstring username;
	std::vector<std::wstring> friends;
	std::vector<std::wstring> members;
	CListCtrl list_members;
	CListCtrl list_friends;
	void SetSwitchBox();
	void SetBtnIcon(int btnId, int iconId, int size);
	//	afx_msg void OnNMDblclkListFriends(NMHDR* pNMHDR, LRESULT* pResult);
	//	afx_msg void OnNMDblclkListMembers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnUpdateGroupName();
	CEdit groupname;
	Group group;
	afx_msg void OnBnClickedBtnCreateGroup();
	afx_msg void OnNMSetfocusListFriends(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickListMembers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickListFriends(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMSetfocusListMembers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnSwap();
	afx_msg void OnBnClickedBtnLeft();
	afx_msg void OnBnClickedBtnRight();
	CString _groupname;
	CString GetGroupName() { return _groupname; }
	std::vector<std::wstring> GetMembers() { return members; }
};
