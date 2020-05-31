#pragma once
#include "stdafx.h"
#include <iostream>
#include "ClientBackgroundService.h"

#include "PackageHelper.h"

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
    std::cout << "Hello World Client!\n";
	init();

	MessageModel msg;
	msg.command = EMessageCommand::SIGN_IN;
	msg.arg.push_back(L"admin");
	msg.arg.push_back(L"pass");
	wstring build_msg = msg.BuildMessage();
	WCHAR packet[100];
	for (int i = 0; i < build_msg.size(); i++) {
		packet[i] = build_msg[i];
	}

	gClientObj.SendMessageServer(packet, build_msg.size());
	system("pause");
	return 0; 
}
