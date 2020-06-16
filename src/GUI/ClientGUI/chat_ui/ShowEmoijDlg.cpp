// ShowEmoijDlg.cpp : implementation file
//

#include "pch.h"
#include "chat_ui.h"
#include "ShowEmoijDlg.h"
#include "afxdialogex.h"
#include <string>
#include "Tools.hpp"


// ShowEmoijDlg dialog

IMPLEMENT_DYNAMIC(ShowEmoijDlg, CDialog)

ShowEmoijDlg::ShowEmoijDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ShowEmoijDlg, pParent)
{

}

ShowEmoijDlg::ShowEmoijDlg(CEdit* mess, CString type, std::wstring username)
	: CDialog(IDD_ShowEmoijDlg, nullptr)
	, mess(mess)
	, type(type)
	, username(username)
{
	std::wstring fileName = username;
	if (fileName != L"")
	{
		std::wstring path = L"./data/messages/" + fileName + L"/recentEmojis.dat";
		recent = Tools().ReadFile(path);
	}
}

void ShowEmoijDlg::HandleRecent() {
	for (int i = 0; i < recent.size(); ++i) {
		for (int j = recent.size() - 1; j > i; --j) {
			if (recent[i] == recent[j]) {
				recent.erase(recent.begin() + j);
			}
		}
	}
	recent.resize(recent.size() > 20 ? 20 : recent.size());
}

ShowEmoijDlg::~ShowEmoijDlg()
{
	std::wstring fileName = username;
	if (fileName != L"")
	{
		std::wstring path = L"./data/messages/" + fileName + L"/recentEmojis.dat";
		HandleRecent();
		Tools().WriteToFile(path, recent);
	}
}

void ShowEmoijDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ICON, list_icon_ctrl);
	InitListIcon();
}

void ShowEmoijDlg::InitListIcon()
{
	if (type == _T("Emotion")) {
		std::vector<std::wstring> tmp{ L"😀", L"😁", L"😂", L"🤣", L"😄", L"😅", L"😆", L"😉", L"😊", L"😋", L"😎", L"😍", L"😘", L"😗", L"😙", L"😚", L"☺️", L"🙂", L"🤗", L"🤩", L"🤔", L"🤨", L"😐", L"😑", L"😶", L"🙄", L"😏", L"😣", L"😥", L"😮", L"🤐", L"😯", L"😪", L"😫", L"😴", L"😌", L"😛", L"😜", L"😝", L"🤤", L"😒", L"😓", L"😔", L"😕", L"🙃", L"🤑", L"😲", L"☹️", L"🙁", L"😖", L"😞", L"😟", L"😤", L"😢", L"😭", L"😦", L"😧", L"😨", L"😩", L"🤯", L"😬", L"😰", L"😱", L"😳", L"🤪", L"😵", L"😡", L"😠", L"🤬", L"😷", L"🤒", L"🤕", L"🤢", L"🤮", L"🤧", L"😇", L"🤠", L"🤡", L"🤥", L"🤫", L"🤭", L"🧐", L"🤓", L"😈", L"👿", L"👹", L"👺", L"💀", L"👻", L"👽", L"🤖", L"💩", L"😺", L"😸", L"😹", L"😻", L"😼", L"😽", L"🙀", L"😿", L"😾" };
		list_icon = tmp;
	} else if (type == _T("Icon")) {
		std::vector<std::wstring> tmp{ L"❤️", L"🧡", L"💛", L"💚", L"💙", L"💜", L"🖤", L"💔", L"❣️", L"💕", L"💞", L"💓", L"💗", L"💖", L"💘", L"💝", L"💟", L"☮️", L"✝️", L"☪️", L"🕉", L"☸️", L"✡️", L"🔯", L"🕎", L"☯️", L"☦️", L"🛐", L"⛎", L"♈️", L"♉️", L"♊️", L"♋️", L"♌️", L"♍️", L"♎️", L"♏️", L"♐️", L"♑️", L"♒️", L"♓️", L"🆔", L"⚛️", L"🉑", L"☢️", L"☣️", L"📴", L"📳", L"🈶", L"🈚️", L"🈸", L"🈺", L"🈷️", L"✴️", L"🆚", L"💮", L"🉐", L"㊙️", L"㊗️", L"🈴", L"🈵", L"🈹", L"🈲", L"🅰️", L"🅱️", L"🆎", L"🆑", L"🅾️", L"🆘", L"❌", L"⭕️", L"🛑", L"⛔️", L"📛", L"🚫", L"💯", L"💢", L"♨️", L"🚷", L"🚯", L"🚳", L"🚱", L"🔞", L"📵", L"🚭", L"❗️", L"❔", L"‼️", L"⁉️", L"🔅", L"🔆", L"〽️", L"⚠️", L"🚸", L"🔱", L"⚜️", L"🔰", L"♻️", L"✅", L"🈯️", L"💹", L"❇️", L"✳️", L"❎", L"🌐", L"💠", L"Ⓜ️", L"🌀", L"💤", L"🏧", L"🚾", L"♿️", L"🅿️", L"🈳", L"🈂️", L"🛂", L"🛃", L"🛄", L"🛅", L"🚹", L"🚺", L"🚼", L"🚻", L"🚮", L"🎦", L"📶", L"🈁", L"🔣", L"ℹ️", L"🔤", L"🔡", L"🔠", L"🆖", L"🆗", L"🆙", L"🆒", L"🆕", L"🆓", L"🔟", L"🔢", L"⏏️", L"▶️", L"⏸", L"⏯", L"⏹", L"⏺", L"⏭", L"⏮", L"⏩", L"⏪", L"⏫", L"⏬", L"◀️", L"🔼", L"🔽", L"➡️", L"⬅️", L"⬆️", L"⬇️", L"↗️", L"↘️", L"↙️", L"↖️", L"↕️", L"↔️", L"↪️", L"↩️", L"⤴️", L"⤵️", L"🔀", L"🔁", L"🔂", L"🔄", L"🔃", L"🎵", L"🎶", L"➕", L"➖", L"➗", L"✖️", L"♾", L"💲", L"💱", L"™️", L"©️", L"®️", L"〰️", L"➰", L"➿", L"🔚", L"🔙", L"🔛", L"🔝", L"🔜", L"✔️", L"☑️", L"🔘", L"⚪️", L"⚫️", L"🔴", L"🔵", L"🔺", L"🔻", L"🔸", L"🔹", L"🔶", L"🔷", L"🔳", L"🔲", L"▪️", L"▫️", L"◾️", L"◽️", L"◼️", L"◻️", L"⬛️", L"⬜️", L"🔈", L"🔇", L"🔉", L"🔊", L"🔔", L"🔕", L"📣", L"📢", L"👁‍🗨", L"💬", L"💭", L"🗯", L"♠️", L"♣️", L"♥️", L"♦️", L"🃏", L"🎴", L"🀄️", L"🕐", L"🕑", L"🕒", L"🕓", L"🕔", L"🕕", L"🕖", L"🕗", L"🕘", L"🕙", L"🕚", L"🕛", L"🕜", L"🕝", L"🕞", L"🕟", L"🕠", L"🕡", L"🕢", L"🕣", L"🕤", L"🕥", L"🕦", L"🕧"};
		list_icon = tmp;
	} else if (type == _T("Nature")) {
		std::vector<std::wstring> tmp{ L"🐶", L"🐱", L"🐭", L"🐹", L"🐰", L"🦊", L"🐻", L"🐼", L"🐨", L"🐯", L"🦁", L"🐮", L"🐷", L"🐽", L"🐸", L"🐵", L"🙈", L"🙉", L"🙊", L"🐒", L"🐔", L"🐧", L"🐦", L"🐤", L"🐣", L"🐥", L"🦆", L"🦅", L"🦇", L"🐺", L"🐗", L"🐴", L"🦄", L"🐝", L"🐛", L"🦋", L"🐌", L"🐚", L"🐞", L"🐜", L"🦗", L"🕷", L"🕸", L"🦂", L"🐢", L"🐍", L"🦎", L"🦖", L"🦕", L"🐙", L"🦑", L"🦐", L"🦀", L"🐡", L"🐠", L"🐟", L"🐬", L"🐳", L"🐋", L"🦈", L"🐊", L"🐅", L"🐆", L"🦓", L"🦍", L"🐘", L"🦏", L"🐪", L"🐫", L"🦒", L"🐃", L"🐂", L"🐄", L"🐎", L"🐖", L"🐏", L"🐑", L"🐐", L"🦌", L"🐕", L"🐩", L"🐈", L"🐓", L"🦃", L"🕊", L"🐇", L"🐁", L"🐀", L"🐿", L"🦔", L"🐾", L"🐉", L"🐲", L"🌵", L"🎄", L"🌲", L"🌳", L"🌴", L"🌱", L"🌿", L"☘️", L"🍀", L"🎍", L"🎋", L"🍃", L"🍂", L"🍁", L"🍄", L"🌾", L"💐", L"🌷", L"🌹", L"🥀", L"🌺", L"🌸", L"🌼", L"🌻", L"🌞", L"🌝", L"🌛", L"🌜", L"🌚", L"🌕", L"🌖", L"🌗", L"🌘", L"🌑", L"🌒", L"🌓", L"🌔", L"🌙", L"🌎", L"🌍", L"🌏", L"💫", L"⭐️", L"🌟", L"✨", L"⚡️", L"☄️", L"💥", L"🔥", L"🌪", L"🌈", L"☀️", L"🌤", L"⛅️", L"🌥", L"☁️", L"🌦", L"🌧", L"⛈", L"🌩", L"🌨", L"❄️", L"☃️", L"⛄️", L"🌬", L"💨", L"💧", L"💦", L"☔️", L"☂️", L"🌊", L"🌫"};
		list_icon = tmp;
	} else if (type == _T("Food")) {
		std::vector<std::wstring> tmp{ L"🍏", L"🍎", L"🍐", L"🍊", L"🍋", L"🍌", L"🍉", L"🍇", L"🍓", L"🍈", L"🍒", L"🍑", L"🍍", L"🥥", L"🥝", L"🍅", L"🍆", L"🥑", L"🥦", L"🥒", L"🌶", L"🌽", L"🥕", L"🥔", L"🍠", L"🥐", L"🍞", L"🥖", L"🥨", L"🧀", L"🥚", L"🍳", L"🥞", L"🥓", L"🥩", L"🍗", L"🍖", L"🌭", L"🍔", L"🍟", L"🍕", L"🥪", L"🥙", L"🌮", L"🌯", L"🥗", L"🥘", L"🥫", L"🍝", L"🍜", L"🍲", L"🍛", L"🍣", L"🍱", L"🥟", L"🍤", L"🍙", L"🍚", L"🍘", L"🍥", L"🥠", L"🍢", L"🍡", L"🍧", L"🍨", L"🍦", L"🥧", L"🍰", L"🎂", L"🍮", L"🍭", L"🍬", L"🍫", L"🍿", L"🍩", L"🍪", L"🌰", L"🥜", L"🍯", L"🥛", L"🍼", L"☕️", L"🍵", L"🥤", L"🍶", L"🍺", L"🍻", L"🥂", L"🍷", L"🥃", L"🍸", L"🍹", L"🍾", L"🥄", L"🍴", L"🍽", L"🥣", L"🥡", L"🥢" };
		list_icon = tmp;
	} else if (type == _T("Activity")) {
		std::vector<std::wstring> tmp{ L"⚽️", L"🏀", L"🏈", L"⚾️", L"🏐", L"🏉", L"🎾", L"🎱", L"🏓", L"🏸", L"🥅", L"🏒", L"🏑", L"🏏", L"⛳️", L"🏹", L"🎣", L"🥊", L"🥋", L"🎽", L"⛸", L"🥌", L"🛷", L"🎿", L"⛷", L"🏂", L"🏋️‍♀", L"🏋️‍♂️", L"🤼‍♀", L"🤸‍♀️", L"", L"⛹🏿‍♂️", L"🤺", L"🤾‍♀️", L"🤾‍♂️", L"🏌️‍♀️", L"🏌️‍♂️", L"🏇", L"🧘‍♀️", L"🏄‍♀️", L"🏊‍♀️", L"🏊‍♂️", L"🤽‍♀️", L"🤽‍♂️", L"🚣‍♀️", L"🧗‍♀️", L"🚵‍♀️", L"🚴‍♀️", L"🏆", L"🥇", L"🥈", L"🥉", L"🏅", L"🎖", L"🏵", L"🎗", L"🎫", L"🎟", L"🎪", L"🤹‍♀️", L"", L"🎭", L"🎨", L"🎬", L"🎤", L"🎧", L"🎼", L"🎹", L"🥁", L"🎷", L"🎺", L"🎸", L"🎻", L"🎲", L"🧩", L"♟", L"🎯", L"🎳", L"🎮", L"🎰" };
		list_icon = tmp;
	} else if (type == _T("Travel")) {
		std::vector<std::wstring> tmp{ L"🚗", L"🚕", L"🚙", L"🚌", L"🚎", L"🏎", L"🚓", L"🚑", L"🚒", L"🚐", L"🚚", L"🚛", L"🚜", L"🛴", L"🚲", L"🛵", L"🏍", L"🚨", L"🚔", L"🚍", L"🚘", L"🚖", L"🚡", L"🚠", L"🚟", L"🚃", L"🚋", L"🚞", L"🚝", L"🚄", L"🚅", L"🚈", L"🚂", L"🚆", L"🚇", L"🚊", L"🚉", L"✈️", L"🛫", L"🛬", L"🛩", L"💺", L"🛰", L"🚀", L"🛸", L"🚁", L"🛶", L"⛵️", L"🚤", L"🛥", L"🛳", L"⛴", L"🚢", L"⚓️", L"⛽️", L"🚧", L"🚦", L"🚥", L"🚏", L"🗺", L"🗿", L"🗽", L"🗼", L"🏰", L"🏯", L"🏟", L"🎡", L"🎢", L"🎠", L"⛲️", L"⛱", L"🏖", L"🏝", L"🏜", L"🌋", L"⛰", L"🏔", L"🗻", L"🏕", L"⛺️", L"🏠", L"🏡", L"🏘", L"🏚", L"🏗", L"🏭", L"🏢", L"🏬", L"🏣", L"🏤", L"🏥", L"🏦", L"🏨", L"🏪", L"🏫", L"🏩", L"💒", L"🏛", L"⛪️", L"🕌", L"🕍", L"🕋", L"⛩", L"🛤", L"🛣", L"🗾", L"🎑", L"🏞", L"🌅", L"🌄", L"🌠", L"🎇", L"🎆", L"🌇", L"🌆", L"🏙", L"🌃", L"🌌", L"🌉", L"🌁" };
		list_icon = tmp;
	} else if (type == _T("Object")) {
		std::vector<std::wstring> tmp{ L"⌚️", L"📱", L"📲", L"💻", L"⌨️", L"🖥", L"🖨", L"🖱", L"🖲", L"🕹", L"🗜", L"💽", L"💾", L"💿", L"📀", L"📼", L"📷", L"📸", L"📹", L"🎥", L"📽", L"🎞", L"📞", L"☎️", L"📟", L"📠", L"📺", L"📻", L"🎙", L"🎚", L"🎛", L"⏱", L"⏲", L"⏰", L"🕰", L"⌛️", L"⏳", L"📡", L"🔋", L"🔌", L"💡", L"🔦", L"🕯", L"🗑", L"🛢", L"💸", L"💵", L"💴", L"💶", L"💷", L"💰", L"💳", L"💎", L"⚖️", L"🔧", L"🔨", L"⚒", L"🛠", L"⛏", L"🔩", L"⚙️", L"⛓", L"🔫", L"💣", L"🔪", L"🗡", L"⚔️", L"🛡", L"🚬", L"⚰️", L"⚱️", L"🏺", L"🔮", L"📿", L"💈", L"⚗️", L"🔭", L"🔬", L"🕳", L"💊", L"💉", L"🌡", L"🚽", L"🚰", L"🚿", L"🛁", L"🛀", L"🛎", L"🔑", L"🗝", L"🚪", L"🛋", L"🛏", L"🛌", L"🖼", L"🛍", L"🛒", L"🎁", L"🎈", L"🎏", L"🎀", L"🎊", L"🎉", L"🎎", L"🏮", L"🎐", L"✉️", L"📩", L"📨", L"📧", L"💌", L"📥", L"📤", L"📦", L"🏷", L"📪", L"📫", L"📬", L"📭", L"📮", L"📯", L"📜", L"📃", L"📄", L"📑", L"📊", L"📈", L"📉", L"🗒", L"🗓", L"📆", L"📅", L"📇", L"🗃", L"🗳", L"🗄", L"📋", L"📁", L"📂", L"🗂", L"🗞", L"📰", L"📓", L"📔", L"📒", L"📕", L"📗", L"📘", L"📙", L"📚", L"📖", L"🔖", L"🔗", L"📎", L"🖇", L"📐", L"📏", L"📌", L"📍", L"✂️", L"🖊", L"🖋", L"✒️", L"🖌", L"🖍", L"📝", L"✏️", L"🔍", L"🔎", L"🔏", L"🔐", L"🔒", L"🔓" };
		list_icon = tmp;
	}
	else if (type == _T("Recent")) {
		for (auto x : recent) {
			list_icon.push_back(std::wstring(x));
		}
	}
	else {
		std::vector<std::wstring> tmp{ L"🤲", L"👐", L"🙌", L"👏", L"🤝", L"👍", L"👎", L"👊", L"✊", L"🤛", L"🤜", L"🤞", L"✌️", L"🤟", L"🤘", L"👌", L"👈", L"👉", L"👆", L"👇", L"☝️", L"✋", L"🤚", L"🖐", L"🖖", L"👋", L"🤙", L"💪", L"🖕", L"✍️", L"🙏", L"💍", L"💄", L"💋", L"👄", L"👅", L"👂", L"👃", L"👣", L"👁", L"👀", L"🧠", L"🗣", L"👤", L"👥", L"🧥", L"👚", L"👕", L"👖", L"👔", L"👗", L"👙", L"👘", L"👠", L"👡", L"👢", L"👞", L"👟", L"🧦", L"🧤", L"🧣", L"🎩", L"🧢", L"👒", L"🎓", L"⛑", L"👑", L"👝", L"👛", L"👜", L"💼", L"🎒", L"👓", L"🕶", L"🌂", L"👶", L"👧", L"🧒", L"👦", L"👩", L"🧑", L"👨", L"👵", L"🧓", L"👴", L"👲", L"👳‍♀️", L"👳‍♂️", L"🧕", L"🧔", L"👱‍♂️", L"👱‍♀️", L"👨", L"👩‍", L"‍♀️", L"♂️", L"👮‍♀️", L"👮‍♂️", L"👷‍♀️", L"👷‍♂️", L"💂‍♀️", L"💂‍♂️", L"🕵️‍♀️", L"🕵️‍♂️", L"👩‍⚕️", L"👨‍⚕️", L"👩‍🌾", L"👨‍🌾", L"👩‍🍳", L"👨‍🍳", L"👩‍🎓", L"👨‍🎓", L"👩‍🎤", L"👨‍🎤", L"👩‍🏫", L"👨‍🏫", L"👩‍🏭", L"👨‍🏭", L"👩‍💻", L"👨‍💻", L"👩‍💼", L"👨‍💼", L"👩‍🔧", L"👨‍🔧", L"👩‍🔬", L"👨‍🔬", L"👩‍🎨", L"👨‍🎨", L"👩‍🚒", L"👨‍🚒", L"👩‍✈️", L"👨‍✈️", L"👩‍🚀", L"👨‍🚀", L"👩‍⚖️", L"👨‍⚖️", L"👰", L"🤵", L"👸", L"🤴", L"🤶", L"🎅", L"🧙‍♀️", L"🧙‍♂️", L"🧝‍♀️", L"🧝‍♂️", L"🧛‍♀️", L"🧛‍♂️", L"🧟‍♀️", L"🧟‍♂️", L"🧞‍♀️", L"🧞‍♂️", L"🧜‍♀️", L"🧜‍♂️", L"🧚‍♀️", L"🧚‍♂️", L"👼", L"🤰", L"🤱", L"🙇‍♀️", L"🙇‍♂️", L"💁‍♀️", L"💁‍♂️", L"🙅‍♀️", L"🙅‍♂️", L"🙆‍♀️", L"🙆‍♂️", L"🙋‍♀️", L"🙋‍♂️", L"🤦‍♀️", L"🤦‍♂️", L"🤷‍♀️", L"🤷‍♂️", L"🙎‍♀️", L"🙎‍♂️", L"🙍‍♀️", L"🙍‍♂️", L"💇‍♀️", L"💇‍♂️", L"💆‍♀️", L"💆‍♂️", L"🧖‍♀️", L"🧖‍♂️", L"💅", L"🤳", L"💃", L"🕺", L"👯‍♀️", L"👯‍♂️", L"🕴", L"🚶‍♀️", L"🚶‍♂️", L"🏃‍♀️", L"🏃‍♂️", L"👫", L"👭", L"👬", L"💑", L"👩‍❤️‍👩", L"👨‍❤️‍👨", L"💏", L"👩‍❤️‍💋‍👩", L"👨‍❤️‍💋‍👨", L"👪", L"👨‍👩‍👧", L"👨‍👩‍👧‍👦", L"👨‍👩‍👦‍👦", L"👨‍👩‍👧‍👧", L"👩‍👩‍👦", L"👩‍👩‍👧", L"👩‍👩‍👧‍👦", L"👩‍👩‍👦‍👦", L"👩‍👩‍👧‍👧", L"👨‍👨‍👦", L"👨‍👨‍👧", L"👨‍👨‍👧‍👦", L"👨‍👨‍👦‍👦", L"👨‍👨‍👧‍👧", L"👩‍👦", L"👩‍👧", L"👩‍👧‍👦", L"👩‍👦‍👦", L"👩‍👧‍👧", L"👨‍👦", L"👨‍👧", L"👨‍👧‍👦", L"👨‍👦‍👦", L"👨‍👧‍👧" };
		list_icon = tmp;
	}

	for (int i = 0; i < list_icon.size(); ++i) {
		list_icon_ctrl.InsertItem(i, list_icon[i].c_str());
	}
}




BEGIN_MESSAGE_MAP(ShowEmoijDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ICON, &ShowEmoijDlg::OnNMClickListIcon)
END_MESSAGE_MAP()


// ShowEmoijDlg message handlers


void ShowEmoijDlg::OnNMClickListIcon(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString content;
	mess->GetWindowTextW(content);
	CString icon = list_icon_ctrl.GetItemText(pNMItemActivate->iItem, 0);
	content += icon;
	recent.insert(recent.begin(), icon);
	mess->SetWindowTextW(content);
	*pResult = 0;
}
