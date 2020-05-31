#pragma once
#include "stdafx.h"
#include <iostream>
#include "ClientBackgroundService.h"
#include "ServerBackgroundService.h"

#include "PackageHelper.h"

void ProcessPackage(MessageModel m) {
	cout << "RECV Event();" << endl;
}
int initServer() {
	char buf[4096];
	
	if (!gServerObj.IsConnected())
	{
		MessageBox(0, L"\nFailed to initialise server socket.", 0, 0);
		return 0;
	}
	gServerObj.addHandler(&ProcessPackage);
	AfxBeginThread(listenServerThread, 0);
	return 1;

}

int init() {
	
	gClientObj.Init(L"127.0.0.1", 8084);
	if (!gClientObj.IsConnected())
	{
		MessageBox(0, L"\nFailed to initialise server socket.", 0, 0);
		return 0;
	}

	AfxBeginThread(recMessageThread, 0);

	return 1;
}


int main()
{
	std::cout << "Hello World server!\r\n";
	initServer();
	while (true);
	return 0;
}
