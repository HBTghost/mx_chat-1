#pragma once
#include "stdafx.h"
#include <iostream>
#include "ServerBackgroundService.h"

#include "PackageHelper.h"
#include "MxChatConfigure.h"

MxChatConfigure config;
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

int main()
{
	std::cout << "Hello World server!\r\n";
	initServer();
	while (true);
	return 0;
}
