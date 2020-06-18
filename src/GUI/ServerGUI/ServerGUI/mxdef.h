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
#include <list>
#include "Logger.h"
#include "Account.h"
using namespace std;
using namespace CPlusPlusLogging;


enum EMessageCommand {
	CLIENT_SIGN_IN = 1,
	SERVER_RESPONSE_SIGN_IN_SUCCESS,
	SERVER_RESPONSE_SIGN_IN_ERROR,

	CLIENT_SIGN_UP,
	SERVER_RESPONSE_SIGN_UP_SUCCESS,
	SERVER_RESPONSE_SIGN_UP_ERROR,


	CLIENT_REQUEST_PRIVATE_CHAT,
	SERVER_RESPONSE_HASH_KEY,//private or public
	CLIENT_SEND_PRIVATE_CHAT,



	CLIENT_REQUEST_LIST_ONLINE,

	CLIENT_REQUEST_GROUP_CHAT,
	CLIENT_SEND_GROUP_CHAT,
	CLIENT_JOIN_GROUP_CHAT,
	CLIENT_OUT_GROUP_CHAT,
	CLIENT_REMOVE_GROUP_CHAT,

};

class SClientPacket {
public:
	Account *account;
	SOCKET client;
	string username;
};
typedef list<SClientPacket*>::iterator PSIClientPacket;