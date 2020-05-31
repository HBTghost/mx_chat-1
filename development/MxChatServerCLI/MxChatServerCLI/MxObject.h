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
	SIGN_UP = 5,
	SIGN_IN = 8
};

#endif