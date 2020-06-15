#pragma once
#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include "ServerBackgroundService.h"
#include "Logger.h"
using namespace std;
using namespace CPlusPlusLogging;



int initServer() {
	char buf[4096];
	if (!gServerObj.IsConnected())
	{
		cout << "Failed to initialise server socket." << endl;
		LOG_INFO("Failed to init server socket");
		//MessageBox(0, L"\nFailed to initialise server socket.", 0, 0);4
		return 0;
	}
	AfxBeginThread(listenServerThread, 0);
	return 1;
}

int main()
{
	Logger* pLogger = NULL; // Create the object pointer for Logger Class
	pLogger = Logger::getInstance();
	pLogger->updateLogType(LOG_TYPE::BOTH_LOG);
	std::cout << "Hello World server!\r\n";

	initServer();
	while (true);
	return 0;
}

