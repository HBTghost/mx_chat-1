#include "pch.h"
#include "resource.h"
#include "ServerSocket.h"
#include "ServerBackgroundService.h"

ServerSocket::ServerSocket() : _isConnected(false)
{
	cout << "Starting TCP Socket Server " << endl;

	//initialize winsock https://docs.microsoft.com/en-us/windows/win32/winsock/initializing-winsock

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

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons((u_short)8084);
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;

	this->_socketListenClient = socket(AF_INET, SOCK_STREAM, 0);

	if (_socketListenClient == INVALID_SOCKET) {
		return;
	}
	if (bind(_socketListenClient, (sockaddr*)&server_addr, sizeof(server_addr)) != 0)
	{
		return;
	}
	if (listen(_socketListenClient, 10) != 0)
	{
		return;
	}
	_isConnected = true;
	return;
}

ServerSocket::~ServerSocket()
{
	closesocket(_socketListenClient);
	WSACleanup();
	if (_listClient.size() != 0)
	{
		for (auto& client : _listClient)
		{
			delete client;
		}
	}

}

bool ServerSocket::IsConnected()
{
	return _isConnected;
}

void ServerSocket::BeginListenClient()
{
	sockaddr_in from;
	int addr_from_len = sizeof(from);
	_socketClient = accept(_socketListenClient, (sockaddr*)&from, &addr_from_len);
	SClientPacket* recvPackage = new SClientPacket;
	recvPackage->sClient = _socketClient;

	HWND hwnd = _cWnd->GetSafeHwnd();
	if (_socketClient != INVALID_SOCKET) {
		_cWnd->SendMessage(ID_CLIENT_CONNECTED_MESSAGE, 0);
		//SendMessage(hwnd, WM_COMMAND, ID_CLIENT_CONNECTED_MESSAGE , 0);
		_listClient.push_back(recvPackage);
	}

	//regis thread server thread 
	AfxBeginThread(recServerThread, (void*)_socketClient);


}
int ServerSocket::SendPackageClient(SClientPacket* packet, WCHAR* msg, int len) {
	int iStat = 0;

	iStat = send(packet->sClient, (char*)msg, len * 2 + 2, 0);
	if (iStat == -1)
		_listClient.remove(packet);
	if (iStat == -1)
		return 1;
	return 0;
}

int ServerSocket::SendPackageClientAll(WCHAR* msg, int len)
{
	list<SClientPacket*>::iterator itl;
	for (itl = _listClient.begin(); itl != _listClient.end(); itl++)
	{
		this->SendPackageClient(*itl, msg, len);
	}
	return 0;
}


int ServerSocket::ReceivePackageClient(SOCKET recvSocket)
{
	WCHAR* message;
	WCHAR temp[4096];
	int iStat;
	int len;
	iStat = recv(recvSocket, (char*)temp, 4096, 0);
	list<SClientPacket*>::iterator itl;
	for (itl = _listClient.begin(); itl != _listClient.end(); itl++)
	{
		if ((*itl)->sClient == recvSocket)
		{
			break;
		}
	}
	if (iStat == -1)
	{
		this->getCWND()->SendMessage(ID_CLIENT_DISCONNECTED_MESSAGE, (WPARAM)*itl);
		//SendMessage(_hwnd, WM_COMMAND, ID_CLIENT_DISCONNECTED_MESSAGE,(LPARAM) *itl);
		return 1;
	}
	else {
		message = temp;
		switch (message[0]) {
			case EMessageCommand::SIGN_UP: {
				wstring username;
				wstring password;

				username = message + 1;
				password = message + wcsnlen_s(message,4096 - username.length()) + 1;
				Account* acc_su = new Account(username, password);

				this->getCWND()->SendMessage(ID_CLIENT_SIGNUP_MESSAGE, (WPARAM)acc_su);


				break;
			}
			case EMessageCommand::SIGN_IN: {
				
				break;
			}
		}
	}
	return 0;

}

void ServerSocket::setHWND(HWND hwnd)
{
	_hwnd = hwnd;
}
HWND ServerSocket::getHWND()
{
	HWND hwnd = _cWnd->GetSafeHwnd();
	return hwnd;
}
void ServerSocket::setCWND(CWnd*& cwnd)
{
	this->_cWnd = cwnd;
}
CWnd*& ServerSocket::getCWND()
{
	return this->_cWnd;
	// TODO: insert return statement here
}