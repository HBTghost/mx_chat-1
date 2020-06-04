#include "stdafx.h"

#include "ServerSocket.h"
#include "ServerBackgroundService.h"
#include "PackageHelper.h"
#include "Account.h"

ServerSocket::ServerSocket() : _isConnected(false)
{
	wcout << "Starting TCP Socket Server " << endl;

	//initialize winsock https://docs.microsoft.com/en-us/windows/win32/winsock/initializing-winsock

	WSAData wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wcout << "WSAStartup failed: " << iResult << endl;
		return;
	}
	else {
		wcout << "The Winsock dll found!" << endl;
		wcout << "The status: " << wsaData.szSystemStatus << endl;
	}

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons((u_short)8084);
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;

	this->_socketListenClient = socket(AF_INET, SOCK_STREAM, 0);

	if (_socketListenClient == INVALID_SOCKET) {
		return;
	}
	if (::bind(_socketListenClient, (sockaddr*)&server_addr, sizeof(server_addr)) != 0)
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
	SClientPacket* recvPackage = new SClientPacket();
	recvPackage->client = _socketClient;

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

	iStat = send(packet->client, (char*)msg, len * 2 + 2, 0);
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
	iStat = recv(recvSocket, (char*)temp, 4096*2, 0);
	list<SClientPacket*>::iterator itl;
	for (itl = _listClient.begin(); itl != _listClient.end(); itl++)
	{
		if ((*itl)->client == recvSocket)
		{
			break;
		}
	}
	if (iStat == -1)
	{
		wcout << "Has + 1 client disconnected" << endl;
		_listClient.remove(*itl);
		//this->getCWND()->SendMessage(ID_CLIENT_DISCONNECTED_MESSAGE, (WPARAM)*itl);
		return 1;
	}
	else {
		mtx.lock();
		this->_total_msg++;
		message = temp;
		
		MessageModel model = PackageHelper::ParseMessage(temp, 4096);
		this->ProcessMessage(model, itl);
		//this->OnMessageEventHandler(model);
		mtx.unlock();
	}
	return 0;

}

void ServerSocket::NotifyListUserOnline() {
	MessageModel res_msg;
	res_msg.command = EMessageCommand::NOTIFY_LIST_USER_ONLINE;
	res_msg.arg = this->ListUserOnline();
	wstring build_msg = res_msg.BuildMessage();
	WCHAR* pp = StringHelper::wstringToWcharP(build_msg);
	this->SendPackageClientAll(pp, build_msg.size());
}
void ServerSocket::ProcessMessage(MessageModel& model, list<SClientPacket*>::iterator &c_socket)
{
	EMessageCommand command = model.command;
	
	switch (command)	
	{
	case CLIENT_SIGN_UP:
		break;
	case CLIENT_SIGN_IN:{
		wstring username;
		wstring password;
		username = model.arg[0];
		password = model.arg[1];
		wstring server_log;
		MessageModel res_msg;
		//Account* acc_su = new Account(username, password);

		bool condition = StringHelper::wstringCompare(username, L"admin") && StringHelper::wstringCompare(password, L"pass");
		condition = condition || ( StringHelper::wstringCompare(username, L"mod") && StringHelper::wstringCompare(password, L"pass"));
		if (condition) {
			wcout << "[LOGIN] SUCCESS -  USERNAME " << username << " | PASS " << password << endl;
			res_msg.command = EMessageCommand::SERVER_SIGN_IN_SUCCESS;
			(*c_socket)->account = new Account(username, password);
			this->NotifyListUserOnline();
		}
		else {
			res_msg.command = EMessageCommand::SERVER_SIGN_IN_ERROR_PASS;
			wcout << "[LOGIN] FAIL -  USERNAME " << username << " | PASS " << password << endl;
		}
		res_msg.num_package = 0;
		res_msg.arg = this->ListUserOnline();
		wstring build_msg = res_msg.BuildMessage();
		WCHAR* pp = StringHelper::wstringToWcharP(build_msg);
		this->SendPackageClient(*c_socket, pp, build_msg.size());
		break;
	}
	case SERVER_SIGN_IN_ERROR_PASS:
		break;
	case SERVER_SIGN_IN_SUCCESS:
		break;
	case CLIENT_PRIVATE_MSG:
	{
		SClientPacket* desSocket = this->GetClientByUsername(model.arg[0]);
		if (desSocket == nullptr) {
			wcout << "[PRIVATE_MSG] DES NOT FOUND" << endl;
		}
		else{
			model.arg[0] = (*c_socket)->account->getUserName();
			wstring build_msg = model.BuildMessage();
			WCHAR* pp = StringHelper::wstringToWcharP(build_msg);

			this->SendPackageClient(desSocket, pp, build_msg.size());
		}
		break; 
	}
	case CLIENT_GROUP_MSG:
		
		break;
	case CLIENT_REQUEST_TRANSFER_FILE:
		
		break;
	case CLIENT_BEGIN_TRANSFER_FILE:
	{
		// | Comand | Total size | current pack | id | msg |
		//
		//this->TransferFile();
		//SClientPacket* desSocket = this->GetClientByUsername(L"admin");
		//if (desSocket == nullptr) {
		//	wcout << "[PRIVATE_MSG] DES NOT FOUND" << endl;
		//}
		//else{
			wstring build_msg = model.BuildMessage();
			WCHAR* pp = StringHelper::wstringToWcharP(build_msg);
			this->SendPackageClient(*c_socket, pp, 4096 );
		//}
		break;
	}
	case CLIENT_END_TRANSFER_FILE:
		break;
	default:
		break;
	}
}

void ServerSocket::TransferFile() {

}
SClientPacket* ServerSocket::GetClientByUsername(wstring username)
{
	vector<wstring> listOnline;
	list<SClientPacket*>::iterator itl;
	for (itl = _listClient.begin(); itl != _listClient.end(); itl++)
	{
		if (!(*itl)->account) {
			continue;
		}
		if (StringHelper::wstringCompare((*itl)->account->getUserName(), username))
		{
			return *itl;
		}
	}
	return nullptr;
}

vector<wstring> ServerSocket::ListUserOnline()
{
	vector<wstring> listOnline;
	list<SClientPacket*>::iterator itl;
	for (itl = _listClient.begin(); itl != _listClient.end(); itl++)
	{
		if (!(*itl)->account) {
			continue; 
		}
		if ((*itl)->account->getUserName().c_str() != L"")
		{
			listOnline.push_back((*itl)->account->getUserName());
		}
	}
	return listOnline;
}
