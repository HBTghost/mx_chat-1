#pragma once
#include <Afxwin.h>
#include "mxdef.h"
#include <iostream>
#include <xstring>
#include <winsock2.h>
#include "SDataPacket.h"
#include "ClientConversation.h"
#include <string>
#include "resource.h"
using namespace std;
class ClientSocket
{
private:
	SOCKET _connect;
	string _serverIPAddress = "127.0.0.1";
	int _serverPort = 8084;
	bool _isConnected = false;


	bool IsLogined = false;

public:

	HWND hwnd;
	string username;
	bool task_sync = false;
	vector<string> _list_online;


	vector<ClientConversation*> _list_conversation;
	ClientSocket() {

	}
	void AddHwnd(HWND hwnd) {
		if (hwnd != NULL) {
			this->hwnd = hwnd;
		}
	}
	~ClientSocket() {
		if (_isConnected) {
			_isConnected = false;
			closesocket(_connect);
		}
	}
	void CloseConnection() {
		if (_isConnected)
		{
			_isConnected = false;
			closesocket(_connect);
		}
	}

	void Shutdown() {
		if (_isConnected) {
			_isConnected = false;
			shutdown(_connect, 2);
		}
	}

	bool IsConnected() {
		return _isConnected;
	}
	void InitClient(string ip_address = "127.0.0.1", int port = 8084) {

		this->_serverIPAddress = (ip_address);
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

	int SendMessagePackage(char* message, int len = PACKAGE_SIZE) {
		int iStat = 0;
		iStat = send(_connect, (char*)message, len, 0);
		if (iStat == -1)
			return 1;
		return 0;
	}
	int RecvMessage() {
		char* message;
		char temp[PACKAGE_SIZE];
		int iStat;
		iStat = recv(_connect, (char*)temp, PACKAGE_SIZE, 0);
		if (iStat == -1) {
			DWORD err = GetLastError();
			cout << "error connection " << err << endl;
			return -1;
		}
		else if (iStat == 0) {
			cout << "Server disconnected" << endl;
			return 1;
		}
		else {
			message = temp;
			this->ProcessMessage(temp);
		}

		return 0;
	}

	
	void ProcessMessage(char* messsage) {
		SDataPackage model(messsage);
		EMessageCommand command = (EMessageCommand)model.command;
		switch (command)
		{
		case SERVER_RESPONSE_SIGN_IN_SUCCESS:
		{
			//login success even
			cout << "Login successfully!!!" << endl;
			this->username = model._data_items[0];
			SendMessageW(hwnd, IDC_FORM_LOGIN_MSG_HANDLER, IDC_FORM_LOGIN_MSG_HANDLER_LOGIN_SUCESS, 0);
			break;
		}
		case SERVER_RESPONSE_SIGN_IN_ERROR:
			cout << "Login error, please check again!!!" << endl;
			SendMessageW(hwnd, IDC_FORM_LOGIN_MSG_HANDLER, IDC_FORM_LOGIN_MSG_HANDLER_LOGIN_ERROR, 0);
			break;
		case SERVER_RESPONSE_SIGN_UP_SUCCESS:
		{
			this->username = model._data_items[0];
			SendMessageW(hwnd, IDC_FORM_LOGIN_MSG_HANDLER, IDC_FORM_LOGIN_MSG_HANDLER_REGISTER_SUCCESS, 0);
		}
		break;
		case SERVER_RESPONSE_SIGN_UP_ERROR:
			SendMessageW(hwnd, IDC_FORM_LOGIN_MSG_HANDLER, IDC_FORM_LOGIN_MSG_HANDLER_REGISTER_ERROR, 0);
			break;
		case CLIENT_REQUEST_LIST_ONLINE:
			this->_list_online = model._data_items;
			SendMessageW(hwnd, IDC_FORM_CHAT_MSG_HANDLER, IDC_FORM_CHAT_MSG_HANDLER_LIST_ONLINE, 0);
			//MessageBoxW(hwnd, _T("Client list received "), _T("Alert"), MB_ICONERROR);
			break;
		case SERVER_RESPONSE_HASH_KEY:
			//send message to window to init and open new dialog chat
			//|hash|usr0|usr1
			//model.DebugPackage();
			LOG_INFO("Response hash key");
			LOG_INFO("Hash : " + model._data_items[0]);
			LOG_INFO("User src: " + model._data_items[1]);
			LOG_INFO("User des: " + model._data_items[2]);
			SendMessageW(hwnd, IDC_FORM_CHAT_MSG_HANDLER, IDC_FORM_CHAT_MSG_HANDLER_HASH_CONVERSATION,  (LPARAM) &model);
			cout << "Response hash key" << endl;
			break;
		case CLIENT_SEND_GROUP_CHAT:
			SendMessageW(hwnd, IDC_FORM_CHAT_MSG_HANDLER, IDC_FORM_CHAT_MSG_HANDLER_RECEIVE_CONVERSATION, (LPARAM)&model);

			LOG_INFO("Response group chat");
			//model.DebugPackage();
			cout << "Receive message client" << endl;
			break;
		case CLIENT_SEND_PRIVATE_CHAT:
			SendMessageW(hwnd, IDC_FORM_CHAT_MSG_HANDLER, IDC_FORM_CHAT_MSG_HANDLER_RECEIVE_CONVERSATION, (LPARAM)&model);

			LOG_INFO("Response private chat");
			LOG_INFO("MSG: " + model._data_items[0]);
			//model.DebugPackage();
			cout << "Receive message client" << endl;
			break;

		default:
			break;
		}
	}

};

