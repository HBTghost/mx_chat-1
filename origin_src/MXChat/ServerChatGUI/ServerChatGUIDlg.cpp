﻿
// ServerChatGUIDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ServerChatGUI.h"
#include "ServerChatGUIDlg.h"
#include "afxdialogex.h"
#include "ServerStructure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region  CABOUT_DLG

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


// CServerChatGUIDlg dialog

#pragma endregion


// inside the class declaration
afx_msg LRESULT CServerChatGUIDlg::OnUserDefinedMessage(WPARAM wParam, LPARAM lParam) {
	//wstring* pString = (wstring*)lParam;
	//LPCTSTR str = pString->c_str();                 // i.e. std::wstring to wchar_t*
	SClientPacket* packet = (SClientPacket*)wParam;
	CString nLog;

	nLog.Format(_T("Tài khoản [ %s ] đã kết nối"), packet->username.c_str());
	UpdateData(TRUE);

	m_strServerLog.Append(nLog);
	m_strServerLog.Append(L"\r\n");
	UpdateData(FALSE);
	AfxMessageBox((packet->username.c_str()));

	// Do something with pString... You can even pass it to a CStringW constructor. 
	//delete[] pString; // and don't forget this!

	//AfxMessageBox(_T("On user defined"));
	return 1;
}


afx_msg LRESULT CServerChatGUIDlg::OnConnectedMsg(WPARAM wParam, LPARAM lParam) {
	CString nLog = _T("Có thêm 1 Client đã kết nối \r\n");

	UpdateData(TRUE);
	m_strServerLog.Append(nLog);
	UpdateData(FALSE);
	return 1;
}

afx_msg LRESULT CServerChatGUIDlg::OnSignupMsg(WPARAM wParam, LPARAM lParam) {
	Account* account = (Account*)wParam;
	if (account == nullptr) {
		return -1; 
	}
	CString nLog;
	nLog.Format(_T("Đăng ký :  Tài khoản -  [%s] |  Mật khẩu [%s] \r\n"), account->getUserName().c_str(), account->getPassWord().c_str());

	UpdateData(TRUE);
	m_strServerLog.Append(nLog);
	UpdateData(FALSE);
	return 1;
}
afx_msg LRESULT CServerChatGUIDlg::OnDisConnectedMsg(WPARAM wParam, LPARAM lParam) {
	CString nLog = _T("Có 1 Client đã ngắt kết nối \r\n");
	UpdateData(TRUE);
	m_strServerLog.Append(nLog);
	UpdateData(FALSE);
	return 1;
}
CServerChatGUIDlg::CServerChatGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVERCHATGUI_DIALOG, pParent)
	, m_strServerLog(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerChatGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SERVER_LOG, m_strServerLog);
}

BEGIN_MESSAGE_MAP(CServerChatGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(ID_CLIENT_CONNECTED_MESSAGE, OnConnectedMsg)
	ON_MESSAGE(ID_CLIENT_DISCONNECTED_MESSAGE, OnDisConnectedMsg)
	ON_MESSAGE(ID_CLIENT_SIGNUP_MESSAGE, OnSignupMsg)
	ON_BN_CLICKED(IDC_BUTTON1, &CServerChatGUIDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CServerChatGUIDlg message handlers

BOOL CServerChatGUIDlg::OnInitDialog()
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

void CServerChatGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerChatGUIDlg::OnPaint()
{
	/*
Init socket
*/

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
HCURSOR CServerChatGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerChatGUIDlg::OnBnClickedButton1()
{
	WCHAR* message;
	WCHAR temp[4096];
	temp[0] = 1;
	temp[1] = 'a';
	temp[2] = 'b';
	temp[3] = '\0';
	temp[4] = 'c';
	temp[5] = 'o';
	temp[6] = 'n';
	temp[7] = 't';
	temp[8] = 'e';
	temp[9] = 'n';
	temp[10] = 't';
	temp[11] = '\0';

	message = temp;
	gServerObj.SendPackageClientAll(message, 12);
	
	/*
	CWnd* c = gServerObj.getCWND();
	std::wstring msg(L"dulieu1");
	std::wstring msg2(L"dulieu2");
	SClientPacket* packet = new SClientPacket();
	packet->username = L"admin";
	//packet->username = L"usera";


	c->SendMessage(ID_CLIENT_CONNECTED_MESSAGE, (WPARAM)packet);
	*/
	// TODO: Add your control notification handler code here
}
