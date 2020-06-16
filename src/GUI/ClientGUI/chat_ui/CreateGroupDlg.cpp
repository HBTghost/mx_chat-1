// CreateGroupDlg.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "CreateGroupDlg.h"
#include "afxdialogex.h"


// CreateGroupDlg dialog

IMPLEMENT_DYNAMIC(CreateGroupDlg, CDialog)

CreateGroupDlg::CreateGroupDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CreateGroupDlg, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

CreateGroupDlg::CreateGroupDlg(AccountManagement* accMa, Account* account, std::vector<std::wstring> members)
	: CDialog(IDD_CreateGroupDlg, nullptr)
	, accMa(accMa)
	, account(account)
	, members(members)
{
	this->members.insert(this->members.begin(), account->GetUsername());
	groups = accMa->GetGroups(*account);
	friends = accMa->GetFriends(*account);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

CreateGroupDlg::CreateGroupDlg(AccountManagement* accMa, Account* account, std::wstring group_name)
	: CDialog(IDD_CreateGroupDlg, nullptr)
	, accMa(accMa)
	, account(account)
{
	friends = accMa->GetFriends(*account);
	groups = accMa->GetGroups(*account);
	for (size_t i = 0; i < groups.size(); ++i) {
		if (groups[i].name == group_name) {
			this->group = groups[i];
			for (size_t i = 0; i < group.members.size(); ++i) {
				if (group.members[i] == account->GetUsername()) {
					group.members.erase(group.members.begin() + i);
					group.members.insert(group.members.begin(), account->GetUsername());
					break;
				}
			}
			this->members = group.members;
			groups.erase(groups.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < friends.size(); ++i) {
		for (size_t j = 0; j < members.size(); ++j) {
			if (friends[i] == members[j]) {
				friends.erase(friends.begin() + i);
				--i;
				break;
			}
		}
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

CreateGroupDlg::~CreateGroupDlg()
{
}

void CreateGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ARROW, arrow);
	DDX_Control(pDX, IDC_LIST_MEMBERS, list_members);
	DDX_Control(pDX, IDC_LIST_FRIENDS, list_friends);
	DDX_Control(pDX, IDC_GROUP_NAME, groupname);

	SetArrowFont();

	list_friends.InsertColumn(0, _T(""), LVCFMT_LEFT, 200);
	list_members.InsertColumn(0, _T(""), LVCFMT_LEFT, 200);

	SetSwitchBox();
	if (group.name.size()) {
		groupname.SetWindowTextW(group.name.c_str());
		groupname.SetSel(-1);
		GetDlgItem(IDC_BTN_CREATE_GROUP)->SetWindowTextW(TEXT("Modify"));
	}
}

void CreateGroupDlg::SetArrowFont() {
	CFont font;
	font.CreateFont(
		80,                        // nHeight
		100,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_EXTRABOLD,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ARABIC_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial"));              // lpszFacename
	arrow.SetFont(&font);
	arrow.SetWindowTextW(TEXT("⇆"));
}

void CreateGroupDlg::SetSwitchBox()
{
	// Set members
	CString strItem = _T("");
	if (list_members.GetItemCount() != members.size()) {
		list_members.DeleteAllItems();
	}
	for (size_t i = 0; i < members.size(); ++i) {
		strItem.Format(members[i].c_str(), i);
		if (list_members.GetItemCount() != members.size()) {
			list_members.InsertItem(i, strItem, i);
		}
	}

	// Set friends
	if (list_friends.GetItemCount() != friends.size()) {
		list_friends.DeleteAllItems();
	}
	for (size_t i = 0; i < friends.size(); ++i) {
		strItem.Format(friends[i].c_str(), i);
		if (list_friends.GetItemCount() != friends.size()) {
			list_friends.InsertItem(i, strItem, i);
		}
	}

	// Focus to group name
	groupname.SetFocus();
	groupname.SetSel(-1);
}


BEGIN_MESSAGE_MAP(CreateGroupDlg, CDialog)
	ON_WM_PAINT()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FRIENDS, &CreateGroupDlg::OnNMDblclkListFriends)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MEMBERS, &CreateGroupDlg::OnNMDblclkListMembers)
	ON_EN_UPDATE(IDC_GROUP_NAME, &CreateGroupDlg::OnEnUpdateGroupName)
	ON_BN_CLICKED(IDC_BTN_CREATE_GROUP, &CreateGroupDlg::OnBnClickedBtnCreateGroup)
END_MESSAGE_MAP()


BOOL CreateGroupDlg::OnInitDialog()
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

void CreateGroupDlg::OnPaint()
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

// CreateGroupDlg message handlers


void CreateGroupDlg::OnNMDblclkListFriends(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nSelected = pNMItemActivate->iItem;
	CString strItem = list_friends.GetItemText(nSelected, 0);
	if (strItem.GetLength()) {
		members.emplace_back(strItem);
		friends.erase(friends.begin() + nSelected);
		SetSwitchBox();
	}
	*pResult = 0;
}


void CreateGroupDlg::OnNMDblclkListMembers(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nSelected = pNMItemActivate->iItem;
	CString strItem = list_members.GetItemText(nSelected, 0);
	if (strItem.GetLength() && strItem != account->GetUsername().c_str()) {
		friends.emplace_back(strItem);
		members.erase(members.begin() + nSelected);
		SetSwitchBox();
	}
	*pResult = 0;
}

bool isIn(std::wstring item, std::vector<Group> list) {
	for (auto x : list) {
		if (x.name == item) {
			return true;
		}
	}
	return false;
}

void CreateGroupDlg::OnEnUpdateGroupName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	CString tmp;
	GetDlgItemText(IDC_GROUP_NAME, tmp);
	std::wstring _groupname(tmp);
	if (_groupname.find_first_not_of(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_ ") != std::string::npos) {
		MessageBox(_T("Group name must contain a-z, A-Z, 0-9 and _ or space character only!"), _T("Alert"), MB_ICONERROR);
		tmp.Delete(tmp.GetLength() - 1, 1);
		groupname.SetWindowTextW(tmp);
		groupname.SetSel(-1);
	}
	// TODO:  Add your control notification handler code here
}


void CreateGroupDlg::OnBnClickedBtnCreateGroup()
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	CString _groupname;
	GetDlgItemText(IDC_GROUP_NAME, _groupname);
	std::wstring name(_groupname);

	if (isIn(name, groups)) {
		MessageBox(_T("Group name already exist!"), _T("Alert"), MB_ICONERROR);
	}
	else if (_groupname.GetLength() < 1) {
		MessageBox(_T("Group name must not be empty!"), _T("Alert"), MB_ICONERROR);
	}
	else if (members.size() < 2) {
		MessageBox(_T("Group must have at least two members, please add more by double click friend name on friends box!"), _T("Alert"), MB_ICONWARNING);
	}
	else {
		CString _mess;
		if (group.name.size()) {
			members.insert(members.begin(), name);
			accMa->UpdateGroup(*account, group.name, Group(members));
			_mess = _T("Group '") + _groupname + _T("' modify successfully!");
		}
		else {
			accMa->CreateGroup(*account, name, members);
			_mess = _T("Group '") + _groupname + _T("' create successfully!");
		}
		MessageBox(_mess, _T("Alert"), MB_ICONINFORMATION);
		CDialog::OnOK();
	}
	groupname.SetFocus();
	groupname.SetSel(-1);
}
