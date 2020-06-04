#pragma once
#include <windows.h>
#include "ClientSocket.h"

class ClientBackgroundService
{
public:
	int InitClient();
	friend UINT recMessageThread(LPVOID pParam);

	void CreateWorkerThread();
	void TestLogin(wstring user, wstring pass);
	void TestSignUp(wstring user, wstring pass);


	wstring TestOnlineList() {
		return gClientObj.GetListUserOnlineStr();
	}
	void TestTransferFile(wstring user); 
	void TestPrivateMessage(wstring user, wstring msg);
	void TestGroupMessage(wstring group_id, wstring msg);
private:
	ClientSocket gClientObj;
	
	void SomeFunction();
};