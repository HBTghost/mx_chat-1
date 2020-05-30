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
	if(_listClient.size() != 0)
	{
		for (auto &client : _listClient)
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
	_socketClient = accept(_socketListenClient, (sockaddr*)&from, &addr_from_len );
	SClientPacket *recvPackage = new SClientPacket;
	recvPackage->sClient = _socketClient;
	
	HWND hwnd = _cWnd->GetSafeHwnd();
	if (_socketClient != INVALID_SOCKET) {
		_cWnd->SendMessage(ID_CLIENT_CONNECTED_MESSAGE, 12);
		//SendMessage(hwnd, WM_COMMAND, ID_CLIENT_CONNECTED_MESSAGE , 0);
		_listClient.push_back(recvPackage);
	}

	//regis thread server thread 
	AfxBeginThread(recServerThread, (void*)_socketClient);

	
}
void ServerSocket::SendPackageClient(SClientPacket* packet, WCHAR* msg, int len) {

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
void ServerSocket::setCWND(CWnd *&cwnd)
{
	this->_cWnd = cwnd;
}
CWnd*& ServerSocket::getCWND()
{
	return this->_cWnd;
	// TODO: insert return statement here
}
int ServerSocket::ReceivePackageClient(SOCKET recvSocket)
{
	return  0;
}
