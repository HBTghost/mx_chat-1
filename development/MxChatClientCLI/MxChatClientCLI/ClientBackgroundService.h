#pragma once
#include <windows.h>
#include "ClientSocket.h"



/* MyClass.h */
class ClientBackgroundService
{
public:
	int InitClient();
	friend UINT recMessageThread(LPVOID pParam);

	void CreateWorkerThread();
	void TestLogin();
private:
	ClientSocket gClientObj;
	void SomeFunction();
};