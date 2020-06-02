#include "stdafx.h"
#include "ClientSocket.h"
#include "ClientBackgroundService.h"



UINT recMessageThread(LPVOID lParam)
{
	ClientBackgroundService* pMyClass = reinterpret_cast<ClientBackgroundService*> (lParam);

	while (1)
	{
		if (pMyClass->gClientObj.RecvMessageServer())
			break;
	}
	return 0;
}

int ClientBackgroundService::InitClient()
{

	gClientObj.Init(L"127.0.0.1", 8084);
	if (!gClientObj.IsConnected())
	{
		MessageBox(0, L"\nFailed to initialise server socket.", 0, 0);
		return 0;
	}

	//AfxBeginThread(recMessageThread, this);

	return 1;
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

void ClientBackgroundService::TestLogin(wstring user, wstring pass)
{
	MessageModel msg;
	msg.command = EMessageCommand::CLIENT_SIGN_IN;
	msg.num_package = 0;
	//4294967295
	//4 294 967 295
	//
	msg.total_size = 4096;
	msg.arg.push_back(user);
	msg.arg.push_back(pass);
	WCHAR * build_msg = msg.BuildBlockMessage();

	gClientObj.SendMessageServer(build_msg, 4096);
}

void ClientBackgroundService::SomeFunction()
{
	cout << "Some func " << endl;
}

