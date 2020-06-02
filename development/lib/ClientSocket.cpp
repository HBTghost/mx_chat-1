#include "stdafx.h"
#include "ClientSocket.h"

ClientSocket::ClientSocket() : _isConnected(false), _serverPort(8084)
{
}

ClientSocket::~ClientSocket()
{
	if (_isConnected)
		closesocket(_connect);
}

void ClientSocket::Init(wstring ipAddress = _T("127.0.0.1"), int port = 8084)
{
	this->_serverIPAddress = wstring(ipAddress);
	_serverPort = port;

	WSAData wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "WSAStartup failed: " << iResult << endl;
		return;
	}
	else {
		cout << "The Winsock dll found!" << endl;
		cout << "The status: " << wsaData.szSystemStatus << endl;
	}

	struct hostent* hp;
	unsigned int addr;
	struct sockaddr_in server;

	_connect = socket(AF_INET, SOCK_STREAM, 0);
	if (_connect == INVALID_SOCKET)
		return;


	addr = inet_addr("127.0.0.1");
	hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);

	if (hp == NULL)
	{
		closesocket(_connect);
		return;
	}

	server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(_serverPort);
	if (connect(_connect, (struct sockaddr*)&server, sizeof(server)))
	{
		closesocket(_connect);
		return;
	}
	_isConnected = true;
	return;

}

int ClientSocket::SendMessageServer( WCHAR* message, int len)
{
	int iStat = 0;

	iStat = send(_connect, (char*)message, len * 2 + 2, 0);
	if (iStat == -1)
		return 1;

	return 0;
}


int ClientSocket::RecvMessageServer()
{
	char acRetData[4096];
	int iStat = 0;

	iStat = recv(_connect, acRetData, 4096, 0);
	if (iStat == -1)
		return 1;

	
	this->ProcessMessage(acRetData);

	//this->getCWND()->SendMessage(ID_CLIENT_RECEIVE_MESSAGE, (WPARAM)acRetData);
	//sendmsg receive
	//SendMessage(_hwnd, WM_COMMAND, (WPARAM)IDC_RECEIVE, (LPARAM)acRetData);

	return 0;
}

bool ClientSocket::IsConnected()
{
	return _isConnected;
}

void ClientSocket::ProcessMessage(char* msg) {
	//process message here overide it

}
