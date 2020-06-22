// EditProfileDlg.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "EditProfileDlg.h"
#include "afxdialogex.h"
#include "EditPasswordDlg.h"
#include "ConfirmDlg.h"


// EditProfileDlg dialog

IMPLEMENT_DYNAMIC(EditProfileDlg, CDialog)

EditProfileDlg::EditProfileDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_EditProfileDlg, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}
EditProfileDlg::EditProfileDlg(AccountManagement* accMa, Account* account)
	: CDialog(IDD_EditProfileDlg, nullptr)
	, accMa(accMa)
	, account(account)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

EditProfileDlg::~EditProfileDlg()
{
}

void EditProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EditProfileDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_BTN_EDIT_PASSWORD, &EditProfileDlg::OnBnClickedBtnEditPassword)
	ON_BN_CLICKED(ID_BTN_ABOUT, &EditProfileDlg::OnBnClickedBtnAbout)
END_MESSAGE_MAP()


BOOL EditProfileDlg::OnInitDialog()
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

void EditProfileDlg::OnPaint()
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
// EditProfileDlg message handlers


void EditProfileDlg::OnBnClickedBtnEditPassword()
{
	return;
	// TODO: Add your control notification handler code here
	EditPasswordDlg editPass(accMa, account);
	editPass.DoModal();
	accMa->UpdateAccountPassword(*account);
}


void EditProfileDlg::OnBnClickedBtnAbout()
{
	// TODO: Add your control notification handler code here
	ConfirmDlg confirm;
	confirm.SetMess(L"Messenger: Version 1.0");
	confirm.DoModal();
}
