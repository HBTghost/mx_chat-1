#pragma once
#include "mxdef.h"
#include <iostream>
#include <list>
#include <map>
#include "SDataPacket.h"
#include "DebugHelper.h"
#include "Conversation.h"
#include "PrivateConversation.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include "GroupConversation.h"
using namespace std;
class ServerSocket
{
public:
	AccountManagement accMa;


	ServerSocket() {
		this->InitServer();
	}
	void InitServer() {
		LOG_INFO("Starting TCP Socket server ");
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
			LOG_INFO("[CLIENT DISCONNECT OR ERROR] error connection");

			cout << "[CLIENT DISCONNECT OR ERROR] error connection " << err << endl;
			_listClient.remove(*itl);
			this->NotifyOnlineList();
			return 1;
		}
		else if (iStat == 0) {
			DWORD err = GetLastError();
			LOG_INFO("[CLIENT DISCONNECT ] CLIENT DISCONNECTED");

			cout << "Disconnected " << endl;
			_listClient.remove(*itl);
			this->NotifyOnlineList();
			return 1;
		}
		else {
			message = temp;
			this->ProcessMessage(temp, itl);
			cout << "Processing message" << endl;
			LOG_INFO("[CLIENT MESSAGE ] Processing Message");

		}
		return 0;
	}
	void ProcessMessage(char* temp, list<SClientPacket*>::iterator& c_socket) {
		SDataPackage model(temp);
		EMessageCommand command = (EMessageCommand)model.command;
		//model.DebugPackage();
		switch (command)
		{
		case CLIENT_SIGN_IN: {
			LOG_INFO("SIGN IN REQUEST");
			string user = model._data_items[0];
			string pass = model._data_items[1];
			cout << "CLIENT request sign in from User: [" << user << "] & Password: [" << pass << "]" << endl;
			LOG_INFO("CLIENT request sign in from User: [" + user + "] & Password: [" + pass + "]");

			SDataPackage* res_msg = (new SDataPackage())
				->SetHeaderDesSrc("server", "client")
				->SetHeaderNumPackage(0)
				->SetHeaderTotalSize(4096);;

			std::vector<string>::iterator it = std::find(_list_online.begin(), _list_online.end(), user);
			if (it != _list_online.end()) {
				//found
				LOG_INFO("Sign in [" + user + "] & Password: [" + pass + "] error session existed => Response");
				res_msg->SetHeaderCommand(EMessageCommand::SERVER_RESPONSE_SIGN_IN_ERROR);
			}
			else {
				int flag = accMa.CheckAccount(Account(user, pass));

				if (flag == RIGHT_PASSWORD) {
					res_msg->SetHeaderCommand(EMessageCommand::SERVER_RESPONSE_SIGN_IN_SUCCESS);
					(*c_socket)->account = new Account(user, pass);
					(*c_socket)->username = user;

					LOG_INFO("Sign in [" + user + "] & Password: [" + pass + "] success => Response");

				}
				else if (flag == WRONG_PASSWORD) {
					LOG_INFO("Sign in [" + user + "] & Password: [" + pass + "] error wrong pass => Response");
					res_msg->SetHeaderCommand(EMessageCommand::SERVER_RESPONSE_SIGN_IN_ERROR);
				}
				else {
					LOG_INFO("Sign in [" + user + "] & Password: [" + pass + "] error => Response");
					res_msg->SetHeaderCommand(EMessageCommand::SERVER_RESPONSE_SIGN_IN_ERROR);
				}

			}
			res_msg->_data_items.push_back(user);

			char* res_msg_buff = res_msg->BuildMessage();
			this->SendMessagePackage(*c_socket, res_msg_buff, PACKAGE_SIZE);
			//std::this_thread::sleep_for(std::chrono::seconds(3));
			this->NotifyOnlineList();
			break;
		}
		case CLIENT_SIGN_UP: {
			LOG_INFO("SIGN UP REQUEST");
			string user = model._data_items[0];
			string pass = model._data_items[1];
			cout << "CLIENT request sign up from User: [" << user << "] & Password: [" << pass << "]" << endl;
			LOG_INFO("CLIENT request sign up from User: [" + user + "] & Password: [" + pass + "]");

			SDataPackage* res_msg = (new SDataPackage())
				->SetHeaderDesSrc("server", "client")
				->SetHeaderNumPackage(0)
				->SetHeaderTotalSize(4096);;

			int flag = accMa.AddAccount(Account(user, pass));
			if (flag) {
				res_msg->SetHeaderCommand(EMessageCommand::SERVER_RESPONSE_SIGN_UP_SUCCESS);
				LOG_INFO("Sign up [" + user + "] & Password: [" + pass + "] success => Response");
			}
			else {
				res_msg->SetHeaderCommand(EMessageCommand::SERVER_RESPONSE_SIGN_UP_ERROR);
				LOG_INFO("Sign up [" + user + "] & Password: [" + pass + "] error => Response");
			}
			res_msg->_data_items.push_back(user);
			char* res_msg_buff = res_msg->BuildMessage();
			this->SendMessagePackage(*c_socket, res_msg_buff, PACKAGE_SIZE);
			break;
		}
		case CLIENT_REQUEST_LIST_ONLINE: {
			SDataPackage* res_msg = (new SDataPackage())
				->SetHeaderCommand(EMessageCommand::CLIENT_REQUEST_LIST_ONLINE)
				->SetHeaderDesSrc("server", "client")
				->SetHeaderNumPackage(0)
				->SetHeaderTotalSize(4096);
			res_msg->_data_items = this->GetListOnline();
			char* res_msg_buff = res_msg->BuildMessage();
			this->SendMessagePackage(*c_socket, res_msg_buff, PACKAGE_SIZE);
			break;
		}
		case CLIENT_REQUEST_PRIVATE_CHAT: {
			InitRequestPrivateChat(&model, c_socket);
			break;
		}
		case CLIENT_REQUEST_GROUP_CHAT: {
			InitRequestGroupChat(&model, c_socket);
			break;
		}
		case CLIENT_SEND_GROUP_CHAT: {
			TransferGroupChatMessage(&model, c_socket);
			break;
		}

		case CLIENT_SEND_PRIVATE_CHAT: {
			TransferPrivateChatMessage(&model, c_socket);
			break;
		}
		case CLIENT_REQUEST_TRANSFER_FILE:
		{
			InitRequestTransferFile(&model, c_socket);
			break;
		}
		case CLIENT_BEGIN_TRANSFER_FILE:
		{
			TransferFileMessage(&model, c_socket);
			break;
		}
		case CLIENT_END_TRANSFER_FILE:
		{
			EndFileMessage(&model, c_socket);
			break;
		}
		default:
			break;
		}
	}

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
	vector<SClientPacket*> FindClientOnlineClientPacket(vector<string> list_mem) {
		vector<SClientPacket*> list_client_on;
		for (string item : list_mem) {
			PSIClientPacket desClient = this->FindClientByUsername(item);
			list_client_on.push_back(*desClient);
		}
		return  list_client_on;
	}


	void InitRequestTransferFile(SDataPackage* package, PSIClientPacket srcClient) {
		string hash_key_con = package->GetSHA256Des();
		Conversation* pcon = _lcs[hash_key_con];
		if (pcon != nullptr) {
			
				char* msg = package->data();
				for (SClientPacket*& p_client : pcon->_list_client) {
					if (p_client != *srcClient) {
						this->SendMessagePackage(p_client, msg, PACKAGE_SIZE);
						LOG_INFO("InitRequestTransferFile() : + 1 Sent to other clients ");
					}
				}

				/*
				SDataPackage* transfer_stt_msg = (new SDataPackage())
					->SetHeaderCommand(EMessageCommand::SERVER_RESPONSE_TRANSFER_STATUS)
					->SetHeaderDesSrc("server", hash_key_con)
					->SetHeaderNumPackage(package->GetCurrentPacket())
					->SetHeaderTotalSize(PACKAGE_SIZE);
				transfer_stt_msg->_data_items.push_back("allow");
				char* res_msg_buff = transfer_stt_msg->BuildMessage();
				this->SendMessagePackage(*srcClient, res_msg_buff, PACKAGE_SIZE);
				*/
			
		}
		else {
			LOG_ERROR("InitRequestTransferFile() : Cannot find conversation ");
		}

	}

	void TransferFileMessage(SDataPackage* package, PSIClientPacket srcClient) {
		//find conversation
		//sha des is package conversation
		string hash_key_con = package->GetSHA256Des();
		Conversation* pcon = _lcs[hash_key_con];
		if (pcon != nullptr) {
			char* msg = package->data();
			for (SClientPacket*& p_client : pcon->_list_client) {
				if (p_client != *srcClient) {
					this->SendMessagePackage(p_client, msg, PACKAGE_SIZE);
					LOG_INFO("TransferFileMessage() : + 1 Sent to other clients ");
				}
			}
		}
		else {
			LOG_ERROR("TransferFileMessage() : Cannot find conversation ");
		}
	}
	void EndFileMessage(SDataPackage* package, PSIClientPacket srcClient) {
		//find conversation
		//sha des is package conversation
		string hash_key_con = package->GetSHA256Des();
		Conversation* pcon = _lcs[hash_key_con];
		if (pcon != nullptr) {
			char* msg = package->data();
			for (SClientPacket*& p_client : pcon->_list_client) {
				if (p_client != *srcClient) {
					this->SendMessagePackage(p_client, msg, PACKAGE_SIZE);
					LOG_INFO("TransferFileMessage() : + 1 Sent to other clients ");
				}
			}
		}
		else {
			LOG_ERROR("TransferFileMessage() : Cannot find conversation ");
		}
	}

	void InitRequestGroupChat(SDataPackage* package, PSIClientPacket srcClient) {
		vector<string> list_mem;
		copy(package->_data_items.begin() + 1, package->_data_items.end(), back_inserter(list_mem));

		vector<SClientPacket*> list_client_online = this->FindClientOnlineClientPacket(list_mem);
		GroupConversation* pcon = new GroupConversation(package->_data_items[0], list_mem);
		pcon->_list_client = list_client_online;

		char* msg = pcon->BuildNewHashMsg();

		for (SClientPacket*& p_client : pcon->_list_client) {
			this->SendMessagePackage(p_client, msg, PACKAGE_SIZE);
		}

		_lcs.insert(pair<string, Conversation*>(pcon->id_hash, pcon));
	}

	void TransferGroupChatMessage(SDataPackage* package, PSIClientPacket srcClient) {
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
				if (p_client->username != (*srcClient)->username) {
					this->SendMessagePackage(p_client, msg, PACKAGE_SIZE);
					LOG_INFO("TransferPrivateChatMessage() : + 1 Sent to other clients ");
				}
			}
		}
		else {
			LOG_ERROR("TransferPrivateChatMessage() : Cannot find conversation ");
		}
	}
	void NotifyOnlineList() {

		PSIClientPacket itl;

		SDataPackage* res_msg = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_REQUEST_LIST_ONLINE)
			->SetHeaderDesSrc("server", "client")
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		res_msg->_data_items = this->GetListOnline();
		char* res_msg_buff = res_msg->BuildMessage();


		for (itl = _listClient.begin(); itl != _listClient.end(); itl++)
		{
			if (!((*itl)->username).empty())
			{
				this->SendMessagePackage(*itl, res_msg_buff, PACKAGE_SIZE);
			}
		}
	}


#pragma endregion
	bool IsConnected()
	{
		return _isConnected;
	}
	list<SClientPacket*>& GetListClient() {
		return _listClient;
	}
	vector<string>& GetListOnline() {
		_list_online.clear();
		PSIClientPacket itl;
		for (itl = _listClient.begin(); itl != _listClient.end(); itl++)
		{
			if (!((*itl)->username).empty())
			{
				_list_online.push_back((*itl)->username);
			}
		}
		return _list_online;
	}

private:
	//string cache_list_online = "";
	vector<string> _list_online;
	map<string, Conversation*> _lcs;
	bool _isConnected = false;
	list<SClientPacket*> _listClient;

	SOCKET _socketClient;
	SOCKET _socketListenClient;
};