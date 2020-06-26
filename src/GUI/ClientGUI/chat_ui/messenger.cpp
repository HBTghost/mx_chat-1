// messenger.cpp : implementation file
//

#include "pch.h"
#include<algorithm> // for copy() and assign() 
#include<iterator> // for back_inserter 

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

const CString ATTACH_FILE_FLAG = _T("\r\r\r");
const CString ATTACH_FILE_ICON = _T("📌 ");

// messenger dialog

IMPLEMENT_DYNAMIC(messenger, CDialog)

messenger::messenger(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_messenger, pParent)
{

}

messenger::messenger(ClientBackgroundService* mClientService, CWnd* parent) : CDialog(IDD_messenger, nullptr)
{
	this->mClientService = mClientService;
	this->mClientService->CreateWorkerThread();
	//wstring username = L"a";
	//accMa = new AccountManagement;
	//account = accMa->GetAccount(username).Clone();
	friends = StringHelper::VectorStringToWideString(this->mClientService->gClientObj._list_online);
	this->username = StringHelper::utf8_decode(mClientService->username).c_str();
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

messenger::messenger(std::wstring username)
	: CDialog(IDD_messenger, nullptr)
{
	//accMa = new AccountManagement;
	//account = accMa->GetAccount(username).Clone();
	//friends = accMa->GetFriends(*account);
	//groups = accMa->GetGroups(*account);
	if (friends.size()) {
		target = friends[0];
		targetIsGroup = false;
	}
	else if (groups.size()) {
		//target = groups[0].name;
		targetIsGroup = true;
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

messenger::~messenger()
{
	//if (account) {
	//	delete account;
	//}
	//if (accMa) {
	//	delete accMa;
	//}
}

void messenger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FRIENDS, list_friends);
	DDX_Control(pDX, IDC_MESS_CONTENT, mess_content);
	DDX_Control(pDX, IDC_LIST_MESS, list_mess);
	DDX_Control(pDX, IDC_MESS_CONTENT, mess_content);
	DDX_Control(pDX, IDC_LIST_GROUPS, list_groups);
	DDX_Control(pDX, IDC_LIST_MEMBER_CHAT, m_messMember);
	DDX_Control(pDX, IDC_LIST_FILE_TRANSFER, m_ListFile);

	SetMessColor();
	SetUserIcon();
	SetSendBtnIcon();
	SetAddFriendIcon();

	ShowFriends();
	ShowGroups();
	list_mess.InsertColumn(0, _T(""), LVCFMT_LEFT, 870);


	mess_content.GetWindowRect(&mess_rect);
	ScreenToClient(&mess_rect);
	list_mess.GetWindowRect(&list_mess_rect);
	ScreenToClient(&list_mess_rect);
	DDX_Control(pDX, IDC_LIST_LOGS, list_logs);
}

HBRUSH messenger::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_LIST_MEMBER_CHAT || pWnd->GetDlgCtrlID() == IDC_LIST_FILE_TRANSFER)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		switch (colorTheme)
		{
		case BRICK:
			pDC->SetBkColor(RGB(253, 237, 236));
			break;
		case PURPLE:
			pDC->SetBkColor(RGB(244, 236, 247));
			break;
		case BLUE:
			pDC->SetBkColor(RGB(235, 245, 251));
			break;
		case GREEN:
			pDC->SetBkColor(RGB(232, 246, 243));
			break;
		case SGREEN:
			pDC->SetBkColor(RGB(234, 250, 241));
			break;
		case YELLOW:
			pDC->SetBkColor(RGB(254, 249, 231));
			break;
		case ORANGE:
			pDC->SetBkColor(RGB(251, 238, 230));
			break;
		case GRAY:
			pDC->SetBkColor(RGB(242, 244, 244));
			break;
		case WHITE:
			pDC->SetBkColor(RGB(255, 255, 255));
			break;
		default:
			break;
		}
		hbr = m_brush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MESS_CONTENT)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		switch (colorTheme)
		{
		case BRICK:
			pDC->SetBkColor(RGB(245, 183, 177));
			break;
		case PURPLE:
			pDC->SetBkColor(RGB(210, 180, 222));
			break;
		case BLUE:
			pDC->SetBkColor(RGB(174, 214, 241));
			break;
		case GREEN:
			pDC->SetBkColor(RGB(162, 217, 206));
			break;
		case SGREEN:
			pDC->SetBkColor(RGB(171, 235, 198));
			break;
		case YELLOW:
			pDC->SetBkColor(RGB(249, 231, 159));
			break;
		case ORANGE:
			pDC->SetBkColor(RGB(237, 187, 153));
			break;
		case GRAY:
			pDC->SetBkColor(RGB(204, 209, 209));
			break;
		case WHITE:
			pDC->SetBkColor(RGB(255, 255, 255));
			break;
		default:
			break;
		}
		hbr = m_brush;
	}
	return hbr;
}

void messenger::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	bool draw = true;
	COLORREF color;
	if (nIDCtl == IDC_LIST_MEMBER_CHAT) {
		color = RGB(175, 122, 197);
	}
	else if (nIDCtl == IDC_LIST_FILE_TRANSFER) {
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

		dc.SetBkColor(RGB(255, 0, 255));
		dc.SetTextColor(RGB(0, 0, 0));
		dc.SetBkMode(TRANSPARENT);

		TCHAR buffer[MAX_PATH];
		ZeroMemory(buffer, MAX_PATH);
		::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);
		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		dc.Detach();
	}

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BEGIN_MESSAGE_MAP(messenger, CDialog)
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDCANCEL, &messenger::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_SEND, &messenger::OnBnClickedBtnSend)
//	ON_BN_CLICKED(IDC_BTN_ADD_FRIEND, &messenger::OnBnClickedBtnAddFriend)
	ON_BN_CLICKED(IDC_ICON, &messenger::OnBnClickedIcon)
	ON_BN_CLICKED(IDC_BTN_SEND_FILE, &messenger::OnBnClickedBtnSendFile)
	ON_EN_CHANGE(IDC_MESS_CONTENT, &messenger::OnEnChangeMessContent)
	ON_EN_UPDATE(IDC_MESS_CONTENT, &messenger::OnEnUpdateMessContent)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FRIENDS, &messenger::OnRightClickListFriends)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FRIENDS, &messenger::OnDoubleClickListFriends)
	ON_BN_CLICKED(IDC_BTN_SEND_ICON, &messenger::OnBnClickedBtnSendIcon)
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GROUPS, &messenger::OnDoubleClickListGroups)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_GROUPS, &messenger::OnRightClickListGroups)
	ON_BN_CLICKED(IDC_BTN_NOTIFICATION, &messenger::OnBnClickedBtnNotification)
	ON_BN_CLICKED(IDC_BTN_ADD_GROUP, &messenger::OnBnClickedBtnAddGroup)
	ON_MESSAGE(IDC_FORM_CHAT_MSG_HANDLER, &messenger::OnFormMsgHandler)
	ON_LBN_DBLCLK(IDC_LIST_FILE_TRANSFER, &messenger::OnLbnDblclkListFileTransfer)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MESS, &messenger::OnNMClickListMess)
	ON_BN_CLICKED(IDC_BTN_LINK, &messenger::OnBnClickedBtnLink)
	ON_BN_CLICKED(IDC_BTN_CAMERA, &messenger::OnBnClickedBtnCamera)
	ON_BN_CLICKED(IDC_BTN_IMAGE, &messenger::OnBnClickedBtnImage)
	ON_BN_CLICKED(IDC_BTN_VIDEO, &messenger::OnBnClickedBtnVideo)
	ON_BN_CLICKED(IDC_BTN_SOUND, &messenger::OnBnClickedBtnSound)
	ON_BN_CLICKED(IDC_BTN_DOC, &messenger::OnBnClickedBtnDoc)
	ON_NOTIFY(NM_CLICK, IDC_LIST_GROUPS, &messenger::OnNMClickListGroups)
//	ON_NOTIFY(NM_CLICK, IDC_LIST_FRIENDS, &messenger::OnNMClickListFriends)
ON_BN_CLICKED(IDC_BTN_ADD_FRIEND, &messenger::OnBnClickedBtnAddFriend)
END_MESSAGE_MAP()


BOOL messenger::OnInitDialog()
{

//	HICON hIconSm = (HICON)LoadImage(NULL, "default.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	CDialog::OnInitDialog();
	// btnid iconid , size
	//	SetBtnIcon(IDC_ICON, IDI_AVATAR, 32);


	SetIcon(m_hIcon, TRUE);			// Set big icon
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog


	HICON hIcon = (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_AVATAR),
		IMAGE_ICON,
		32, 32, // use actual size
		LR_DEFAULTCOLOR
	);
	sendWndIconToTaskbar(this->GetSafeHwnd(), m_hIcon );

	SetIcon(m_hIcon, FALSE);		// Set small icon

	this->mClientService->AddHwnd(this->GetSafeHwnd());
	SetWindowText(_T("Messenger: ") + username);


	
	mClientService->GetListOnline();


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

BOOL messenger::sendWndIconToTaskbar(HWND hWnd, HICON hIcon)
{
	BOOL ret = TRUE;
	ASSERT(hWnd);
	if (!::IsWindow(hWnd))
		return FALSE;
	CWnd* pWnd;
	pWnd = pWnd->FromHandle(hWnd);
	ASSERT(pWnd);
	if (!pWnd)
		return FALSE;
	if (pWnd->GetParent())
	{
		if (::SetWindowLong(hWnd, GWL_HWNDPARENT, NULL) == 0)
			return FALSE;
	}


	if (!(pWnd->ModifyStyle(NULL, WS_OVERLAPPEDWINDOW)))
		ret = FALSE;
	pWnd->SetIcon(hIcon, TRUE);
	pWnd->SetIcon(hIcon, FALSE);

	return ret;
}

LRESULT messenger::OnFormMsgHandler(WPARAM wParam, LPARAM lParam)
{
	UINT command_msg = (UINT)wParam;
	SDataPackage* model;
	ClientConversation* tempConv;
	vector<wstring> new_friends, val_add, val_remove;
	switch (command_msg)
	{
	case IDC_FORM_CHAT_MSG_HANDLER_LIST_ONLINE:
	{
		//MessageBox(_T("Form list online chat update "), _T("Alert"), MB_ICONINFORMATION);
		new_friends = StringHelper::VectorStringToWideString(this->mClientService->gClientObj._list_online);
		Tools().Fillter(friends, new_friends, &val_add, &val_remove);
		for (auto x : val_add) {
			list_logs.AddString((x + L" online 😃").c_str());
		}
		for (auto x : val_remove) {
			list_logs.AddString((x + L" offline 😭").c_str());
		}
		int nCount = 0;
		nCount = list_logs.GetCount(); // Declare m_LisBox as a control in ClassView member variables.
		list_logs.SetCurSel(nCount - 1); // List box is zero based.

		list_logs.SetTopIndex(nCount - 1); // moves the view.
		friends = new_friends;
		ShowFriends();
		break;
	}
	case IDC_FORM_CHAT_MSG_HANDLER_HASH_CONVERSATION: {
		if (lParam) {
			model = (SDataPackage*)lParam;
			string  hash_id = model->_data_items[0];
			if (model->_data_items[1] == "PRIVATE") {
				string display_name = model->_data_items[2] == this->mClientService->username ? model->_data_items[3] : model->_data_items[2];
				tempConv = new ClientConversation(display_name, hash_id, false);
				copy(model->_data_items.begin() + 2, model->_data_items.end(), back_inserter(tempConv->list_member));
				
			}
			else if (model->_data_items[1] == "GROUP") {

				string display_name = model->_data_items[2];
				tempConv = new ClientConversation(display_name, hash_id, true);
				copy(model->_data_items.begin() + 3, model->_data_items.end(), back_inserter(tempConv->list_member));
				
				//copy()
			}
			mListChat.insert(pair<string, ClientConversation*>(hash_id, tempConv));

			current_hash = tempConv->hash_id;
			target = Tools().StringToWstring(tempConv->display_name);
			ShowMessages();
			ShowGroups();

			LOG_INFO("Handle set hash");
		}
		break;
	}
	case IDC_FORM_CHAT_MSG_HANDLER_SERVER_DISCONNECTED:
		MessageBox(_T("Server Disconnected, exit application"), _T("Alert"), MB_ICONERROR | MB_TASKMODAL);
		break; 
	case IDC_FORM_CHAT_MSG_HANDLER_RECEIVE_CONVERSATION:
		if (lParam) {
			model = (SDataPackage*)lParam;
			string from_src = model->GetSrc();
			string hash_conversation_id = model->GetSHA256Des();
			string message = model->_data_items[0];
			
			ClientConversation* cCon = mListChat[hash_conversation_id];
			if (cCon) {
				if(notification){
					Tools().PlayGotMessSound();
				}
				cCon->list_mess.push_back(from_src + " : " + message);
				if (current_hash == hash_conversation_id) {
					wstring dest = StringHelper::utf8_decode(from_src);
					std::wstring mess = StringHelper::utf8_decode(message);
					
					CString flag = ATTACH_FILE_FLAG;
					int p1 = mess.find(flag);
					if (p1 != mess.length() - flag.GetLength() || p1 == -1) {


						wchar_t space(L' ');
					

						int line_size = 50;
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
									mess = (j == 0 && i == 0 ? dest + L" : " : L"······ ") + wstring(CString(mess.c_str()).Trim());
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


								mess = (j == 0 && i == 0 ? dest + L" : " : L"······ ") + wstring(CString(mess.c_str()).Trim());

								list_mess.InsertItem(count++, mess.c_str());
								i += end_s;
								end = line_size + i;
							}
						}
					}
					else {
						CString tmp = _T(" : ");
						list_mess.InsertItem(count++, dest.c_str() + tmp + mess.c_str());
					}


					// list_mess.InsertItem(count++, msg_received.c_str());
					LOG_INFO("Handle insert to chatbox");
				}
				else {
					cCon->pending_msg += 1;
				}
				ShowGroups();
				LOG_INFO("IDC_FORM_CHAT_MSG_HANDLER_RECEIVE_CONVERSATION() : add new message");
			}
			else {
				LOG_ERROR("IDC_FORM_CHAT_MSG_HANDLER_RECEIVE_CONVERSATION() : Cannot find conversation ");
			}
		}
		break;
	case IDC_FORM_CHAT_MSG_HANDLER_TRANSFER_FILE:
		//handle 
		if (lParam) {
			model = (SDataPackage*)lParam;

			string from_src = model->GetSrc();
			string hash_conversation_id = model->GetSHA256Des();
			ClientConversation* cCon = mListChat[hash_conversation_id];
			if (cCon) {
				if(cCon->transfering == false){
					//use current packet for chunk size
					cCon->transfering = true;
					cCon->InitFileTransferManagement(model->_data_items[0], model->GetTotalSize(), model->GetCurrentPacket());
					if (current_hash == model->GetSHA256Des()) {
						if (notification) {
							Tools().PlayGotMessSound();
						}
						m_ListFile.AddString(StringHelper::utf8_decode(cCon->ftm->_desFileName).c_str());
						int nCount = 0;
						nCount = m_ListFile.GetCount(); // Declare m_LisBox as a control in ClassView member variables.
						m_ListFile.SetCurSel(nCount - 1); // List box is zero based.

						m_ListFile.SetTopIndex(nCount - 1); // moves the view.
						CString _mess = CString((from_src + " : ").c_str()) + ATTACH_FILE_ICON + StringHelper::utf8_decode(cCon->ftm->_desFileName).c_str() + ATTACH_FILE_FLAG;
						list_mess.InsertItem(count++, _mess);
					}
					LOG_INFO("IDC_FORM_CHAT_MSG_HANDLER_BEGIN_TRANSFER_FILE() : add new message");
				}
				else {
					LOG_INFO("IDC_FORM_CHAT_MSG_HANDLER_BEGIN_TRANSFER_FILE() : transfering not allow send new file");
				}
			}
			else {
				LOG_ERROR("IDC_FORM_CHAT_MSG_HANDLER_BEGIN_TRANSFER_FILE() : Cannot find conversation ");
			}
		}

		//MessageBox(_T("INIT TRANSFER FILE !!!"), _T("Alert"), MB_ICONERROR);
		break;
	case IDC_FORM_CHAT_MSG_HANDLER_BEGIN_TRANSFER_FILE:
		if (lParam) {
			model = (SDataPackage*)lParam;
			string from_src = model->GetSrc();
			string hash_conversation_id = model->GetSHA256Des();
			ClientConversation* cCon = mListChat[hash_conversation_id];
			if (cCon) {
				cCon->ProcessChunk(model);
				//use current packet for chunk size
				LOG_INFO("IDC_FORM_CHAT_MSG_HANDLER_BEGIN_TRANSFER_FILE() : add new message");
			}
			else {
				LOG_ERROR("IDC_FORM_CHAT_MSG_HANDLER_BEGIN_TRANSFER_FILE() : Cannot find conversation ");
			}
		}
		
		//MessageBox(_T("IDC_FORM_CHAT_MSG_HANDLER_BEGIN_TRANSFER_FILE BEGIN TRANSFER FILE !!!"), _T("Alert"), MB_ICONERROR);
		break;
	case IDC_FORM_CHAT_MSG_HANDLER_END_TRANSFER_FILE:{
		model = (SDataPackage*)lParam;
		string from_src = model->GetSrc();
		string hash_conversation_id = model->GetSHA256Des();
		ClientConversation* cCon = mListChat[hash_conversation_id];
		//AfxMessageBox(L"Received file successfully!!!");
		MessageBox(_T("Received file successfully!!!"), _T("Alert"), MB_ICONINFORMATION);
		
		//MessageBox(_T("END TRANSFER FILE !!!"), _T("Alert"), MB_ICONERROR);
		break;
	}
	default:
		break;
	}
	return 0L;
}


void messenger::SetMessColor()
{
	switch (colorTheme)
	{
	case BRICK:
		list_mess.SetBkColor(RGB(253, 237, 236));
		list_groups.SetBkColor(RGB(253, 237, 236));
		list_friends.SetBkColor(RGB(253, 237, 236));
		break;
	case PURPLE:
		list_mess.SetBkColor(RGB(244, 236, 247));
		list_groups.SetBkColor(RGB(244, 236, 247));
		list_friends.SetBkColor(RGB(244, 236, 247));
		break;
	case BLUE:
		list_mess.SetBkColor(RGB(235, 245, 251));
		list_groups.SetBkColor(RGB(235, 245, 251));
		list_friends.SetBkColor(RGB(235, 245, 251));
		break;
	case GREEN:
		list_mess.SetBkColor(RGB(232, 246, 243));
		list_groups.SetBkColor(RGB(232, 246, 243));
		list_friends.SetBkColor(RGB(232, 246, 243));
		break;
	case SGREEN:
		list_mess.SetBkColor(RGB(234, 250, 241));
		list_groups.SetBkColor(RGB(234, 250, 241));
		list_friends.SetBkColor(RGB(234, 250, 241));
		break;
	case YELLOW:
		list_mess.SetBkColor(RGB(254, 249, 231));
		list_groups.SetBkColor(RGB(254, 249, 231));
		list_friends.SetBkColor(RGB(254, 249, 231));
		break;
	case ORANGE:
		list_mess.SetBkColor(RGB(251, 238, 230));
		list_groups.SetBkColor(RGB(251, 238, 230));
		list_friends.SetBkColor(RGB(251, 238, 230));
		break;
	case GRAY:
		list_mess.SetBkColor(RGB(242, 244, 244));
		list_groups.SetBkColor(RGB(242, 244, 244));
		list_friends.SetBkColor(RGB(242, 244, 244));
		break;
	case WHITE:
		list_mess.SetBkColor(RGB(255, 255, 255));
		list_groups.SetBkColor(RGB(255, 255, 255));
		list_friends.SetBkColor(RGB(255, 255, 255));
		break;
	default:
		break;
	}
}

// Show friends list to listcontrol component => use to show list user online
void messenger::ShowFriends()
{
	static CImageList imgList;
	CString strItem = _T("");
	imgList.DeleteImageList();
	imgList.Create(32, 32, ILC_COLOR32, 0, 0);

	if (list_friends.GetItemCount() != friends.size()-1) {
		list_friends.DeleteAllItems();
	}
	int buf = 0;
	for (int i = 0; i < friends.size(); ++i) {
		if (friends[i].c_str() != username) {
			//if (!targetIsGroup && (friends[i] == target || target.length() == 0)) {
			//	imgList.Add(AfxGetApp()->LoadIconW(IDI_USER_CHATTING));
			//}
			//else {
			//	imgList.Add(AfxGetApp()->LoadIconW(IDI_USER_ONL));
			//}
			imgList.Add(AfxGetApp()->LoadIconW(IDI_USER_ONL));
			strItem.Format(friends[i].c_str(), i);
			if (list_friends.GetItemCount() != friends.size()-1) {
				list_friends.InsertItem(i-buf, strItem, i-buf);
			}
		}
		else {
			buf++;
		}
	}
	list_friends.SetImageList(&imgList, LVSIL_NORMAL);
}


// Show groups chat, this case will use for private chat and group chat (both base on conversation list)
void messenger::ShowGroups()
{
	static CImageList imgList3;
	CString strItem = _T("");
	imgList3.DeleteImageList();
	imgList3.Create(34, 34, ILC_COLOR32, 0, 0);
	if (list_groups.GetItemCount() != mListChat.size()) {
		list_groups.DeleteAllItems();
	}
	int i = 0;
	groups.clear();
	for (std::pair<std::string, ClientConversation*> element : mListChat)
	{
		groups.push_back(element.second->display_name);
		int pending_msg = element.second->pending_msg;
		if (element.second->_is_group_msg == true) {
			if (element.second->display_name == Tools().WstringToString(target)) {
				imgList3.Add(AfxGetApp()->LoadIconW(IDI_GROUP_CHATTING));
			}
			else if (pending_msg == 0) {
				imgList3.Add(AfxGetApp()->LoadIconW(IDI_GROUP));
			}
			else {
				if (pending_msg > 9) {
					imgList3.Add(AfxGetApp()->LoadIconW(IDI_ICO_GROUP_9P));
				}
				else {
					imgList3.Add(AfxGetApp()->LoadIconW(IDI_ICO_GROUP_1 + pending_msg - 1));
				}
			}
		}
		else {
			if (element.second->display_name == Tools().WstringToString(target)) {
				imgList3.Add(AfxGetApp()->LoadIconW(IDI_USER_CHATTING));
			}
			else if (pending_msg == 0) {
				imgList3.Add(AfxGetApp()->LoadIconW(IDI_USER));
			}
			else {
				if (pending_msg > 9) {
					imgList3.Add(AfxGetApp()->LoadIconW(IDI_USER_9P));
				}
				else {
					imgList3.Add(AfxGetApp()->LoadIconW(IDI_USER_1 + pending_msg - 1));
				}
			}
		}

		if (list_groups.GetItemCount() != mListChat.size()) {
			strItem = CString(element.second->display_name.c_str());
			list_groups.InsertItem(i, strItem, i);
		}
		i++;
		//std::cout << element.first << " :: " << element.second << std::endl;
	}

	list_groups.SetImageList(&imgList3, LVSIL_NORMAL);

	/*
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
	*/
}

// Show group selected chat first
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
		//if (targetIsGroup && (groups[i].name == target || target.length() == 0)) {
		//	imgList3.Add(AfxGetApp()->LoadIconW(IDI_GROUP_CHATTING));
		//}
		//else {
		//	imgList3.Add(AfxGetApp()->LoadIconW(IDI_GROUP));
		//}
		//strItem.Format(groups[i].name.c_str(), i);
		if (list_groups.GetItemCount() != groups.size()) {
			list_groups.InsertItem(i, strItem, i);
		}
	}
	list_groups.SetImageList(&imgList3, LVSIL_NORMAL);
}


void messenger::ShowMessgesMember() {
	if (current_hash == "") {
		return;
	}

	m_messMember.ResetContent(); 
	for (string item : mListChat[current_hash]->list_member) {
		wstring msg_received = StringHelper::utf8_decode(item);
		m_messMember.AddString(msg_received.c_str());
	}
	
	//CString str = _T("");
	/*
	for (int i = 0; i < 10; i++) {

		//str.Format(_T("Item %d"), i);
		m_messMember.AddString(str);
	}
	*/
}

// Show message from path cache
void messenger::ShowMessages()
{
	SetChatBoxTitle();
	list_mess.DeleteAllItems();
	count = 0;
	if (current_hash == "") {
		return;
	}
	ShowMessgesMember();
	for (string item : mListChat[current_hash]->list_mess) {
		wstring msg_received = StringHelper::utf8_decode(item);
		list_mess.InsertItem(count++, msg_received.c_str());
	}

	RECT r;
	list_mess.GetItemRect(0, &r, LVIR_BOUNDS);
	list_mess.Scroll(CSize(0, (r.bottom - r.top) * list_mess.GetItemCount()));
	/* FROM FILE
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
	*/
}


// Get message from list message control (List control chat box)
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
	target = Tools().StringToWstring(mListChat[current_hash]->display_name);
	CString friendName = target.c_str();
	CString title;
	if (friendName.GetLength() > 0) {
		if (mListChat[current_hash]->_is_group_msg) {
			title = _T(" Chatting in: ") + friendName + _T(" ");
		}
		else {
			title = _T(" Chatting with: ") + friendName + _T(" ");
		}
	}
	else {
		title = _T(" Add friend or Create / Create group to start chat ");
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

// save message to file
void messenger::SaveMessages()
{
	std::wstring fileName = target;
	if (fileName != L"")
	{
		std::wstring path;
		//if (targetIsGroup) {
		//	path = L"./data/messages/" + account->GetUsername() + L"/groups/" + fileName + L".dat";
		//}
		//else {
		//	path = L"./data/messages/" + account->GetUsername() + L"/friends/" + fileName + L".dat";
		//}
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
	SetBtnIcon(IDC_ICON, IDI_AVATAR, 40);
	SetBtnIcon(IDC_BTN_NOTIFICATION, IDI_NOTIFICATION, 40);
}

void messenger::SetSendBtnIcon()
{
	SetBtnIcon(IDC_BTN_SEND, IDI_LOVE, 22);
	SetBtnIcon(IDC_BTN_SEND_ICON, IDI_SEND_EMOJI, 22);
	SetBtnIcon(IDC_BTN_SEND_FILE, IDI_SEND_FILE, 22);
	SetBtnIcon(IDC_BTN_SOUND, IDI_SOUND, 22);
	SetBtnIcon(IDC_BTN_DOC, IDI_DOC, 22);
	SetBtnIcon(IDC_BTN_IMAGE, IDI_IMAGE, 22);
	SetBtnIcon(IDC_BTN_VIDEO, IDI_VIDEO, 22);
	SetBtnIcon(IDC_BTN_CAMERA, IDI_CAMERA, 22);
	SetBtnIcon(IDC_BTN_LINK, IDI_LINK, 22);
}

void messenger::SetAddFriendIcon()
{
	SetBtnIcon(IDC_BTN_ADD_FRIEND, IDI_BRICK + colorTheme, 44);
	SetBtnIcon(IDC_BTN_ADD_GROUP, IDI_ADD_GROUP, 44);
}

void messenger::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//SaveMessages();
	CDialog::OnOK();
}


// Send message
void messenger::OnBnClickedBtnSend()
{
	if (current_hash == "") {
		MessageBox(_T("Please create a conversation to start!"), _T("Alert"), MB_ICONERROR);
		return;
	}
	
	// TODO: Add your control notification handler code here
	CString content;
	GetDlgItemText(IDC_MESS_CONTENT, content);
	string s_content = "";


	//pending
	//this->mClientService->SendPrivateMessage()
	if (content.GetLength() > 0) {
		wchar_t space(L' ');
		std::wstring mess(content);

		s_content = StringHelper::utf8_encode(mess);
		mListChat[current_hash]->list_mess.push_back(s_content);
		if (mListChat[current_hash]->_is_group_msg == true) {
			this->mClientService->SendGroupMessage(current_hash, s_content);

		} else{
			this->mClientService->SendPrivateMessage(current_hash, s_content);
		}

		int line_size = 50;
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
					mess = (j == 0 && i == 0 ? _T("Me : ") : _T("······ ")) + std::wstring(CString(mess.c_str()).Trim());
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


				mess = (j == 0 && i == 0 ? _T("Me : ") : _T("······ ")) + std::wstring(CString(mess.c_str()).Trim());
				string str_mess = StringHelper::utf8_encode(mess);
				mListChat[current_hash]->list_mess.push_back(str_mess);

				list_mess.InsertItem(count++, mess.c_str());
				i += end_s;
				end = line_size + i;
			}
		}
	}
	else {
		list_mess.InsertItem(count++, _T("Me : 🖤"));
		string str_mess = StringHelper::utf8_encode(_T("Me : 🖤"));
		mListChat[current_hash]->list_mess.push_back(str_mess);
		wstring tim = _T("🖤");
		s_content = StringHelper::utf8_encode(tim);
		if (mListChat[current_hash]->_is_group_msg == true) {
			this->mClientService->SendGroupMessage(current_hash, s_content);

		}
		else {
			this->mClientService->SendPrivateMessage(current_hash, s_content);
		}

	}
	mess_content.SetWindowTextW(L"");
	OnEnUpdateMessContent();
	OnEnChangeMessContent();
	mess_content.SetFocus();
}


// Add friends,
//void messenger::OnBnClickedBtnAddFriend()
//{
//
//	// TODO: Add your control notification handler code here
//	AddFriendDlg addFriend(accMa, account);
//	INT_PTR nRet = -1;
//	nRet = addFriend.DoModal();
//
//	CString res = addFriend.GetMessageValue(); //message between two dlg ~ name of conversation
//
//	// Handle the return value from DoModal
//	switch (nRet)
//	{
//	case -1:
//		AfxMessageBox(_T("Dialog box could not be created!"));
//		break;
//	case IDABORT:
//		// Do something
//		break;
//	case IDOK:
//		//AfxMessageBox(res);
//
//		//friends = accMa->GetFriends(*account);
//		//StartChat(friends[friends.size() - 1], false);
//		break;
//	case IDCANCEL:
//		// Do something
//		break;
//	default:
//		// Do something
//		break;
//	};
//	mess_content.SetFocus();
//	mess_content.SetSel(-1);
//}

void messenger::OnBnClickedIcon()
{
	// TODO: Add your control notification handler code here
	EditProfileDlg edit;

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

void messenger::SendFile(CString filetype) {
	if (current_hash == "") {
		MessageBox(_T("Please create a conversation to start!"), _T("Alert"), MB_ICONERROR);
		return;
	}
	// TODO: Add your control notification handler code here
	//AttachFile attachFile(true);
	//attachFile.DoModal();
	//const TCHAR szFilter[] = filetype;
	CString sFilePath = _T("");
	CString sFileName = _T("");
	CString sFileExt = _T("");

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filetype, this);
	if (dlg.DoModal() == IDOK)
	{
		sFilePath = dlg.GetPathName();
		sFileName = dlg.GetFileName();
		sFileExt = dlg.GetFileExt();
		//m_FilePathEditBox.SetWindowText(sFilePath);
		CT2A str_path(sFilePath);
		CT2A str_file_name(sFileName);
		CT2A str_file_ext(sFileExt);

		if (Tools().is_ascii(str_path.m_psz)) { // path -> chi chua tieng anh
			ClientConversation* cCon = mListChat[current_hash];
			if (cCon->transfering == true) {
				AfxMessageBox(L"Please wait transfer completed");
			}
			else {
				m_ListFile.AddString(sFilePath);
				int nCount = 0;
				nCount = m_ListFile.GetCount(); // Declare m_LisBox as a control in ClassView member variables.
				m_ListFile.SetCurSel(nCount - 1); // List box is zero based.

				m_ListFile.SetTopIndex(nCount - 1); // moves the view.
				cCon->transfering = true;
				mClientService->InitTransferFile(current_hash, 7680, str_path.m_psz, string(str_file_name.m_psz));
				cCon->transfering = false;//completed
				list_mess.InsertItem(count++, _T("Me : ") + ATTACH_FILE_ICON + sFilePath + ATTACH_FILE_FLAG);
			} // chen ten file, ki hieu
		}
		else {
			MessageBox(_T("Please choose another file or rename file and file path so that file path and file name contain ASCII characters only!"), _T("Alert"), MB_ICONERROR);
		}
	}
	mess_content.SetFocus();
	mess_content.SetSel(-1);
}

//send file
void messenger::OnBnClickedBtnSendFile()
{
	SendFile();
}


// use scroll list control chatbox
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

	int line = int(size.cx / 858);
	line += Tools().countChInString(_T("\r\n"), mess);
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

// use scroll list control chatbox

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

// Get selected item form ClistCtrl
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

//Start chat
void messenger::StartChat(std::wstring item, bool isGroup) {
	// SaveMessages();
	target = item;
	targetIsGroup = isGroup;
	ShowGroupsClick();
	ShowFriends();
	ShowMessages();
	mess_content.SetFocus();
	mess_content.SetSel(-1);
}


//right click mouse
void messenger::OnRightClickListFriends(NMHDR* pNMHDR, LRESULT* pResult)
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

	if (friends_name.size()) {


		std::vector<string> list_member_chat = StringHelper::VectorWideStringToString(friends_name);

		// TODO: Add your control notification handler code here
		//AddFriendDlg addFriend(accMa, account);
		INT_PTR nRet = -1;
		//nRet = addFriend.DoModal();

		CreateGroupDlg createGroupDlg(wstring(username), friends_name, friends, StringHelper::VectorStringToWideString(groups));
		nRet = createGroupDlg.DoModal();

		CString name_of_conversation = createGroupDlg.GetGroupName(); //message between two dlg ~ name of conversation
		string str_name_of_conversation;
		CT2A ascii(name_of_conversation);
		str_name_of_conversation = string(ascii.m_psz);
		list_member_chat = StringHelper::VectorWideStringToString(createGroupDlg.GetMembers());


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
		{
			if (list_member_chat.size() == 1) {
				this->mClientService->CreatePrivateConversation(list_member_chat[0], str_name_of_conversation);
			}
			else {
				bool check_existed_user = false;
				for (string user : list_member_chat) {
					if (this->mClientService->username == user) {
						check_existed_user = true;
						break;
					}
				}
				if (check_existed_user == false) {
					list_member_chat.insert(list_member_chat.begin(), this->mClientService->username);
				}
				this->mClientService->CreateGroupConversation(list_member_chat, str_name_of_conversation);
			}
		}
		break;
		case IDCANCEL:
			// Do something
			break;
		default:
			// Do something
			break;
		};
	}

	mess_content.SetFocus();
	mess_content.SetSel(-1);
	*pResult = 0;
}


void messenger::OnDoubleClickListFriends(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	int nSelected = pNMItemActivate->iItem;
	CString strItem = list_friends.GetItemText(nSelected, 0);
	if (strItem.GetLength() && !Tools().isIn(strItem, groups)) {

		CT2A strName(strItem);
		string des = strName.m_psz;

		this->mClientService->CreatePrivateConversation(string(strName.m_psz), string(strName.m_psz));
	}
	mess_content.SetFocus();
	mess_content.SetSel(-1);
	
	/*
	if (strItem.GetLength()) {
		StartChat(std::wstring(strItem), false);
	}
	*/
	*pResult = 0;

}


void messenger::OnBnClickedBtnSendIcon()
{
	// TODO: Add your control notification handler code here
	if (current_hash == "") {
		MessageBox(_T("Please create a conversation to start!"), _T("Alert"), MB_ICONERROR);
		return;
	}
	CString name;
	EmojiDlg emojiDlg(&mess_content, std::wstring(username));
	emojiDlg.DoModal();
	mess_content.SetFocus();
	mess_content.SetSel(-1);
	OnEnUpdateMessContent();
}


//void messenger::OnDoubleClickListGroups(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	
//	/*
//	CString strItem = list_groups.GetItemText(nSelected, 0);
//	if (strItem.GetLength()) {
//
//		StartChat(std::wstring(strItem), true);
//	}
//	*/
//	*pResult = 0;
//}

void messenger::OnRightClickListGroups(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nSelected = pNMItemActivate->iItem;
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
	if (groups_name.size()) {

		ChooseGroup chooseGroup;

		//ChooseFriend chooseFriend;
		INT_PTR nRet = -1;
		nRet = chooseGroup.DoModal();
		// Handle the return value from DoModal
		switch (nRet)
		{
		case -1:
			AfxMessageBox(_T("Dialog box could not be created!"));
			break;
		case ID_BTN_OUT_GROUP:
			//groups = accMa->GetGroups(*account);
			ShowGroups();
			break;
		case ID_BTN_START_CHAT_GROUP:
			for (std::pair<std::string, ClientConversation*> element : mListChat)
			{
				if (nSelected-- == 0) {
					current_hash = element.second->hash_id;
					element.second->pending_msg = 0;//read
					break;
				}
			}

			ShowMessages();
			ShowGroups();
			break;
		case ID_BTN_MODIFY_GROUP:
			//groups = accMa->GetGroups(*account);
			ShowGroups();
		default:
			// Do something
			break;
		};
	}
	mess_content.SetFocus();
	mess_content.SetSel(-1);
	*pResult = 0;
}

void messenger::OnBnClickedBtnNotification()
{
	Tools().PlayGotMessSound();
	// TODO: Add your control notification handler code here
	if (notification) {
		SetBtnIcon(IDC_BTN_NOTIFICATION, IDI_UNNOTIFICATION, 40);
	}
	else {
		SetBtnIcon(IDC_BTN_NOTIFICATION, IDI_NOTIFICATION, 40);
	}
	notification = !notification;
	mess_content.SetFocus();
	mess_content.SetSel(-1);
}


void messenger::OnBnClickedBtnAddGroup()
{
	// TODO: Add your control notification handler code here
	CreateGroupDlg createGroupDlg(wstring(username), vector<wstring>(), friends, StringHelper::VectorStringToWideString(groups));

	INT_PTR nRet = -1;
	nRet = createGroupDlg.DoModal();

	CString name_of_conversation = createGroupDlg.GetGroupName(); //message between two dlg ~ name of conversation
	string str_name_of_conversation;
	CT2A ascii(name_of_conversation);
	str_name_of_conversation = string(ascii.m_psz);
	vector<string> list_member_chat = StringHelper::VectorWideStringToString(createGroupDlg.GetMembers());


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
	{
		if (list_member_chat.size() == 1) {
			this->mClientService->CreatePrivateConversation(list_member_chat[0], str_name_of_conversation);
		}
		else {
			bool check_existed_user = false;
			for (string user : list_member_chat) {
				if (this->mClientService->username == user) {
					check_existed_user = true;
					break;
				}
			}
			if (check_existed_user == false) {
				list_member_chat.insert(list_member_chat.begin(), this->mClientService->username);
			}
			this->mClientService->CreateGroupConversation(list_member_chat, str_name_of_conversation);
		}
	}
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

void messenger::OnDoubleClickFileItem(NMHDR* pNMHDR, LRESULT* pResult) {

}

void messenger::OnLbnDblclkListFileTransfer()
{
	// TODO: Add your control notification handler code here


	// TODO: Add your control notification handler code here
	CListBox* pList1 = (CListBox*)(GetDlgItem(IDC_LIST_FILE_TRANSFER));
	int nSel = pList1->GetCurSel();
	if (nSel != LB_ERR)
	{
		CString ItemSelected;
		pList1->GetText(nSel, ItemSelected);
		
		//CT2A ascii(ItemSelected);
		//system(ascii.m_psz);
		ConfirmDlg confirmDlg;
		confirmDlg.SetMess(L"File \"" + std::wstring(ItemSelected) + L"\" may contain viruses that may be harmful to your computer!\r\nAre you sure to open this file?");
		if (confirmDlg.DoModal() == IDOK) {
			ShellExecute(0, 0, ItemSelected, 0, 0, SW_SHOW);
		}
		//AfxMessageBox(ItemSelected);
	}
	else {
		//return;
	}

}


void messenger::OnNMClickListMess(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nSelected = pNMItemActivate->iItem; // get item -> vi tri
	CString strItem = list_mess.GetItemText(nSelected, 0); // lay vi tri dau tien
	if (strItem.GetLength()) {
		CString flag = ATTACH_FILE_FLAG;
		bool isFile = false;
		int p1 = strItem.Find(flag);
		if (p1 == strItem.GetLength() - flag.GetLength()) {
			CString icon = ATTACH_FILE_ICON;
			int p2 = strItem.Find(icon); // vi tri icon
			if (p2 != -1) {
				p2 += icon.GetLength(); // do dai icon
				CString path = strItem.Mid(p2, p1 - p2); // lay duong dan
				ConfirmDlg confirmDlg;
				confirmDlg.SetMess(L"File \"" + std::wstring(path) + L"\" may contain viruses that may be harmful to your computer!\r\nAre you sure to open this file?");
				if (confirmDlg.DoModal() == IDOK) {
					ShellExecute(0, 0, path, 0, 0, SW_SHOW);
				}
			}

		}
	}
	*pResult = 0;
}


void messenger::OnBnClickedBtnLink()
{
	if (current_hash == "") {
		MessageBox(_T("Please create a conversation to start!"), _T("Alert"), MB_ICONERROR);
		return;
	}
	// TODO: Add your control notification handler code here
	AddFriendDlg sendLink;
	if (sendLink.DoModal() == IDOK) {
		CString link = sendLink.GetMessageValue();
		CString _mess = _T("Me : ") + ATTACH_FILE_ICON + link + ATTACH_FILE_FLAG;
		list_mess.InsertItem(count++, _mess);
		string str_mess = StringHelper::utf8_encode(wstring(_mess));
		mListChat[current_hash]->list_mess.push_back(str_mess);
		string s_content = StringHelper::utf8_encode(wstring(ATTACH_FILE_ICON + link + ATTACH_FILE_FLAG));
		if (mListChat[current_hash]->_is_group_msg == true) {
			this->mClientService->SendGroupMessage(current_hash, s_content);
		}
		else {
			this->mClientService->SendPrivateMessage(current_hash, s_content);
		}
	}

}


void messenger::OnBnClickedBtnCamera()
{
	if (current_hash == "") {
		MessageBox(_T("Please create a conversation to start!"), _T("Alert"), MB_ICONERROR);
		return;
	}
	// TODO: Add your control notification handler code here
}


void messenger::OnBnClickedBtnImage()
{
	// TODO: Add your control notification handler code here
	if (current_hash == "") { // kiem tra xem chon chua
		MessageBox(_T("Please create a conversation to start!"), _T("Alert"), MB_ICONERROR);
		return;
	}
	SendFile(_T("Image Files (*.png, *.jpg, *.jpeg, *.gif)|*.png; *.jpg; *.jpeg; *.gif||"));
}


void messenger::OnBnClickedBtnVideo()
{
	// TODO: Add your control notification handler code here
	if (current_hash == "") {
		MessageBox(_T("Please create a conversation to start!"), _T("Alert"), MB_ICONERROR);
		return;
	}
	SendFile(_T("Video Files (*.mp4, *.flv, *.avi, *.wmv)|*.mp4; *.flv; *.avi; *.wmv||")); // loc
}


void messenger::OnBnClickedBtnSound()
{
	// TODO: Add your control notification handler code here
	if (current_hash == "") {
		MessageBox(_T("Please create a conversation to start!"), _T("Alert"), MB_ICONERROR);
		return;
	}
	SendFile(_T("Sound Files (*.mp3, *.wav, *.flac, *.aac)|*.mp3; *.wav; *.flac; *.aac||"));
}


void messenger::OnBnClickedBtnDoc()
{
	// TODO: Add your control notification handler code here
	if (current_hash == "") {
		MessageBox(_T("Please create a conversation to start!"), _T("Alert"), MB_ICONERROR);
		return;
	}
	SendFile(_T("Documents (*txt, *.doc, *.docx, *.pdf, *.epub)|*.doc; *.docx; *.pdf; *.epub||"));
}


void messenger::OnNMClickListGroups(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nSelected = pNMItemActivate->iItem;
	CString selected = list_groups.GetItemText(nSelected, 0);

	// 4 3 2 1 
	// 3 2 1 0 (n-i-1) i = 0 n= 4 => 3
	// i = 1 => rever = 2 
	//
	//int reverse_idx_counter = mListChat.size() - nSelected - 1;
	if (selected.GetLength()) {
		for (std::pair<std::string, ClientConversation*> element : mListChat)
		{
			if (nSelected-- == 0) {
				current_hash = element.second->hash_id;
				element.second->pending_msg = 0;//read
				break;
			}
			//std::cout << element.first << " :: " << element.second << std::endl;
		}

		ShowMessages();
		ShowGroups();
	}
	mess_content.SetFocus();
	mess_content.SetSel(-1);
	*pResult = 0;
}


//void messenger::OnNMClickListFriends(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: Add your control notification handler code here
//
//	*pResult = 0;
//}


void messenger::OnBnClickedBtnAddFriend()
{
	// TODO: Add your control notification handler code here
	//colorTheme = (colorTheme + 1) % 8;
	EmojiDlg emojiDlg(true);
	INT_PTR nRet = emojiDlg.DoModal();
	if (emojiDlg.color_val != -1) {
		this->colorTheme = emojiDlg.color_val;
		SetBtnIcon(IDC_BTN_ADD_FRIEND, IDI_BRICK + colorTheme, 44);
	}
	
	SetMessColor();
	this->RedrawWindow();
	mess_content.SetFocus();
	mess_content.SetSel(-1);
}
