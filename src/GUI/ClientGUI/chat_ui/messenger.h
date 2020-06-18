#pragma once
#include "Account.hpp"
#include <map>
#include <unordered_map>

// messenger dialog

class messenger : public CDialog
{
	DECLARE_DYNAMIC(messenger)

public:
	messenger(CWnd* pParent = nullptr);   // standard constructor
	messenger(ClientBackgroundService* mClientService, CWnd* pParent = nullptr);
	messenger(wstring username);
	virtual ~messenger();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_messenger };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
public:
	 
	std::pair<std::string, ClientConversation*> *chatBoxTarget; //selected

	ClientBackgroundService* mClientService;
	LRESULT OnFormMsgHandler(WPARAM wParam, LPARAM lParam);


	unordered_map<string, ClientConversation*> mListChat;

	string current_hash = "";
	string current_des_name = "";
	
	void ShowFriends();
	void ShowGroups();
	void ShowGroupsClick();
	void ShowMessages();
	std::vector<CString> GetMessagesFromListMess();
	void SaveMessages();
	void SetUserIcon();
	void SetSendBtnIcon();
	void SetAddFriendIcon();
	void SetChatBoxTitle();
	void SetBtnIcon(int btnId, int iconId, int size);
	std::vector<CString> GetSelectedItemText(CListCtrl* plctrl);
	void StartChat(std::wstring item, bool isGroup = false);
	bool targetIsGroup = false;
	Account* account;
	std::vector<wstring> friends;
	std::wstring target;
	std::vector<Group> groups;
	CStatic username;
	CListCtrl list_friends;
	CEdit mess_content;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnSend();
	CListCtrl list_mess;
	afx_msg void OnBnClickedBtnAddFriend();
	int count = 0;
	afx_msg void OnBnClickedIcon();
	afx_msg void OnBnClickedBtnSendFile();
	afx_msg void OnEnChangeMessContent();
	CRect mess_rect;
	CRect list_mess_rect;
	afx_msg void OnEnUpdateMessContent();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_brush;
	afx_msg void OnRightClickListFriends(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListFriends(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnSendIcon();
	AccountManagement* accMa;
	CListCtrl list_groups;
	afx_msg void OnDoubleClickListGroups(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightClickListGroups(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnNotification();
	bool notification = true;
	afx_msg void OnBnClickedBtnAddGroup();


};
