#pragma once

#include <Afxwin.h>
#include <iostream>
#include <xstring>
#include <winsock2.h>
#include "FileTransferManagment.h"
using namespace std;

class ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();

	void Init(wstring ipAddress, int port);
	int SendMessageServer(WCHAR* message, int len);
	int RecvMessageServer();
	bool IsConnected();
	//void ProcessMessage(MessageModel& msg);
	void ProcessMessage(WCHAR* temp);
	wstring GetListUserOnlineStr() {
		wstring str_list = L"";
		for(auto item : _listUserOnline) {
			str_list += item; 
			str_list += L',';
		}
		if (!str_list.empty()) {
			str_list.pop_back();
		}
		return str_list;
	}
	 


private:
	SOCKET _connect;
	wstring _serverIPAddress;
	int _serverPort;
	bool _isConnected;
	wstring _username;
	wstring _password;
	vector<wstring> _listUserOnline;
	FileTransferManagment ftm;
};