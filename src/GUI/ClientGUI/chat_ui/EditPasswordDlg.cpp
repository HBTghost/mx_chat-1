// EditPasswordDlg.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "EditPasswordDlg.h"
#include "afxdialogex.h"
#include <string>
#include "Account.hpp"

// EditPasswordDlg dialog

IMPLEMENT_DYNAMIC(EditPasswordDlg, CDialog)

EditPasswordDlg::EditPasswordDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_EditPasswordDlg, pParent)
{
}

EditPasswordDlg::EditPasswordDlg(AccountManagement* accMa, Account* account)
	: CDialog(IDD_EditPasswordDlg, nullptr)
	, accMa(accMa)
	, account(account)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

EditPasswordDlg::~EditPasswordDlg()
{
}

void EditPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUR_PASS, password_cur);
	DDX_Control(pDX, IDC_PASS1, password1);
	DDX_Control(pDX, IDC_PASS2, password2);
}


BEGIN_MESSAGE_MAP(EditPasswordDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &EditPasswordDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &EditPasswordDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_CUR_PASS, &EditPasswordDlg::OnEnChangeCurPass)
	ON_EN_CHANGE(IDC_PASS1, &EditPasswordDlg::OnEnChangePass1)
	ON_EN_CHANGE(IDC_PASS2, &EditPasswordDlg::OnEnChangePass2)
END_MESSAGE_MAP()


BOOL EditPasswordDlg::OnInitDialog()
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

void EditPasswordDlg::OnPaint()
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
// EditPasswordDlg message handlers


void EditPasswordDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString _cur_pass, _pass1, _pass2;
	GetDlgItemText(IDC_CUR_PASS, _cur_pass);
	GetDlgItemText(IDC_PASS1, _pass1);
	GetDlgItemText(IDC_PASS2, _pass2);
	std::wstring cur_pass(_cur_pass), pass1(_pass1), pass2(_pass2);

	if (!account->IsMatchPassword(cur_pass)) {
		MessageBox(_T("Wrong current password!"), _T("Alert"), MB_ICONERROR);
	}
	else if (_pass1 == _T("")) {
		MessageBox(_T("New password must not be empty!"), _T("Alert"), MB_ICONERROR);
	}
	else if (_pass1 != _pass2) {
		MessageBox(_T("Two new password does not match!"), _T("Alert"), MB_ICONERROR);
	}
	else {
		account->UpdatePassword(pass1);
		accMa->UpdateAccountPassword(*account);
		MessageBox(_T("Password change successfully!"), _T("Alert"), MB_ICONINFORMATION);
		CDialog::OnOK();
	}
}


void EditPasswordDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}


void EditPasswordDlg::OnEnChangeCurPass()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString tmp;
	GetDlgItemText(IDC_CUR_PASS, tmp);
	std::wstring _password(tmp);
	if (_password.find_first_of(L",") != std::string::npos) {
		MessageBox(_T("Password must not contain ',' character!"), _T("Alert"), MB_ICONERROR);
		tmp.Delete(tmp.GetLength() - 1, 1);
		password_cur.SetWindowTextW(tmp);
		password_cur.SetSel(-1);
	}
}


void EditPasswordDlg::OnEnChangePass1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	CString tmp;
	GetDlgItemText(IDC_PASS1, tmp);
	std::wstring _password(tmp);
	if (_password.find_first_of(L",") != std::string::npos) {
		MessageBox(_T("Password must not contain ',' character!"), _T("Alert"), MB_ICONERROR);
		tmp.Delete(tmp.GetLength() - 1, 1);
		password1.SetWindowTextW(tmp);
		password1.SetSel(-1);
	}
	// TODO:  Add your control notification handler code here
}


void EditPasswordDlg::OnEnChangePass2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	CString tmp;
	GetDlgItemText(IDC_PASS2, tmp);
	std::wstring _password(tmp);
	if (_password.find_first_of(L",") != std::string::npos) {
		MessageBox(_T("Password must not contain ',' character!"), _T("Alert"), MB_ICONERROR);
		tmp.Delete(tmp.GetLength() - 1, 1);
		password2.SetWindowTextW(tmp);
		password2.SetSel(-1);
	}
	// TODO:  Add your control notification handler code here
}
