#pragma once
#include "mxdef.h"
#include <iostream>
#include <list>
#include "SDataPacket.h"
#include "DebugHelper.h"
using namespace std;
class ServerSocket
{
public:
	ServerSocket() {
		this->InitServer();
	}
	void InitServer() {
		cout << "Starting TCP Socket server " << endl;
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
	~ServerSocket() {
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
	void BeginListenClient();
	int ReceivePackageClient(SOCKET recvSocket)
	{
		char* message;
		char temp[PACKAGE_SIZE]{ 0 };
		int iStat;
		int len_remain;

		iStat = recv(recvSocket, (char*)temp, PACKAGE_SIZE, 0);
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
			DWORD err = GetLastError();
			cout << "[CLIENT DISCONNECT OR ERROR] error connection " << err << endl;
			_listClient.remove(*itl);
			return 1;
		}
		else if (iStat == 0) {
			DWORD err = GetLastError();
			cout << "Disconnected " << endl;
			_listClient.remove(*itl);
			return 1;
		}
		else {
			message = temp;
			this->ProcessMessage(temp, itl);
			cout << "Processing message" << endl;
		}
		return 0;
	}
	void ProcessMessage(char* temp, list<SClientPacket*>::iterator& c_socket) {
		SDataPackage model(temp);
		EMessageCommand command = (EMessageCommand)model.command;
		model.DebugPackage();
		switch (command)
		{
		case CLIENT_SIGN_IN:
			LOG_INFO("SIGN IN REQUEST");
			cout << "CLIENT request sign in" << endl;
			break;
		default:
			break;
		}
	}
	int SendMessagePackage(SClientPacket* packet, char* msg, int len) {
		int iStat = 0;

		iStat = send(packet->client, (char*)msg, len, 0);
		if (iStat == -1){
			_listClient.remove(packet);
			LOG_ERROR("SendMessagePackage() - Client disconnected error");
			return -1;
		}
		if (iStat == 0){
			LOG_ERROR("SendMessagePackage() - Client disconnected");
			return 1;
		}
		return 0;
	}
	bool IsConnected()
	{
		return _isConnected;
	}
private:
	bool _isConnected = false;
	list<SClientPacket*> _listClient;
	SOCKET _socketClient;
	SOCKET _socketListenClient;
};