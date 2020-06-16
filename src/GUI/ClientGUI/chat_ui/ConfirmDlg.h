#pragma once
#include <string>

// ConfirmDlg dialog

class ConfirmDlg : public CDialog
{
	DECLARE_DYNAMIC(ConfirmDlg)

public:
	ConfirmDlg(CWnd* pParent = nullptr);   // standard constructor
	ConfirmDlg(std::wstring message);
	virtual ~ConfirmDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ConfirmDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
	std::wstring message;
public:
	void SetMess(std::wstring message);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit mess;
};
