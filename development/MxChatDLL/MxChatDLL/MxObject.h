#pragma once
#ifndef MxObject_H
#define MxObject_H
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
	CLIENT_SIGN_UP = 1,
	SERVER_SIGN_UP_ERROR_USER,
	SERVER_SIGN_UP_SUCCESS,

	CLIENT_SIGN_IN,
	SERVER_SIGN_IN_ERROR_PASS,
	SERVER_SIGN_IN_SUCCESS,
	
	CLIENT_PRIVATE_MSG,
	CLIENT_GROUP_MSG,
	
	CLIENT_REQUEST_TRANSFER_FILE,
	CLIENT_END_TRANSFER_FILE

};

#endif