#pragma once
#include "stdafx.h"
#include <iostream>
#include "ClientBackgroundService.h"
#include <chrono>



using namespace std;

ClientBackgroundService client;
int main()
{
	std::cout << "Hello World Client!\n";
	client.InitClient();
	client.CreateWorkerThread();
	client.TestLogin(L"admin", L"pass");
//	Sleep(1000); // pauses for 10 seconds

	client.TestLogin(L"admin123", L"pass");
	//Sleep(1000); // pauses for 10 seconds

	client.TestLogin(L"admin1x", L"passdd");
	client.TestLogin(L"admin2x", L"passdd");
	client.TestLogin(L"admin3x", L"passdd");
	client.TestLogin(L"admin4x", L"passdd");
	client.TestLogin(L"admin5x", L"passdd");
	client.TestLogin(L"admin6x", L"passdd");
	client.TestLogin(L"admin7x", L"passdd");
	client.TestLogin(L"admin8x", L"passdd");

	while (true);
	return 0;
}
