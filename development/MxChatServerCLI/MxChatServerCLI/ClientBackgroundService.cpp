#include "stdafx.h"
#include "ClientSocket.h"
#include "ClientBackgroundService.h"

ClientSocket gClientObj;

UINT  recMessageThread(LPVOID lParam)
{
	while (1)
	{
		if (gClientObj.RecvMessageServer())
			break;
	}
	return 0;
}