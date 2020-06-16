#pragma once
#include <vector>
#include <string>

// ShowEmoijDlg dialog

class ShowEmoijDlg : public CDialog
{
	DECLARE_DYNAMIC(ShowEmoijDlg)

public:
	ShowEmoijDlg(CWnd* pParent = nullptr);   // standard constructor
	ShowEmoijDlg(CEdit* mess, CString type, std::wstring username);
	void HandleRecent();
	virtual ~ShowEmoijDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ShowEmoijDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString type;
	DECLARE_MESSAGE_MAP()

public:
	CEdit* mess;
	std::vector<std::wstring> list_icon;
	std::vector<CString> recent;
	void InitListIcon();
	std::wstring username;
	CListCtrl list_icon_ctrl;
	afx_msg void OnNMClickListIcon(NMHDR* pNMHDR, LRESULT* pResult);
};
