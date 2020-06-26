#pragma once
#include <string>

// EmojiDlg dialog

class EmojiDlg : public CDialog
{
	DECLARE_DYNAMIC(EmojiDlg)

public:
	EmojiDlg(CWnd* pParent = nullptr);   // standard constructor
	EmojiDlg(CEdit* mess, std::wstring username);
	EmojiDlg(bool color);
	bool color = false;
	virtual ~EmojiDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EmojiDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
public:
	void InitList();
	CListCtrl list_emoji;
	std::wstring username;
	CEdit* mess;
	int color_val = -1;
	afx_msg void OnNMClickListEmoji(NMHDR* pNMHDR, LRESULT* pResult);
};
