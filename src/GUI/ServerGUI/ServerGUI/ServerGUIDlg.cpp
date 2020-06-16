
// ServerGUIDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ServerGUI.h"
#include "ServerGUIDlg.h"
#include "afxdialogex.h"
#include "ServerBackgroundService.h"
#include "Logger.h"


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


// CServerGUIDlg dialog



int initServer() {
	char buf[4096];
	if (!gServerObj.IsConnected())
	{
		cout << "Failed to initialise server socket." << endl;
		LOG_INFO("Failed to init server socket");
		//MessageBox(0, L"\nFailed to initialise server socket.", 0, 0);4
		return 0;
	}
	LOG_INFO("initServer() Running!!!");

	AfxBeginThread(listenServerThread, 0);

	return 1;
}

CServerGUIDlg::CServerGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVERGUI_DIALOG, pParent)
	, m_ServerIP(_T("127.0.0.1"))
	, m_ServerPort(_T("8084"))
	, m_ServerLogger(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);



}

void CServerGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_ServerIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_ServerPort);
	DDX_Text(pDX, IDC_RICHEDIT_LOGGER, m_ServerLogger);
}

LRESULT CServerGUIDlg::OnCommandIdTestMsg(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);
	CString* message = (CString*)wParam;
	m_ServerLogger.Append(*message);
	UpdateData(false);
	return 0;
}
BEGIN_MESSAGE_MAP(CServerGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LISTEN, &CServerGUIDlg::OnBnClickedButtonListen)

	ON_MESSAGE(IDC_MSG_TEST, &CServerGUIDlg::OnCommandIdTestMsg)
END_MESSAGE_MAP()


// CServerGUIDlg message handlers

BOOL CServerGUIDlg::OnInitDialog()
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




	Logger* pLogger = NULL; // Create the object pointer for Logger Class
	pLogger = Logger::getInstance();
	pLogger->updateLogType(LOG_TYPE::BOTH_LOG);
	pLogger->setHwnd(*this->GetHwnd());

	initServer();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerGUIDlg::OnPaint()
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
HCURSOR CServerGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerGUIDlg::OnBnClickedButtonListen()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CT2A ascii_ip(m_ServerIP);
	CT2A ascii_port(m_ServerPort);
	char* ip = ascii_ip.m_psz;
	char* port = ascii_port.m_psz;
	//initServer();
	LOG_INFO("Init GUI Server 127.0.0.1 8084");
	//AfxMessageBox(L"Sign in ....");
	UpdateData(false);

}
