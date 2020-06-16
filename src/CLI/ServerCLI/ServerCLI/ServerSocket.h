#pragma once
#include "mxdef.h"
#include <iostream>
#include <list>
#include <map>
#include "SDataPacket.h"
#include "DebugHelper.h"
#include "Conversation.h"
#include "PrivateConversation.h"

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
		case CLIENT_SIGN_IN: {
			LOG_INFO("SIGN IN REQUEST");
			string user = model._data_items[0];
			string pass = model._data_items[1];
			cout << "CLIENT request sign in " << user << " pass " << pass << endl;
			(*c_socket)->account = new Account(user, pass);
			(*c_socket)->username = user;
			break;
		}
		case CLIENT_REQUEST_PRIVATE_CHAT: {
			InitRequestPrivateChat(&model, c_socket);
			break;
		}
		case CLIENT_SEND_PRIVATE_CHAT: {
			TransferPrivateChatMessage(&model, c_socket);
			break;
		}
		default:
			break;
		}
	}

	// manage conservation

	// 
#pragma region Feature Support Function
	list<SClientPacket*>::iterator FindClientByUsername(string username) {
		list<SClientPacket*>::iterator itl;
		for (itl = _listClient.begin(); itl != _listClient.end(); itl++)
		{
			if ((*itl)->username == username)
			{
				break;
			}
		}
		return itl;
	}
	void InitRequestPrivateChat(SDataPackage* package, PSIClientPacket srcClient) {
		PSIClientPacket desClient = this->FindClientByUsername(package->_data_items[0]);

		PrivateConversation* pcon = new PrivateConversation();
		pcon->_list_client.push_back(*srcClient);
		pcon->_list_client.push_back(*desClient);

		char* msg = pcon->BuildNewHashMsg();
		for (SClientPacket*& p_client : pcon->_list_client) {
			this->SendMessagePackage(p_client, msg, PACKAGE_SIZE);
		}
		//insert to list
		_lcs.insert(pair<string, Conversation*>(pcon->id_hash, pcon));
		LOG_INFO("InitRequestPrivateChat() : Sent ");
	}
	void TransferPrivateChatMessage(SDataPackage* package, PSIClientPacket srcClient) {
		//find conversation
		//sha des is package conversation
		string hash_key_con = package->GetSHA256Des();
		Conversation* pcon = _lcs[hash_key_con];
		if (pcon != nullptr) {
			char* msg = package->data();
			for (SClientPacket*& p_client : pcon->_list_client) {
				if (p_client != *srcClient) {
					this->SendMessagePackage(p_client, msg, PACKAGE_SIZE);
					LOG_INFO("TransferPrivateChatMessage() : + 1 Sent to other clients ");
				}
			}
		}
		else {
			LOG_ERROR("TransferPrivateChatMessage() : Cannot find conversation ");
		}
	}


#pragma endregion






	int SendMessagePackage(SClientPacket* packet, char* msg, int len) { 
		int iStat = 0;

		iStat = send(packet->client, (char*)msg, len, 0);
		if (iStat == -1) {
			_listClient.remove(packet);
			LOG_ERROR("SendMessagePackage() - Client disconnected error");
			return -1;
		}
		if (iStat == 0) {
			LOG_ERROR("SendMessagePackage() - Client disconnected");
			return 1;
		}
		return 0;
	}

	bool IsConnected()
	{
		return _isConnected;
	}
	list<SClientPacket*>& GetListClient() {
		return _listClient;
	}

private:

	map<string, Conversation*> _lcs;


	bool _isConnected = false;
	list<SClientPacket*> _listClient;

	SOCKET _socketClient;
	SOCKET _socketListenClient;
};