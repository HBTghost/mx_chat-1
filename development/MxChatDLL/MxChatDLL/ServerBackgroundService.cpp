#include "stdafx.h"
#include "ServerSocket.h"
#include "ServerBackgroundService.h"

ServerSocket gServerObj;

UINT  recServerThread(LPVOID lParam)
{
	SOCKET recSocket = (SOCKET)lParam;
	int activity = 0;
	while (1)
	{
	
		if (gServerObj.ReceivePackageClient(recSocket)) {
			break;
		}
	}
	return 0;
}

UINT  listenServerThread(LPVOID lParam)
{
	while (1)
		gServerObj.BeginListenClient();
	return 0;
}