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

enum EMessageCommand {
	SIGN_UP =5,
	SIGN_IN =8
};

#define ID_CLIENT_CONNECTED_MESSAGE				WM_USER+1
#define ID_CLIENT_DISCONNECTED_MESSAGE			WM_USER+2
#define ID_CLIENT_SIGNIN_MESSAGE				WM_USER+3
#define ID_CLIENT_SIGNUP_MESSAGE				WM_USER+4
#define ID_CLIENT_LOGOUT_MESSAGE				WM_USER+5
