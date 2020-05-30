#pragma once
#define _AFXDLL
#include "ServerStructure.h"
class ServerSocket
{
public:
	ServerSocket();
	~ServerSocket();
	bool IsConnected();
	void BeginListenClient();
	void SendPackageClient(SClientPacket *packet, WCHAR*msg, int len);
	int ReceivePackageClient(SOCKET recvSocket);
	void setHWND(HWND hwnd);
	HWND getHWND();
	void setCWND(CWnd *&cwnd);
	CWnd*& getCWND();
private:
	HWND _hwnd;
	CWnd *_cWnd;
	SOCKET _socketClient;
	SOCKET _socketListenClient;
	bool _isConnected;
	vector<SClientPacket*> _listClient;
};