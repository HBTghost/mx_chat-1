#pragma once
#include <windows.h>
#include "mxdef.h"
#include "ClientSocket.h"
#include <chrono>
#include <mutex>
#include <thread>
#include "DebugHelper.h"


class ClientBackgroundService
{
public:
	int InitClient()
	{

		gClientObj.InitClient("127.0.0.1", 8084);
		if (!gClientObj.IsConnected())
		{
			MessageBox(0, L"\nFailed to initialise Client socket.", 0, 0);
			return 0;
		}

		//AfxBeginThread(recMessageThread, this);

		return 1;
	}
	void AddHwnd(HWND hwnd) {
		this->gClientObj.AddHwnd(hwnd);
	}
	friend UINT recMessageThread(LPVOID pParam);
	void CreateWorkerThread();
	 
	void SendPrivateMessage(string hash_des, string message) {
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_SEND_PRIVATE_CHAT)
			->SetHeaderDesSrc(username, "server")
			->SetHeaderDesSrcHash(SHA256()(username), hash_des)
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(message);
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
	}
	void OpenDialogSession(SDataPackage* package) {
		string hash_conservation_id = package->_data_items[0];
		bool chatOn = true;
		string message = "";

		while (chatOn == true) {
			cout << username << ">";
			getline(cin, message);
			SendPrivateMessage(hash_conservation_id, message);
			if (message.compare("exit") == 0) {
				chatOn = false;
			}
		}
	}
	
	void Login(string username, string password) {
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_SIGN_IN)
			->SetHeaderDesSrc("client", "server")
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(username);
		sdata->_data_items.push_back(password);
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
		this->username = username;
		LOG_INFO("Login() send ");
		DebugHelper::DumpArray(__DEBUG_CLIENT_LOGIN_FILE, data, PACKAGE_SIZE);
	}

	void Register(string username, string password) {
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_SIGN_UP)
			->SetHeaderDesSrc("client", "server")
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(username);
		sdata->_data_items.push_back(password);
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
		this->username = username;
		LOG_INFO("Register() send ");
		DebugHelper::DumpArray(__DEBUG_CLIENT_LOGIN_FILE, data, PACKAGE_SIZE);
	}


	void CreatePrivateConversation(string des_username) {
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_REQUEST_PRIVATE_CHAT)
			->SetHeaderDesSrc("client", "server")
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(des_username);
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
		LOG_INFO("CreatePrivateConversation9) : Sent request "); 
		//DebugHelper::DumpArray(__DEBUG_CLIENT_LOGIN_FILE, data, PACKAGE_SIZE);
	
	}
	
	void CloseConnection() {
		gClientObj.CloseConnection();
	}
	void Shutdown() {
		gClientObj.Shutdown();
	}
	string username = "";

	ClientSocket gClientObj;

private:
};