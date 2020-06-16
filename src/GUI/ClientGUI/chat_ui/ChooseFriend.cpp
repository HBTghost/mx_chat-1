// ChooseFriend.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "ChooseFriend.h"
#include "afxdialogex.h"


// ChooseFriend dialog

IMPLEMENT_DYNAMIC(ChooseFriend, CDialog)

ChooseFriend::ChooseFriend(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ChooseFriend, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

ChooseFriend::~ChooseFriend()
{
}

void ChooseFriend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChooseFriend, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &ChooseFriend::OnBnClickedOk)
	ON_BN_CLICKED(ID_BTN_EDIT_PASSWORD, &ChooseFriend::OnBnClickedBtnEditPassword)
	ON_BN_CLICKED(ID_BTN_START_CHAT, &ChooseFriend::OnBnClickedBtnStartChat)
	ON_BN_CLICKED(ID_BTN_UNFRIEND, &ChooseFriend::OnBnClickedBtnUnfriend)
END_MESSAGE_MAP()


BOOL ChooseFriend::OnInitDialog()
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

void ChooseFriend::OnPaint()
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
// ChooseFriend message handlers


void ChooseFriend::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void ChooseFriend::OnBnClickedBtnEditPassword()
{
	// TODO: Add your control notification handler code here
}


void ChooseFriend::OnBnClickedBtnStartChat()
{
	// TODO: Add your control notification handler code here
	EndDialog(ID_BTN_START_CHAT);
	return;
}


void ChooseFriend::OnBnClickedBtnUnfriend()
{
	// TODO: Add your control notification handler code here
	EndDialog(ID_BTN_UNFRIEND);
	return;
}
