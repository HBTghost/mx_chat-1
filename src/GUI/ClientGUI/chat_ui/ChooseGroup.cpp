// ChooseGroup.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "ChooseGroup.h"
#include "afxdialogex.h"
#include "CreateGroupDlg.h"
#include "ConfirmDlg.h"


// ChooseGroup dialog

IMPLEMENT_DYNAMIC(ChooseGroup, CDialog)

ChooseGroup::ChooseGroup(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ChooseGroup, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

ChooseGroup::ChooseGroup(AccountManagement* accMa, Account* account, std::vector<std::wstring> groups_name)
	: CDialog(IDD_ChooseGroup, nullptr)
	, accMa(accMa)
	, account(account)
	, groups_name(groups_name)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

ChooseGroup::~ChooseGroup()
{
}

void ChooseGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	if (groups_name.size() > 1) {
		GetDlgItem(ID_BTN_START_CHAT_GROUP)->EnableWindow(FALSE);
		GetDlgItem(ID_BTN_MODIFY_GROUP)->EnableWindow(FALSE);
	}
}


BEGIN_MESSAGE_MAP(ChooseGroup, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_BTN_START_CHAT_GROUP, &ChooseGroup::OnBnClickedBtnStartChatGroup)
	ON_BN_CLICKED(ID_BTN_OUT_GROUP, &ChooseGroup::OnBnClickedBtnOutGroup)
	ON_BN_CLICKED(ID_BTN_MODIFY_GROUP, &ChooseGroup::OnBnClickedBtnModifyGroup)
END_MESSAGE_MAP()


BOOL ChooseGroup::OnInitDialog()
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

void ChooseGroup::OnPaint()
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

// ChooseGroup message handlers


void ChooseGroup::OnBnClickedBtnStartChatGroup()
{
	// TODO: Add your control notification handler code here
	EndDialog(ID_BTN_START_CHAT_GROUP);
	return;
}


void ChooseGroup::OnBnClickedBtnOutGroup()
{
	// TODO: Add your control notification handler code here
	std::wstring mess;
	mess.clear();
	for (auto x : groups_name) {
		mess += L"\"" + x + L"\"" + L", ";
	}
	mess.erase(mess.end() - 2, mess.end());
	mess = L"After this action, you will not be in (" + mess + L") group anymore!\r\n\r\nAre you sure?";
	ConfirmDlg confirmDlg;
	confirmDlg.SetMess(mess);
	INT_PTR nRet;
	nRet = confirmDlg.DoModal();

	// Handle the return value from DoModal
	switch (nRet)
	{
	case -1:
		AfxMessageBox(_T("Dialog box could not be created!"));
		break;
	case IDABORT:
		// Do something
		break;
	case IDOK:
		accMa->RemoveAccountFromGroups(*account, groups_name);
		EndDialog(ID_BTN_OUT_GROUP);
		break;
	case IDCANCEL:
		break;
	default:
		// Do something
		break;
	}

}


void ChooseGroup::OnBnClickedBtnModifyGroup()
{
	// TODO: Add your control notification handler code here
	CreateGroupDlg createGroupDlg(accMa, account, groups_name[0]);
	createGroupDlg.DoModal();
	EndDialog(ID_BTN_MODIFY_GROUP);
	return;
}
