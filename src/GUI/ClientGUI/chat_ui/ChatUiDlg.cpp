
// ChatUiDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "chat_ui.h"
#include "ChatUiDlg.hpp"
#include "afxdialogex.h"
#include <string>
#include "messenger.h"
#include "Tools.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ChatUiDlg dialog

ChatUiDlg::ChatUiDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CHAT_UI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

ChatUiDlg::ChatUiDlg(CString _username)
	: CDialog(IDD_CHAT_UI_DIALOG, nullptr)
	, _username(_username)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);

}

void ChatUiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERNAME, username);
	DDX_Control(pDX, IDC_TITLE, title);

	CFont font;
	font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial"));              // lpszFacename
	title.SetFont(&font);

	DDX_Control(pDX, IDC_PASSWORD, password);


	if (!isInit) {
		OnCancel();
	}
}

BEGIN_MESSAGE_MAP(ChatUiDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOGIN, &ChatUiDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_REGISTER, &ChatUiDlg::OnBnClickedBtnRegister)
	ON_EN_UPDATE(IDC_USERNAME, &ChatUiDlg::OnEnUpdateUsername)
	ON_EN_CHANGE(IDC_PASSWORD, &ChatUiDlg::OnEnChangePassword)

	ON_MESSAGE(IDC_FORM_LOGIN_MSG_HANDLER, &ChatUiDlg::OnFormLoginMsgHandler)


END_MESSAGE_MAP()

LRESULT ChatUiDlg::OnFormLoginMsgHandler(WPARAM wParam, LPARAM lParam) {
	UINT command_msg = (UINT)wParam;
	switch (command_msg)
	{
	case IDC_FORM_LOGIN_MSG_HANDLER_LOGIN_SUCESS: {
		MessageBox(_T("Login successfully. Enjoy this app!!!"), _T("Alert"), MB_ICONINFORMATION);

		//this->ShowWindow(false);
		//OnOK();
		messenger mess(this->m_ClientService);
		//this->m_ClientService->AddHwnd(mess.GetSafeHwnd());
		//this->ShowWindow(SW_HIDE);
		this->ShowWindow(FALSE);
		mess.DoModal();
		this->ShowWindow(TRUE);
		password.SetWindowTextW(_T("\r\r\r"));
		break;
	}
	case IDC_FORM_LOGIN_MSG_HANDLER_LOGIN_ERROR:
		MessageBox(_T("Login error, please check username or password!!!"), _T("Alert"), MB_ICONERROR);
		break;
	case IDC_FORM_LOGIN_MSG_HANDLER_REGISTER_SUCCESS:
		MessageBox(_T("Register successfuly. Click Login to continue"), _T("Alert"), MB_ICONINFORMATION);
		break;
	case IDC_FORM_LOGIN_MSG_HANDLER_REGISTER_ERROR:
		MessageBox(_T("Register error. Username existed, choose another username to continue !!!"), _T("Alert"), MB_ICONERROR);
		break;



	default:
		break;
	}
	return 0L;
}

// ChatUiDlg message handlers

BOOL ChatUiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	username.SetWindowTextW(_username);
	this->m_ClientService->AddHwnd(this->GetSafeHwnd());
	this->m_ClientService->CreateWorkerThread();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ChatUiDlg::OnPaint()
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

void ChatUiDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	bool draw = true;
	COLORREF color;
	if (nIDCtl == IDC_BTN_REGISTER) {
		color = RGB(175, 122, 197);
	}
	else if (nIDCtl == IDC_BTN_LOGIN) {
		color = RGB(72, 201, 176);
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
		dc.SetTextColor(RGB(255, 255, 255));
		dc.SetBkMode(TRANSPARENT);

		TCHAR buffer[MAX_PATH];
		ZeroMemory(buffer, MAX_PATH);
		::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);
		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		dc.Detach();
	}

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ChatUiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ChatUiDlg::OnBnClickedBtnLogin()
{
	// TODO: Add your control notification handler code here
	CString _username, _password;
	GetDlgItemText(IDC_USERNAME, _username);
	GetDlgItemText(IDC_PASSWORD, _password);


	CT2A ascii_user(_username);
	CT2A ascii_pass(_password);
	string s_user = ascii_user.m_psz;
	string s_pass = ascii_pass.m_psz;

	std::wstring username(_username), password(_password);




	if (_username == _T("")) {
		MessageBox(_T("Username must not be empty!"), _T("Alert"), MB_ICONERROR);
	}
	else if (_password == _T("")) {
		MessageBox(_T("Password must not be empty!"), _T("Alert"), MB_ICONERROR);
	}
	else {
		this->m_ClientService->Login(s_user, s_pass);
		/*
		int flag = accMa.CheckAccount(Account(username, password));
		if (flag == RIGHT_PASSWORD) {
			MessageBox(_T("Logged in successfully!"), _T("Alert"), MB_ICONINFORMATION);
			OnOK();
			messenger mess(username);
			mess.DoModal();
		}
		else if (flag == WRONG_PASSWORD) {
			MessageBox(_T("Wrong password!"), _T("Alert"), MB_ICONWARNING);
		}
		else {
			MessageBox(_T("Account does not exist!"), _T("Alert"), MB_ICONERROR);
		}
		*/
	}
}


void ChatUiDlg::OnBnClickedBtnRegister()
{
	// TODO: Add your control notification handler code here
	CString _username, _password;
	GetDlgItemText(IDC_USERNAME, _username);
	GetDlgItemText(IDC_PASSWORD, _password);


	CT2A ascii_user(_username);
	CT2A ascii_pass(_password);
	string s_user = ascii_user.m_psz;
	string s_pass = ascii_pass.m_psz;

	std::wstring username(_username), password(_password);




	if (_username == _T("")) {
		MessageBox(_T("Username must not be empty!"), _T("Alert"), MB_ICONERROR);
	}
	else if (_password == _T("")) {
		MessageBox(_T("Password must not be empty!"), _T("Alert"), MB_ICONERROR);
	}
	else {
		this->m_ClientService->Register(s_user, s_pass);
		GetDlgItem(IDC_BTN_LOGIN)->SetFocus();
	}

	/*
	// TODO: Add your control notification handler code here
	CString _username, _password;
	GetDlgItemText(IDC_USERNAME, _username);
	GetDlgItemText(IDC_PASSWORD, _password);
	std::wstring username(_username), password(_password);

	if (_username == _T("")){
		MessageBox(_T("Username must not be empty!"), _T("Alert"), MB_ICONERROR);
	}
	else if (_password == _T("")) {
		MessageBox(_T("Password must not be empty!"), _T("Alert"), MB_ICONERROR);
	}
	else {
		if (accMa.AddAccount(Account(username, password))) {
			MessageBox(_T("Account registration successful!"), _T("Alert"), MB_ICONINFORMATION);
		}
		else {
			MessageBox(_T("Username already exists!"), _T("Alert"), MB_ICONERROR);
		}
	}
	*/
}


void ChatUiDlg::OnEnUpdateUsername()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	CString tmp;
	GetDlgItemText(IDC_USERNAME, tmp);
	std::wstring _username(tmp);
	if (_username.size() > 10) {
		MessageBox(_T("Username must smaller than or equal 10 characters!"), _T("Alert"), MB_ICONERROR);
		tmp.Delete(tmp.GetLength() - 1, 1);
		username.SetWindowTextW(tmp);
		username.SetSel(-1);
	}
	if (_username.find_first_not_of(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos) {
		MessageBox(_T("Username must contain a-z, A-Z, 0-9 and _ character only!"), _T("Alert"), MB_ICONERROR);
		tmp.Delete(tmp.GetLength() - 1, 1);
		username.SetWindowTextW(tmp);
		username.SetSel(-1);
	}
}


void ChatUiDlg::OnEnChangePassword()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString tmp;
	GetDlgItemText(IDC_PASSWORD, tmp);
	std::wstring _password(tmp);
	if (_password.find_first_of(L",") != std::string::npos) {
		MessageBox(_T("Password must not contain ',' character!"), _T("Alert"), MB_ICONERROR);
		tmp.Delete(tmp.GetLength() - 1, 1);
		password.SetWindowTextW(tmp);
		password.SetSel(-1);
	}
	if (tmp == _T("\r\r\r")) {
		OnCancel();
	}
}
