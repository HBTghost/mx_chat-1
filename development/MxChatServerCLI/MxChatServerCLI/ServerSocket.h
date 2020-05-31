#pragma once
#define _AFXDLL
#include "MxObject.h"
#include "PackageHelper.h"

#include "EventOnMessageHandler.h"

class ServerSocket : public EventOnMessageHandler
{
public:
	ServerSocket();
	~ServerSocket();
	bool IsConnected();
	void BeginListenClient();
	int SendPackageClient(SClientPacket* packet, WCHAR* msg, int len);
	int SendPackageClientAll(WCHAR* msg, int len);
	int ReceivePackageClient(SOCKET recvSocket);
	void ProcessMessage(MessageModel& package_msg);
	
private:
	HWND _hwnd;
	CWnd* _cWnd;
	SOCKET _socketClient;
	SOCKET _socketListenClient;
	bool _isConnected;
	list<SClientPacket*> _listClient;
};