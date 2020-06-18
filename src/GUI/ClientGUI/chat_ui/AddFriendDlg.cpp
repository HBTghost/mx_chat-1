// AddFriendDlg.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "AddFriendDlg.h"
#include "afxdialogex.h"


// AddFriendDlg dialog

IMPLEMENT_DYNAMIC(AddFriendDlg, CDialog)

AddFriendDlg::AddFriendDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_AddFriendDlg, pParent)
{
}

AddFriendDlg::AddFriendDlg(AccountManagement* accMa, Account* account)
	: CDialog(IDD_AddFriendDlg, nullptr)
	, accMa(accMa)
	, account(account)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

AddFriendDlg::~AddFriendDlg()
{
}

void AddFriendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AddFriendDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, &AddFriendDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &AddFriendDlg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL AddFriendDlg::OnInitDialog()
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

void AddFriendDlg::OnPaint()
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
// AddFriendDlg message handlers


void AddFriendDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}


void AddFriendDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString _friend_username, _greeting;
	GetDlgItemText(IDC_FRIEND_USERNAME, _friend_username);
	GetDlgItemText(IDC_GREETING, _greeting);
	std::wstring friend_username(_friend_username), greeting(_greeting);
	this->m_message_value = _greeting;
	OnOK();
	return;
	if (_friend_username == _T("")) {
		MessageBox(_T("Friend's username must not be empty!"), _T("Alert"), MB_ICONERROR);
	}
	else if (!accMa->AccountExisted(friend_username)) {
		MessageBox(_T("Friend's username does not exist!"), _T("Alert"), MB_ICONERROR);
	}
	else {
		if (account->IsMatchUsername(friend_username)) {
			MessageBox(_T("Friend's username and your username must be different!"), _T("Alert"), MB_ICONERROR);
		}
		else {
			std::vector<std::wstring> friends = accMa->GetFriends(*account);
			if (accMa->AddFriend(*account, friend_username)) {
				CString mess = _T("Add friend '") + _friend_username + "' successfully";
				this->m_message_value = mess;
				MessageBox(mess, _T("Alert"), MB_ICONINFORMATION);
				OnOK();
			} else {
				MessageBox(_T("This account is already your friends!"), _T("Alert"), MB_ICONWARNING);
			}
		}
	}
}
