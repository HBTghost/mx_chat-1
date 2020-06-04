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
		wcout << "WSAStartup failed: " << iResult << endl;
		return;
	}
	else {
		wcout << "The Winsock dll found!" << endl;
		wcout << "The status: " << wsaData.szSystemStatus << endl;
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

int ClientSocket::SendMessageServer( WCHAR* message, int len = 4096)
{
	int iStat = 0;

	iStat = send(_connect, (char*)message, len * 2 , 0);
	if (iStat == -1)
		return 1;

	return 0;
}


int ClientSocket::RecvMessageServer()
{
	WCHAR* message;
	WCHAR temp[4096];
	int iStat;
	int len;
	iStat = recv(_connect, (char*)temp, 4096, 0);

	if (iStat == -1)
	{
		wcout << "Has + 1 client disconnected" << endl;

		//this->getCWND()->SendMessage(ID_CLIENT_DISCONNECTED_MESSAGE, (WPARAM)*itl);
		return 1;
	}
	else {
		message = temp;
		this->ProcessMessage(temp);
	}

	
	//this->getCWND()->SendMessage(ID_CLIENT_RECEIVE_MESSAGE, (WPARAM)acRetData);
	//sendmsg receive
	//SendMessage(_hwnd, WM_COMMAND, (WPARAM)IDC_RECEIVE, (LPARAM)acRetData);

	return 0;
}

bool ClientSocket::IsConnected()
{
	return _isConnected;
}

void ClientSocket::ProcessMessage(WCHAR* temp) {
	MessageModel msg = PackageHelper::ParseMessage(temp, 4096);
	//process message here overide it
	EMessageCommand command = msg.command;
	switch (command)
	{
	case CLIENT_SIGN_UP:
		//ONLY SERVER
		break;
	case SERVER_SIGN_UP_ERROR_USER:
		wcout << "[SERVER RESPONSE] REGISTER \033[1;31m ERROR \033[0m , USERNAME EXISTED !!!" << endl;
		break;
	case SERVER_SIGN_UP_SUCCESS:
		wcout << "[SERVER RESPONSE] REGISTER \033[1;32m SUCCESSFULLY \033[0m !!!" << endl;
		break;
	case CLIENT_SIGN_IN:
		//ONLY SERVER
		break;
	case SERVER_SIGN_IN_ERROR_PASS:
		wcout << "[SERVER RESPONSE] LOGIN PASS \033[1;31m ERROR \033[0m , CHECK PASS !!!" << endl;
		break;
	case SERVER_SIGN_IN_SUCCESS:
		wcout << "[SERVER RESPONSE] LOGIN \033[1;32m SUCCESSFULLY \033[0m !!!" << endl;
		//trigger data to form here
		break;
	case NOTIFY_LIST_USER_ONLINE:
		_listUserOnline = msg.arg;
		wcout << " ======= USER ONLINE UPDATE ========== " << endl;
		//wcout << GetListUserOnlineStr() << endl;
		wcout << " ======= *********** ========== " << endl;
		break;
	case CLIENT_PRIVATE_MSG:
		wcout << "> Receive message from " << msg.arg[0] << endl;
		wcout << ">\t Content " << msg.arg[1] << endl;
		break;
	case CLIENT_GROUP_MSG:
		break;
	case CLIENT_REQUEST_TRANSFER_FILE:
		break;
	case CLIENT_BEGIN_TRANSFER_FILE:
	{
		ftm.AddChunk(msg, temp);
		break;
	}
	case CLIENT_END_TRANSFER_FILE:
		break;
	default:
		break;
	}
}
