// messenger.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "messenger.h"
#include "afxdialogex.h"
#include "ChatUiDlg.hpp"
#include "EditProfileDlg.h"
#include "AddFriendDlg.h"
#include "Tools.hpp"
#include "AttachFile.h"
#include <gdiplusgraphics.h>
#include "ChooseFriend.h"
#include "ConfirmDlg.h"
#include "EmojiDlg.h"
#include "CreateGroupDlg.h"
#include "ChooseGroup.h"


// messenger dialog

IMPLEMENT_DYNAMIC(messenger, CDialog)

messenger::messenger(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_messenger, pParent)
{
	
}

messenger::messenger(ClientBackgroundService* mClientService) : CDialog(IDD_messenger, nullptr)
{
	this->mClientService = mClientService;
	this->mClientService->CreateWorkerThread();
	wstring username = L"a";
	accMa = new AccountManagement;
	account = accMa->GetAccount(username).Clone();
	friends = accMa->GetFriends(*account);
	groups = accMa->GetGroups(*account);
	if (friends.size()) {
		target = friends[0];
		targetIsGroup = false;
	}
	else if (groups.size()) {
		target = groups[0].name;
		targetIsGroup = true;
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

messenger::messenger(std::wstring username)
	: CDialog(IDD_messenger, nullptr)
{
	accMa = new AccountManagement;
	account = accMa->GetAccount(username).Clone();
	friends = accMa->GetFriends(*account);
	groups = accMa->GetGroups(*account);
	if (friends.size()) {
		target = friends[0];
		targetIsGroup = false;
	}
	else if (groups.size()) {
		target = groups[0].name;
		targetIsGroup = true;
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

messenger::~messenger()
{
	if (account) {
		delete account;
	}
	if (accMa) {
		delete accMa;
	}
}

void messenger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT_USERNAME, username);
	DDX_Control(pDX, IDC_LIST_FRIENDS, list_friends);
	DDX_Control(pDX, IDC_MESS_CONTENT, mess_content);
	DDX_Control(pDX, IDC_LIST_MESS, list_mess);
	DDX_Control(pDX, IDC_MESS_CONTENT, mess_content);
	DDX_Control(pDX, IDC_LIST_GROUPS, list_groups);

	SetUserIcon();
	SetSendBtnIcon();
	SetAddFriendIcon();
	ShowFriends();
	ShowGroups();
	list_mess.InsertColumn(0, _T(""), LVCFMT_LEFT, 870);
	ShowMessages();

	mess_content.GetWindowRect(&mess_rect);
	ScreenToClient(&mess_rect);
	list_mess.GetWindowRect(&list_mess_rect);
	ScreenToClient(&list_mess_rect);
}

HBRUSH messenger::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_MESS_CONTENT)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(214, 234, 248));
		//pDC->SetBkMode(TRANSPARENT);
		hbr = m_brush;
	}
	return hbr;
}

BEGIN_MESSAGE_MAP(messenger, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, &messenger::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_SEND, &messenger::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_ADD_FRIEND, &messenger::OnBnClickedBtnAddFriend)
	ON_BN_CLICKED(IDC_ICON, &messenger::OnBnClickedIcon)
	ON_BN_CLICKED(IDC_BTN_SEND_FILE, &messenger::OnBnClickedBtnSendFile)
	ON_EN_CHANGE(IDC_MESS_CONTENT, &messenger::OnEnChangeMessContent)
	ON_EN_UPDATE(IDC_MESS_CONTENT, &messenger::OnEnUpdateMessContent)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FRIENDS, &messenger::OnNMRClickListFriends)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FRIENDS, &messenger::OnNMDblclkListFriends)
	ON_BN_CLICKED(IDC_BTN_SEND_ICON, &messenger::OnBnClickedBtnSendIcon)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GROUPS, &messenger::OnNMDblclkListGroups)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_GROUPS, &messenger::OnNMRClickListGroups)
	ON_BN_CLICKED(IDC_BTN_NOTIFICATION, &messenger::OnBnClickedBtnNotification)
	ON_BN_CLICKED(IDC_BTN_ADD_GROUP, &messenger::OnBnClickedBtnAddGroup)
END_MESSAGE_MAP()


BOOL messenger::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	this->mClientService->AddHwnd(this->GetSafeHwnd());

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void messenger::OnPaint()
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
// messenger message handlers



void messenger::ShowFriends()
{
	static CImageList imgList;
	CString strItem = _T("");
	imgList.DeleteImageList();
	imgList.Create(32, 32, ILC_COLOR32, 0, 0);
	if (list_friends.GetItemCount() != friends.size()) {
		list_friends.DeleteAllItems();
	}
	for (int i = 0; i < friends.size(); ++i) {
		if (!targetIsGroup && (friends[i] == target || target.length() == 0)) {
			imgList.Add(AfxGetApp()->LoadIconW(IDI_USER_CHATTING));
		}
		else {
			imgList.Add(AfxGetApp()->LoadIconW(IDI_USER_ONL));
		}
		strItem.Format(friends[i].c_str(), i);
		if (list_friends.GetItemCount() != friends.size()) {
			list_friends.InsertItem(i, strItem, i);
		}
	}
	list_friends.SetImageList(&imgList, LVSIL_NORMAL);
}

void messenger::ShowGroups()
{
	static CImageList imgList3;
	CString strItem = _T("");
	imgList3.DeleteImageList();
	imgList3.Create(34, 34, ILC_COLOR32, 0, 0);
	list_groups.DeleteAllItems();
	for (int i = 0; i < groups.size(); ++i) {
		if (targetIsGroup && (groups[i].name == target || target.length() == 0)) {
			imgList3.Add(AfxGetApp()->LoadIconW(IDI_GROUP_CHATTING));
		}
		else {
			imgList3.Add(AfxGetApp()->LoadIconW(IDI_GROUP));
		}
		strItem.Format(groups[i].name.c_str(), i);
		list_groups.InsertItem(i, strItem, i);
	}
	list_groups.SetImageList(&imgList3, LVSIL_NORMAL);
}

void messenger::ShowGroupsClick()
{
	static CImageList imgList3;
	CString strItem = _T("");
	imgList3.DeleteImageList();
	imgList3.Create(34, 34, ILC_COLOR32, 0, 0);
	if (list_groups.GetItemCount() != groups.size()) {
		list_groups.DeleteAllItems();
	}
	for (int i = 0; i < groups.size(); ++i) {
		if (targetIsGroup && (groups[i].name == target || target.length() == 0)) {
			imgList3.Add(AfxGetApp()->LoadIconW(IDI_GROUP_CHATTING));
		}
		else {
			imgList3.Add(AfxGetApp()->LoadIconW(IDI_GROUP));
		}
		strItem.Format(groups[i].name.c_str(), i);
		if (list_groups.GetItemCount() != groups.size()) {
			list_groups.InsertItem(i, strItem, i);
		}
	}
	list_groups.SetImageList(&imgList3, LVSIL_NORMAL);
}

void messenger::ShowMessages()
{
	SetChatBoxTitle();
	list_mess.DeleteAllItems();
	count = 0;
	std::wstring fileName = target;
	if (fileName != L"")
	{
		std::wstring path;
		if (targetIsGroup) {
			path = L"./data/messages/" + account->GetUsername() + L"/groups/" + fileName + L".dat";
		}
		else {
			path = L"./data/messages/" + account->GetUsername() + L"/friends/" + fileName + L".dat";
		}
		std::vector < CString > messages = Tools().ReadFile(path);
		for (auto x : messages) {
			list_mess.InsertItem(count++, x);
		}
		RECT r;
		list_mess.GetItemRect(0, &r, LVIR_BOUNDS);
		list_mess.Scroll(CSize(0, (r.bottom - r.top) * list_mess.GetItemCount()));
	}
}

std::vector<CString> messenger::GetMessagesFromListMess()
{
	std::vector < CString > messages;
	CString itemText;
	for (int i = 0; i < list_mess.GetItemCount(); i++)
	{
		itemText = list_mess.GetItemText(i, 0);
		DWORD_PTR p = list_mess.GetItemData(i);
		messages.push_back(itemText);
	}
	return messages;
}
void messenger::SetChatBoxTitle()
{
	CString friendName = target.c_str();
	CString title;
	if (friendName.GetLength() > 0) {
		title = _T(" Chatting with: ") + friendName + _T(" ");
	}
	else {
		title = _T(" Add friend or Create / Join group to start chat ");
	}
	CClientDC dc(&list_mess);
	CFont* pOldFont = dc.SelectObject(list_mess.GetFont());
	CSize size = dc.GetTextExtent(title);
	int buf = (800 - size.cx) / dc.GetTextExtent(_T("🗲")).cx;
	for (int i = 0; i < buf / 2; ++i) {
		title = _T("🗲") + title + _T("🗲");
	}
	title = _T("        ") + title;

	LVCOLUMN lvCol;
	::ZeroMemory((void*)&lvCol, sizeof(LVCOLUMN));
	lvCol.mask = LVCF_TEXT;
	list_mess.GetColumn(0, &lvCol);
	lvCol.pszText = (LPWSTR)(LPCWSTR)(title);
	list_mess.SetColumn(0, &lvCol);
}
void messenger::SaveMessages()
{
	std::wstring fileName = target;
	if (fileName != L"")
	{
		std::wstring path;
		if (targetIsGroup) {
			path = L"./data/messages/" + account->GetUsername() + L"/groups/" + fileName + L".dat";
		}
		else {
			path = L"./data/messages/" + account->GetUsername() + L"/friends/" + fileName + L".dat";
		}
		std::vector < CString > messages = GetMessagesFromListMess();
		Tools().WriteToFile(path, messages);
	}
}

void messenger::SetBtnIcon(int btnId, int iconId, int size) {
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

void messenger::SetUserIcon()
{
	SetBtnIcon(IDC_ICON, IDI_AVATAR, 32);
	SetBtnIcon(IDC_BTN_NOTIFICATION, IDI_NOTIFICATION, 32);

	CFont font;
	font.CreateFont(
		20,                        // nHeight
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

	username.SetWindowTextW(account->GetUsername().c_str());
	username.SetFont(&font);
}

void messenger::SetSendBtnIcon()
{
	SetBtnIcon(IDC_BTN_SEND, IDI_LOVE, 22);
	SetBtnIcon(IDC_BTN_SEND_ICON, IDI_SEND_EMOJI, 22);
	SetBtnIcon(IDC_BTN_SEND_FILE, IDI_SEND_FILE, 22);
}

void messenger::SetAddFriendIcon()
{
	SetBtnIcon(IDC_BTN_ADD_FRIEND, IDI_ADD_FRIEND, 32);
	SetBtnIcon(IDC_BTN_ADD_GROUP, IDI_ADD_GROUP, 36);
}

void messenger::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	SaveMessages();
	CDialog::OnCancel();
	ChatUiDlg chatDlg(account->GetUsername().c_str());
	chatDlg.DoModal();
}


void messenger::OnBnClickedBtnSend()
{
	// TODO: Add your control notification handler code here
	CString content;
	GetDlgItemText(IDC_MESS_CONTENT, content);
	if (content.GetLength() > 0) {
		wchar_t space(L' ');
		std::wstring mess(content);
		int line_size = 60;
		std::vector<std::wstring> contents;
		int p1, buf;
		std::wstring t1 = mess, t2;
		do {
			t2 = t1;
			p1 = t2.find_first_of(L"\r\n");
			if (p1 == std::string::npos) {
				p1 = t2.find_first_of(L"\n");
				if (p1 == std::string::npos) {
					p1 = t2.find_first_of(L"\r");
				}
				buf = 1;
			}
			else {
				buf = 2;
			}
			if (p1 == std::string::npos) {
				contents.push_back(t2);
				break;
			}
			else {
				t1 = t2.substr(p1 + buf, t2.size() - p1 - buf);
				contents.push_back(t2.substr(0, p1));
			}
		} while (true);
		std::wstring tmp;
		for (int j = 0; j < contents.size(); ++j) {
			for (int i = 0, end = line_size + i; i < contents[j].size();) {
				if (end > contents[j].size()) {
					std::wstring mess = contents[j].substr(i, contents[j].size() - end);
					mess = (j == 0 && i == 0 ? _T("Me: ") : _T("       ")) + std::wstring(CString(mess.c_str()).Trim());
					list_mess.InsertItem(count++, mess.c_str());
					break;
				}
				tmp = contents[j].substr(i, line_size);
				int end_s = line_size;
				if (contents[j][end] != space && contents[j][end - 1] != space) {
					end_s = tmp.find_last_of(space);
					if (end_s == -1) {
						end_s = line_size;
					}
				}
				std::wstring mess = tmp.substr(0, end_s);
				mess = (j == 0 && i == 0 ? _T("Me: ") : _T("       ")) + std::wstring(CString(mess.c_str()).Trim());
				list_mess.InsertItem(count++, mess.c_str());
				i += end_s;
				end = line_size + i;
			}
		}
	}
	else {
		list_mess.InsertItem(count++, _T("Me: 🖤"));
	}
	mess_content.SetWindowTextW(L"");
	OnEnUpdateMessContent();
	OnEnChangeMessContent();
	mess_content.SetFocus();
}


void messenger::OnBnClickedBtnAddFriend()
{
	// TODO: Add your control notification handler code here
	AddFriendDlg addFriend(accMa, account);
	INT_PTR nRet = -1;
	nRet = addFriend.DoModal();

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
		friends = accMa->GetFriends(*account);
		StartChat(friends[friends.size() - 1], false);
		break;
	case IDCANCEL:
		// Do something
		break;
	default:
		// Do something
		break;
	};
	mess_content.SetFocus();
	mess_content.SetSel(-1);
}

void messenger::OnBnClickedIcon()
{
	// TODO: Add your control notification handler code here
	EditProfileDlg edit(accMa, account);

	INT_PTR nRet = -1;
	nRet = edit.DoModal();

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
		OnBnClickedCancel();
		break;
	case IDCANCEL:
		// Do something
		break;
	default:
		// Do something
		break;
	};
	mess_content.SetFocus();
	mess_content.SetSel(-1);
}


void messenger::OnBnClickedBtnSendFile()
{
	// TODO: Add your control notification handler code here
	AttachFile attachFile(true);
	attachFile.DoModal();
	mess_content.SetFocus();
	mess_content.SetSel(-1);
}


void messenger::OnEnChangeMessContent()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CRect listRect(mess_rect);
	CString mess;
	GetDlgItemText(IDC_MESS_CONTENT, mess);

	CClientDC dc(&mess_content);
	CFont* pOldFont = dc.SelectObject(mess_content.GetFont());
	CSize size = dc.GetTextExtent(mess);

	int line = int(size.cx / 796);
	line = line > 8 ? 8 : line;
	listRect.top = listRect.top - 22 * line;
	mess_content.MoveWindow(&listRect);


	CRect listRect2(list_mess_rect);
	listRect2.bottom = listRect2.bottom - 22 * line;
	list_mess.MoveWindow(&listRect2);

	RECT r;
	list_mess.GetItemRect(0, &r, LVIR_BOUNDS);
	list_mess.Scroll(CSize(0, (r.bottom - r.top) * list_mess.GetItemCount()));

}


void messenger::OnEnUpdateMessContent()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	CString mess;
	GetDlgItemText(IDC_MESS_CONTENT, mess);
	if (mess.GetLength() != 0) {
		SetBtnIcon(IDC_BTN_SEND, IDI_SEND, 22);
	}
	else {
		SetBtnIcon(IDC_BTN_SEND, IDI_LOVE, 22);
	}
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_SEND);
	pBtn->SetFocus();
	mess_content.SetFocus();
}

std::vector<CString> messenger::GetSelectedItemText(CListCtrl* plctrl)
{
	POSITION pos = plctrl->GetFirstSelectedItemPosition();
	int selected = -1;
	std::vector<CString> res;
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = plctrl->GetNextSelectedItem(pos);
			// selected = nItem + 1;
			res.push_back(plctrl->GetItemText(nItem, 0));
		}
	}
	return res;
}

void messenger::StartChat(std::wstring item, bool isGroup) {
	SaveMessages();
	target = item;
	targetIsGroup = isGroup;
	ShowGroupsClick();
	ShowFriends();
	ShowMessages();
	mess_content.SetFocus();
	mess_content.SetSel(-1);
}

void messenger::OnNMRClickListFriends(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	std::vector<CString> selected = GetSelectedItemText(&list_friends);
	std::vector<std::wstring> friends_name;
	bool includeTarget = false;
	for (auto x : selected) {
		if (x.GetLength()) {
			std::wstring tmp(x);
			if (tmp == target) {
				includeTarget = true;
			}
			friends_name.push_back(tmp);
		}

	}
	ChooseFriend chooseFriend;
	std::wstring mess;

LOOP:
	mess.clear();
	for (auto x : friends_name) {
		mess += L"\"" + x + L"\"" + L", ";
	}
	mess.erase(mess.end() - 2, mess.end());

	ConfirmDlg confirmDlg;
	INT_PTR nRet1, nRet2;
	nRet1 = -1;
	nRet2 = -1;
	nRet1 = chooseFriend.DoModal();
	// Handle the return value from DoModal
	switch (nRet1)
	{
	case -1:
		AfxMessageBox(_T("Dialog box could not be created!"));
		break;
	case ID_BTN_START_CHAT:
		if (selected.size() == 1) {
			StartChat(friends_name[0]);
		}
		else {
			CreateGroupDlg createGroupDlg(accMa, account, friends_name);
			INT_PTR nRet = -1;
			nRet = createGroupDlg.DoModal();

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
				groups = accMa->GetGroups(*account);
				StartChat(groups[groups.size() - 1].name, true);
				break;
			case IDCANCEL:
				// Do something
				break;
			default:
				// Do something
				break;
			};

			mess_content.SetFocus();
			mess_content.SetSel(-1);
		}
		break;
	case ID_BTN_UNFRIEND:
		mess = L"After this action, (" + mess + L") will not be your friends anymore!\r\n\r\nAre you sure?";
		confirmDlg.SetMess(mess);
		nRet2 = confirmDlg.DoModal();

		// Handle the return value from DoModal
		switch (nRet2)
		{
		case -1:
			AfxMessageBox(_T("Dialog box could not be created!"));
			break;
		case IDABORT:
			// Do something
			break;
		case IDOK:
			accMa->RemoveFriends(*account, friends_name);
			friends = accMa->GetFriends(*account);
			if (includeTarget) {
				if (friends.size() > 0) {
					target = friends[0];
				}
				else {
					target = L"";
				}
				ShowMessages();
			}
			ShowFriends();
			break;
		case IDCANCEL:
			// Do something
			goto LOOP;
			break;
		default:
			// Do something
			break;
		}
		break;
	default:
		// Do something
		break;
	};
	mess_content.SetFocus();
	mess_content.SetSel(-1);
	*pResult = 0;
}


void messenger::OnNMDblclkListFriends(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nSelected = pNMItemActivate->iItem;
	CString strItem = list_friends.GetItemText(nSelected, 0);
	if (strItem.GetLength()) {
		StartChat(std::wstring(strItem), false);
	}
	*pResult = 0;
}


void messenger::OnBnClickedBtnSendIcon()
{
	// TODO: Add your control notification handler code here
	CString name;
	username.GetWindowTextW(name);
	EmojiDlg emojiDlg(&mess_content, std::wstring(name));
	emojiDlg.DoModal();
	mess_content.SetFocus();
	mess_content.SetSel(-1);
	OnEnUpdateMessContent();
}


void messenger::OnNMDblclkListGroups(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nSelected = pNMItemActivate->iItem;
	CString strItem = list_groups.GetItemText(nSelected, 0);
	if (strItem.GetLength()) {
		StartChat(std::wstring(strItem), true);
	}
	*pResult = 0;
}


void messenger::OnNMRClickListGroups(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	std::vector<CString> selected = GetSelectedItemText(&list_groups);
	std::vector<std::wstring> groups_name;
	bool includeTarget = false;
	for (auto x : selected) {
		if (x.GetLength()) {
			std::wstring tmp(x);
			if (tmp == target) {
				includeTarget = true;
			}
			groups_name.push_back(tmp);
		}
	}

	ChooseGroup chooseGroup(accMa, account, groups_name);

	//ChooseFriend chooseFriend;
LOOP2:
	INT_PTR nRet = -1;
	nRet = chooseGroup.DoModal();
	// Handle the return value from DoModal
	switch (nRet)
	{
	case -1:
		AfxMessageBox(_T("Dialog box could not be created!"));
		break;
	case ID_BTN_OUT_GROUP:
		groups = accMa->GetGroups(*account);
		ShowGroups();
		break;
	case ID_BTN_START_CHAT_GROUP:
		StartChat(groups_name[0], true);
		break;
	case ID_BTN_MODIFY_GROUP:
		groups = accMa->GetGroups(*account);
		ShowGroups();
	default:
		// Do something
		break;
	};

	mess_content.SetFocus();
	mess_content.SetSel(-1);
	*pResult = 0;
}

void messenger::OnBnClickedBtnNotification()
{
	Tools().PlayGotMessSound();
	// TODO: Add your control notification handler code here
	if (notification) {
		SetBtnIcon(IDC_BTN_NOTIFICATION, IDI_UNNOTIFICATION, 32);
	}
	else {
		SetBtnIcon(IDC_BTN_NOTIFICATION, IDI_NOTIFICATION, 32);
	}
	notification = !notification;
	mess_content.SetFocus();
	mess_content.SetSel(-1);
}


void messenger::OnBnClickedBtnAddGroup()
{
	// TODO: Add your control notification handler code here
	CreateGroupDlg createGroupDlg(accMa, account);

	INT_PTR nRet = -1;
	nRet = createGroupDlg.DoModal();

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
		groups = accMa->GetGroups(*account);
		StartChat(groups[groups.size() - 1].name, true);
		break;
	case IDCANCEL:
		// Do something
		break;
	default:
		// Do something
		break;
	};

	mess_content.SetFocus();
	mess_content.SetSel(-1);
}
