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
	//groups = accMa->GetGroups(*account);
	friends = accMa->GetFriends(*account);
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

CreateGroupDlg::CreateGroupDlg(AccountManagement* accMa, Account* account, std::wstring group_name)
	: CDialog(IDD_CreateGroupDlg, nullptr)
	, accMa(accMa)
	, account(account)
{
	friends = accMa->GetFriends(*account);
	//groups = accMa->GetGroups(*account);
	for (size_t i = 0; i < groups.size(); ++i) {
		if (groups[i] == group_name) {
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

CreateGroupDlg::CreateGroupDlg(std::wstring username, std::vector<std::wstring> members, std::vector<std::wstring> friends, std::vector<std::wstring> groups)
	: CDialog(IDD_CreateGroupDlg, nullptr)
	, username(username)
	, members(members)
	, friends(friends)
	, groups(groups)
{
	this->members.insert(this->members.begin(), username);
	for (size_t i = 0; i < this->friends.size(); ++i) {
		for (size_t j = 0; j < this->members.size(); ++j) {
			if (this->friends[i] == this->members[j]) {
				this->friends.erase(this->friends.begin() + i);
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

void CreateGroupDlg::SetBtnIcon(int btnId, int iconId, int size) {
	CButton* pBtn = (CButton*)GetDlgItem(btnId);
	pBtn->ModifyStyle(0, BS_ICON);
	HICON hIcn = (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(iconId),
		IMAGE_ICON,
		size, size, // use actual size
		LR_DEFAULTCOLOR
	);

	pBtn->SetIcon(hIcn);
}

void CreateGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MEMBERS, list_members);
	DDX_Control(pDX, IDC_LIST_FRIENDS, list_friends);
	DDX_Control(pDX, IDC_GROUP_NAME, groupname);

	list_friends.InsertColumn(0, _T(""), LVCFMT_LEFT, 200);
	list_members.InsertColumn(0, _T(""), LVCFMT_LEFT, 200);

	// Set btn icon

	SetBtnIcon(IDC_BTN_SWAP, IDI_SWAP, 16);
	SetBtnIcon(IDC_BTN_LEFT, IDI_LEFT, 16);
	SetBtnIcon(IDC_BTN_RIGHT, IDI_RIGHT, 16);

	if (group.name.size()) {
		groupname.SetWindowTextW(group.name.c_str());
		groupname.SetSel(-1);
		GetDlgItem(IDC_BTN_CREATE_GROUP)->SetWindowTextW(TEXT("Modify"));
	}
	SetSwitchBox();
}

void CreateGroupDlg::SetSwitchBox()
{
	// Set members
	CString strItem = _T("");
	list_members.DeleteAllItems();
	for (size_t i = 0; i < members.size(); ++i) {
		strItem.Format(members[i].c_str(), i);
		list_members.InsertItem(i, strItem, i);
	}

	// Set friends
	list_friends.DeleteAllItems();
	for (size_t i = 0; i < friends.size(); ++i) {
		strItem.Format(friends[i].c_str(), i);
		list_friends.InsertItem(i, strItem, i);
	}

	// Focus to group name
	groupname.SetFocus();
	groupname.SetSel(-1);
}


BEGIN_MESSAGE_MAP(CreateGroupDlg, CDialog)
	ON_WM_PAINT()
	//	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FRIENDS, &CreateGroupDlg::OnNMDblclkListFriends)
	//	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MEMBERS, &CreateGroupDlg::OnNMDblclkListMembers)
	ON_EN_UPDATE(IDC_GROUP_NAME, &CreateGroupDlg::OnEnUpdateGroupName)
	ON_BN_CLICKED(IDC_BTN_CREATE_GROUP, &CreateGroupDlg::OnBnClickedBtnCreateGroup)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_FRIENDS, &CreateGroupDlg::OnNMSetfocusListFriends)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MEMBERS, &CreateGroupDlg::OnNMClickListMembers)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FRIENDS, &CreateGroupDlg::OnNMClickListFriends)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_MEMBERS, &CreateGroupDlg::OnNMSetfocusListMembers)
	ON_BN_CLICKED(IDC_BTN_SWAP, &CreateGroupDlg::OnBnClickedBtnSwap)
	ON_BN_CLICKED(IDC_BTN_LEFT, &CreateGroupDlg::OnBnClickedBtnLeft)
	ON_BN_CLICKED(IDC_BTN_RIGHT, &CreateGroupDlg::OnBnClickedBtnRight)
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


//void CreateGroupDlg::OnNMDblclkListFriends(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: Add your control notification handler code here
//
//	*pResult = 0;
//}


//void CreateGroupDlg::OnNMDblclkListMembers(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: Add your control notification handler code here
//
//	*pResult = 0;
//}

bool isIn(std::wstring item, std::vector<std::wstring> list) {
	for (auto x : list) {
		if (x == item) {
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
		MessageBox(_T("Group conversation name already exist, you're already in this group!"), _T("Alert"), MB_ICONERROR);
	}
	else if (_groupname.GetLength() < 1) {
		MessageBox(_T("Group name must not be empty!"), _T("Alert"), MB_ICONERROR);
	}
	else if (members.size() < 2) {
		MessageBox(_T("Group must have at least two members, please add more by click friend name on Friends box!"), _T("Alert"), MB_ICONWARNING);
	}
	else {
		CString _mess;
		if (group.name.size()) {
			members.insert(members.begin(), name);
				_mess = _T("Group '") + _groupname + _T("' modify successfully!");
				MessageBox(_mess, _T("Alert"), MB_ICONINFORMATION);
				this->_groupname = _groupname;
				OnOK();
		}
		else {
				_mess = _T("Group '") + _groupname + _T("' create successfully!");
				MessageBox(_mess, _T("Alert"), MB_ICONINFORMATION);
				this->_groupname = _groupname;
				OnOK();
		}
	}
	groupname.SetFocus();
	groupname.SetSel(-1);
}


void CreateGroupDlg::OnNMSetfocusListFriends(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	groupname.SetFocus();
	groupname.SetSel(-1);
	*pResult = 0;
}


void CreateGroupDlg::OnNMClickListMembers(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nSelected = pNMItemActivate->iItem;
	CString strItem = list_members.GetItemText(nSelected, 0);
	if (strItem.GetLength() && strItem != username.c_str()) {
		friends.emplace_back(strItem);
		members.erase(members.begin() + nSelected);
		SetSwitchBox();
	}
	*pResult = 0;
}


void CreateGroupDlg::OnNMClickListFriends(NMHDR* pNMHDR, LRESULT* pResult)
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


void CreateGroupDlg::OnNMSetfocusListMembers(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	groupname.SetFocus();
	groupname.SetSel(-1);
	*pResult = 0;
}



void CreateGroupDlg::OnBnClickedBtnSwap()
{
	// TODO: Add your control notification handler code here
	std::vector<std::wstring> temp = friends;
	temp.insert(temp.begin(), members[0]);
	members.erase(members.begin());
	friends = members;
	members = temp;
	SetSwitchBox();
}


void CreateGroupDlg::OnBnClickedBtnLeft()
{
	// TODO: Add your control notification handler code here
	for (auto x : friends) {
		members.push_back(x);
	}
	friends.clear();
	SetSwitchBox();
}


void CreateGroupDlg::OnBnClickedBtnRight()
{
	// TODO: Add your control notification handler code here
	std::wstring username = members[0];
	for (size_t i = 1; i < members.size(); ++i) {
		friends.push_back(members[i]);
	}
	members.clear();
	members.push_back(username);
	SetSwitchBox();
}
