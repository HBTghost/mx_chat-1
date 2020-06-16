#include "pch.h"
#include "ClientSocket.h"
#include "ClientBackgroundService.h"

UINT recMessageThread(LPVOID lParam)
{
	ClientBackgroundService* pMyClass = reinterpret_cast<ClientBackgroundService*> (lParam);

	while (1)
	{
		if (pMyClass->gClientObj.RecvMessage())
			break;
	}
	return 0;
}

void ClientBackgroundService::CreateWorkerThread()
{
	CWinThread* pWinThread = AfxBeginThread(recMessageThread, this, 0U, CREATE_SUSPENDED);

	if (pWinThread != NULL)
	{
		/* Thread started in SUSPENDED mode. Resume the thread. */
		pWinThread->ResumeThread();
	}
}
