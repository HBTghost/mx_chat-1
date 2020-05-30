#pragma once
#include <afxwin.h>
#include <WinSock2.h>
#include <list>
#include <vector>
#include <iostream>
using namespace std;

struct SClientPacket {
	SOCKET sClient;
	wstring username;
};

#define ID_CLIENT_CONNECTED_MESSAGE		WM_USER+1