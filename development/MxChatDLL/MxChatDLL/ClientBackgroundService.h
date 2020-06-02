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
private:
	ClientSocket gClientObj;
	void SomeFunction();
};