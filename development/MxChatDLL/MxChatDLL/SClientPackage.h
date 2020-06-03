#pragma once
#include <afxwin.h>
#include <WinSock2.h>
#include <list>
#include <vector>
#include <iostream>
#include "Account.h"
using namespace std;

class SClientPacket {
public:
	SClientPacket();
	~SClientPacket();
public:
	Account *account;
	SOCKET client;
	wstring username;
};