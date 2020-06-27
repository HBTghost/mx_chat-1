// EmojiDlg.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "EmojiDlg.h"
#include "afxdialogex.h"
#include "ShowEmoijDlg.h"
#include "CustomColorDlg.h"
#include <vector>
#include <string>

// EmojiDlg dialog

IMPLEMENT_DYNAMIC(EmojiDlg, CDialog)

EmojiDlg::EmojiDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_EmojiDlg, pParent)
{

}

EmojiDlg::EmojiDlg(CEdit* mess, std::wstring username)
	: CDialog(IDD_EmojiDlg, nullptr)
	, mess(mess)
	, username(username)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

EmojiDlg::EmojiDlg(COLORREF a, COLORREF b)
	: CDialog(IDD_EmojiDlg, nullptr)
	, a(a)
	, b(b)
{
	this->color = true;
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APP);
}

EmojiDlg::~EmojiDlg()
{
}

void EmojiDlg::InitList() {
	static CImageList imgList;
	imgList.DeleteImageList();
	imgList.Create(24, 24, ILC_COLOR32, 0, 0);
	CString strItem;
	std::vector<CString> list_item_name;
	if (color) {
		imgList.Add(AfxGetApp()->LoadIconW(IDI_BRICK));
		strItem.Format(_T("Brick", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_PURPLE));
		strItem.Format(_T("Purple", 1));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_BLUE));
		strItem.Format(_T("Blue", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_GREEN1));
		strItem.Format(_T("Green 1", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_GREEN2));
		strItem.Format(_T("Green 2", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_YELLOW));
		strItem.Format(_T("Yellow", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_ORANGE));
		strItem.Format(_T("Orange", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_GRAY));
		strItem.Format(_T("Gray", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_COLOR));
		strItem.Format(_T("Custom", 0));
		list_item_name.push_back(strItem);
	}
	else {
		imgList.Add(AfxGetApp()->LoadIconW(IDI_RECENT));
		strItem.Format(_T("Recent", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_SEND_EMOJI));
		strItem.Format(_T("Emotion", 1));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_LOVE));
		strItem.Format(_T("Icon", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_NATURE));
		strItem.Format(_T("Nature", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_FOOD));
		strItem.Format(_T("Food", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_ACTIVITY));
		strItem.Format(_T("Activity", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_TRAVEL));
		strItem.Format(_T("Travel", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_OBJECT));
		strItem.Format(_T("Object", 0));
		list_item_name.push_back(strItem);

		imgList.Add(AfxGetApp()->LoadIconW(IDI_OTHER));
		strItem.Format(_T("Other", 0));
		list_item_name.push_back(strItem);
	}

	for (int i = 0; i < list_item_name.size(); ++i) {
		list_emoji.InsertItem(i, list_item_name[i], i);
	}
	list_emoji.SetImageList(&imgList, LVSIL_SMALL);
}

void EmojiDlg::DoDataExchange(CDataExchange* pDX)
{
CDialog::DoDataExchange(pDX);
DDX_Control(pDX, IDC_LIST_EMOJI, list_emoji);
InitList();
}


BEGIN_MESSAGE_MAP(EmojiDlg, CDialog)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_LIST_EMOJI, &EmojiDlg::OnNMClickListEmoji)
END_MESSAGE_MAP()


BOOL EmojiDlg::OnInitDialog()
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

void EmojiDlg::OnPaint()
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

// EmojiDlg message handlers

void EmojiDlg::OnNMClickListEmoji(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nSelected = pNMItemActivate->iItem;
	CString strItem = list_emoji.GetItemText(nSelected, 0);

	if (strItem.GetLength() > 0) {
		if (color) {
			if (strItem == _T("Brick")) {
				a = RGB(253, 237, 236);
				b = RGB(245, 183, 177);
			}
			else if (strItem == _T("Purple")) {
				a = RGB(244, 236, 247);
				b = RGB(210, 180, 222);
			}
			else if (strItem == _T("Blue")) {
				a = RGB(235, 245, 251);
				b = RGB(174, 214, 241);
			}
			else if (strItem == _T("Green 1")) {
				a = RGB(232, 246, 243);
				b = RGB(162, 217, 206);
			}
			else if (strItem == _T("Green 2")) {
				a = RGB(234, 250, 241);
				b = RGB(171, 235, 198);
			}
			else if (strItem == _T("Yellow")) {
				a = RGB(254, 249, 231);
				b = RGB(249, 231, 159);
			}
			else if (strItem == _T("Orange")) {
				a = RGB(251, 238, 230);
				b = RGB(237, 187, 153);
			}
			else if (strItem == _T("Gray")) {
				a = RGB(242, 244, 244);
				b = RGB(204, 209, 209);
			}
			else {
				CustomColorDlg customColor(a, b);
				if (customColor.DoModal() == IDOK) {
					a = customColor.a;
					b = customColor.b;
				}
				else {
					goto CON;
				}
			}
			OnOK();
		}
		else {
			ShowEmoijDlg showEmojiDlg(mess, strItem, username);
			showEmojiDlg.DoModal();
		}
	}

	// TODO: Add your control notification handler code here
CON:
	*pResult = 0;
}
