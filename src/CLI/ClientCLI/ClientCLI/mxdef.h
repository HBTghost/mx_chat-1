#pragma once
#define PACKAGE_SIZE 4096
#define PACKAGE_HEADER_SIZE 256
#define PACKAGE_DATA_SIZE  (PACKAGE_SIZE - PACKAGE_HEADER_SIZE)
#define __DEBUG_CLIENT_LOGIN_FILE "debug_client_login_request.bin"
#define _AFXDLL
#include <afxwin.h>
#include <WinSock2.h>
#include <iostream>
#include <string>
#include "Logger.h"
#include "Account.h"
using namespace std;
using namespace CPlusPlusLogging;

enum EMessageCommand {
	CLIENT_SIGN_IN = 1,
	SERVER_RESPONSE_SIGN_IN_SUCCESS,
	SERVER_RESPONSE_SIGN_IN_ERROR,

	CLIENT_REQUEST_PRIVATE_CHAT,
	SERVER_RESPONSE_HASH_KEY,
	CLIENT_SEND_PRIVATE_CHAT

};

class SClientPacket {
public:
	Account* account;
	SOCKET client;
	string username;
	string hash_username;
};