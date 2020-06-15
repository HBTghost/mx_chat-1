#pragma once
#define __DEBUG_CLIENT_LOGIN_FILE "debug_c_login_file.bin"
#define PACKAGE_SIZE 4096
#define PACKAGE_HEADER_SIZE 256
#define PACKAGE_DATA_SIZE  (PACKAGE_SIZE - PACKAGE_HEADER_SIZE)
#define _AFXDLL
#include <afxwin.h>
#include <WinSock2.h>
#include <iostream>
#include <string>
using namespace std;


enum EMessageCommand {
	CLIENT_SIGN_IN = 0,
	CLIENT_SIGN_UP = 1,
};

class SClientPacket {
public:
	SOCKET client;
	string username;
	string hash_username;
};