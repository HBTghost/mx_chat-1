// CustomColorDlg.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "CustomColorDlg.h"
#include "afxdialogex.h"


// CustomColorDlg dialog

IMPLEMENT_DYNAMIC(CustomColorDlg, CDialog)

CustomColorDlg::CustomColorDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CustomColorDlg, pParent)
{

}

CustomColorDlg::CustomColorDlg(COLORREF a, COLORREF b )
	: CDialog(IDD_CustomColorDlg, nullptr)
	, a(a)
	, b(b)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

CustomColorDlg::~CustomColorDlg()
{
}

void CustomColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CustomColorDlg, CDialog)
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_COLOR_A, &CustomColorDlg::OnBnClickedColorA)
	ON_BN_CLICKED(IDC_COLOR_B, &CustomColorDlg::OnBnClickedColorB)
END_MESSAGE_MAP()



BOOL CustomColorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CustomColorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


void CustomColorDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	bool draw = true;
	COLORREF color;
	if (nIDCtl == IDC_COLOR_A) {
		color = a;
	}
	else if (nIDCtl == IDC_COLOR_B) {
		color = b;
	}
	else {
		draw = false;
	}

	if (draw)
	{
		CDC dc;

		dc.Attach(lpDrawItemStruct->hDC);
		RECT rect;
		rect = lpDrawItemStruct->rcItem;

		dc.FillSolidRect(&rect, color);
		UINT state = lpDrawItemStruct->itemState;

		if ((state & ODS_SELECTED))
		{
			dc.DrawEdge(&rect, EDGE_BUMP, BF_RECT);

		}

		//dc.SetBkColor(RGB(255, 0, 0));
		dc.SetTextColor(RGB(0, 0, 0));
		dc.SetBkMode(TRANSPARENT);

		TCHAR buffer[MAX_PATH];
		ZeroMemory(buffer, MAX_PATH);
		::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);
		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		dc.Detach();
	}

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

// CustomColorDlg message handlers

void CustomColorDlg::OnBnClickedColorA()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		a = dlg.GetColor();
		this->RedrawWindow();
	}
}


void CustomColorDlg::OnBnClickedColorB()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		b = dlg.GetColor();
		this->RedrawWindow();
	}
}
