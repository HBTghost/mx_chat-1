// ConfirmDlg.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "ConfirmDlg.h"
#include "afxdialogex.h"


// ConfirmDlg dialog

IMPLEMENT_DYNAMIC(ConfirmDlg, CDialog)

ConfirmDlg::ConfirmDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ConfirmDlg, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

ConfirmDlg::ConfirmDlg(std::wstring message)
	: CDialog(IDD_ConfirmDlg, nullptr)
	, message(message)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

ConfirmDlg::~ConfirmDlg()
{
}

void ConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONFIRM_MESS, mess);
	mess.SetWindowTextW(message.c_str());
}


BEGIN_MESSAGE_MAP(ConfirmDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &ConfirmDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ConfirmDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL ConfirmDlg::OnInitDialog()
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

void ConfirmDlg::OnPaint()
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
// ConfirmDlg message handlers


void ConfirmDlg::SetMess(std::wstring message)
{
	this->message = message;
}

void ConfirmDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void ConfirmDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
