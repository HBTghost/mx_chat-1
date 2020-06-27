#pragma once


// CustomColorDlg dialog

class CustomColorDlg : public CDialog
{
	DECLARE_DYNAMIC(CustomColorDlg)

public:
	CustomColorDlg(CWnd* pParent = nullptr);   // standard constructor
	CustomColorDlg(COLORREF a = RGB(255, 255, 255), COLORREF b = RGB(255, 255, 255));
	virtual ~CustomColorDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CustomColorDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg void OnBnClickedColorA();
	afx_msg void OnBnClickedColorB();
	COLORREF a = RGB(255, 255, 255);
	COLORREF b = RGB(255, 255, 255);
};
