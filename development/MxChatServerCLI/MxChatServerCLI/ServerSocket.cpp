#include "stdafx.h"

#include "ServerSocket.h"
#include "ServerSocket.h"
#include "ServerBackgroundService.h"
#include "PackageHelper.h"
#include "Account.h"

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
		wcout << "Has + 1 client connected" << endl;
		//_cWnd->SendMessage(ID_CLIENT_CONNECTED_MESSAGE, 0);
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
		wcout << "Has + 1 client disconnected" << endl;

		//this->getCWND()->SendMessage(ID_CLIENT_DISCONNECTED_MESSAGE, (WPARAM)*itl);
		return 1;
	}
	else {
		message = temp;

		MessageModel model = PackageHelper::ParseMessage(temp, 4096);
		
		this->ProcessMessage(model);
	}
	return 0;

}

void ServerSocket::ProcessMessage(MessageModel& model)
{

	switch (model.command) {

		case EMessageCommand::SIGN_IN: {
			wstring username;
			wstring password;

			username = model.arg[0];
			password = model.arg[1];
			//password = message + 2 + wcsnlen_s(username, 4096 - username.length()) + 1;
			Account* acc_su = new Account(username, password);
			wcout << "[LOGIN] USERNAME " << username << " | PASS " << password << endl;
			break;
		}

	}
}
