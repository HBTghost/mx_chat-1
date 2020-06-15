// ClientCLI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include "ClientBackgroundService.h"
using namespace std;
ClientBackgroundService client;
int main(int argc, char** argv)
{
	std::cout << "Hello World Client!\n";
	client.InitClient();
	client.CreateWorkerThread();
	client.Login("admin", "pass");
	client.Shutdown();
	while (true);
	return 0;
}
