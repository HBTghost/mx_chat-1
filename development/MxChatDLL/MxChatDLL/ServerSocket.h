#pragma once
#define _AFXDLL
#include "MxObject.h"
#include "PackageHelper.h"
#include "EventOnMessageHandler.h"
#include <mutex>
#include "SClientPackage.h"

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
	void NotifyListUserOnline();
	void ProcessMessage(MessageModel& package_msg, list<SClientPacket*>::iterator &c_socket);
	SClientPacket* GetClientByUsername(wstring username);
	vector<wstring> ListUserOnline();
	int _total_msg = 0;
	int activity = 0;
	int client_socket[30];
private:
	std::mutex mtx;           // mutex for critical section
	fd_set fdread;
	HWND _hwnd;
	CWnd* _cWnd;
	SOCKET _socketClient;
	SOCKET _socketListenClient;
	bool _isConnected;
	list<SClientPacket*> _listClient;
	
};