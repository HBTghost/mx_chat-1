#include "stdafx.h"
#include "ClientSocket.h"
#include "ClientBackgroundService.h"
#include "PackageHelper.h"
#include "StringHelper.h"


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

void ClientBackgroundService::TestLogin()
{
	MessageModel msg;
	msg.command = EMessageCommand::SIGN_IN;
	msg.num_package = 0;
	//4294967295
	//4 294 967 295
	//
	msg.total_size = 294967295;
	msg.arg.push_back(L"admin");
	msg.arg.push_back(L"pass");
	wstring build_msg = msg.BuildMessage();

	WCHAR* pp = StringHelper::wstringToWcharP(build_msg);
	//MessageModel parsed = PackageHelper::ParseMessage(packet, 4096);
	gClientObj.SendMessageServer(pp , build_msg.size());
}

void ClientBackgroundService::SomeFunction()
{
	cout << "Some func " << endl;
}
