#pragma once
#ifndef MxObject_H
#define MxObject_H
#include <afxwin.h>
#include <WinSock2.h>
#include <list>
#include <vector>
#include <iostream>
using namespace std;



enum EMessageCommand {
	CLIENT_SIGN_UP = 1,
	SERVER_SIGN_UP_ERROR_USER,
	SERVER_SIGN_UP_SUCCESS,

	CLIENT_SIGN_IN,
	SERVER_SIGN_IN_ERROR_PASS,
	SERVER_SIGN_IN_SUCCESS,
	
	CLIENT_LOG_OUT,
	NOTIFY_CLIENT_LOG_OUT,
	SERVER_LOG_OUT,
	NOTIFY_SERVER_LOG_OUT, 

	CLIENT_PRIVATE_MSG,
	CLIENT_GROUP_MSG,
	
	CLIENT_REQUEST_TRANSFER_FILE,
	CLIENT_END_TRANSFER_FILE,

	NOTIFY_LIST_USER_ONLINE,
};

#endif