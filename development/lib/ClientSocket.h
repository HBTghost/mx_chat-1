#pragma once

#include <Afxwin.h>
#include <iostream>
#include <xstring>
#include <winsock2.h>
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
	virtual void ProcessMessage(char* msg);
	 


private:
	SOCKET _connect;
	wstring _serverIPAddress;
	int _serverPort;
	bool _isConnected;
	wstring _username;
	wstring _password;
};