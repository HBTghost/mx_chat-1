﻿
// ClientChatGUIDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ClientChatGUI.h"
#include "ClientChatGUIDlg.h"
#include "afxdialogex.h"
#include "ClientStructure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientChatGUIDlg dialog



CClientChatGUIDlg::CClientChatGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENTCHATGUI_DIALOG, pParent)
	, m_log_client(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientChatGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_PRIVATE, m_msg_chat_private);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_username);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_password);
	DDX_Control(pDX, IDC_EDIT_USER_CHAT_PRIVATE, m_user_chat_private);
	DDX_Text(pDX, IDC_LOG_CLIENT, m_log_client);
}

BEGIN_MESSAGE_MAP(CClientChatGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(ID_CLIENT_RECEIVE_MESSAGE, OnReceiveMsg)

	ON_BN_CLICKED(IDC_BTN_SIGNUP, &CClientChatGUIDlg::OnBnClickedBtnSignup)
	ON_BN_CLICKED(IDC_BTN_SIGNIN, &CClientChatGUIDlg::OnBnClickedBtnSignin)
	ON_BN_CLICKED(IDC_BTN_CHATPRIVATE, &CClientChatGUIDlg::OnBnClickedBtnChatprivate)
	ON_BN_CLICKED(IDC_BTN_SEND_PRIVATE, &CClientChatGUIDlg::OnBnClickedBtnSendPrivate)
END_MESSAGE_MAP()



afx_msg LRESULT CClientChatGUIDlg::OnReceiveMsg(WPARAM wParam, LPARAM lParam) {
	WCHAR* message = (WCHAR*)wParam;
	switch (message[0])
	{
	case 1:
		message++;
		int	len = wcslen(message);
		WCHAR* partner;
		WCHAR* content;

		partner = message;
		content = partner + wcslen(partner) + 1;

		UpdateData(TRUE);

		CString nLog;

		nLog.Format(_T("partner [ %s ] nội dung [%s]\r\n"),partner, content);
		m_log_client.Append(nLog);
		UpdateData(FALSE);

		
		

		break;
	}
	return 1;
}

// CClientChatGUIDlg message handlers

BOOL CClientChatGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientChatGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientChatGUIDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientChatGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientChatGUIDlg::OnBnClickedBtnSignup()
{
	// TODO: Add your control notification handler code here
}


void CClientChatGUIDlg::OnBnClickedBtnSignin()
{
	// TODO: Add your control notification handler code here
}


void CClientChatGUIDlg::OnBnClickedBtnChatprivate()
{
	// TODO: Add your control notification handler code here
}


void CClientChatGUIDlg::OnBnClickedBtnSendPrivate()
{
	// TODO: Add your control notification handler code here
}
