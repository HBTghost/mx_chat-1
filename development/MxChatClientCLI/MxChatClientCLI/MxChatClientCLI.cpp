#pragma once
#include "stdafx.h"
#include <iostream>
#include "ClientBackgroundService.h"
#include "StringHelper.h"

#include "PackageHelper.h"

ClientBackgroundService client;

int main()
{
	std::cout << "Hello World Client!\n";
	client.InitClient();
	client.CreateWorkerThread();
	client.TestLogin();


	system("pause");
	return 0;
}
