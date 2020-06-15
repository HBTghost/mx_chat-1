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
			MessageBox(0, L"\nFailed to initialise server socket.", 0, 0);
			return 0;
		}

		//AfxBeginThread(recMessageThread, this);

		return 1;
	}
	friend UINT recMessageThread(LPVOID pParam);
	void CreateWorkerThread();
	 
	void Login(string username, string password) {
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_SIGN_IN)
			->SetHeaderDesSrc("client", "server")
			->SetHeaderNumPackage(2)
			->SetHeaderTotalSize(4096)
			->EnableHashRawDesSrc();
		sdata->_data_items.push_back(username);
		sdata->_data_items.push_back(password);
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
		DebugHelper::DumpArray(__DEBUG_CLIENT_LOGIN_FILE, data, PACKAGE_SIZE);
	}
	void CloseConnection() {
		gClientObj.CloseConnection();
	}
	void Shutdown() {
		gClientObj.Shutdown();
	}
	
private:
	ClientSocket gClientObj;
	
};