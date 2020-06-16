#pragma once


// ChooseFriend dialog

class ChooseFriend : public CDialog
{
	DECLARE_DYNAMIC(ChooseFriend)

public:
	ChooseFriend(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ChooseFriend();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ChooseFriend };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnEditPassword();
	afx_msg void OnBnClickedBtnStartChat();
	afx_msg void OnBnClickedBtnUnfriend();
};
