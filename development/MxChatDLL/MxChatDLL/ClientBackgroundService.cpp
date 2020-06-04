#include "stdafx.h"
#include "ClientSocket.h"
#include "ClientBackgroundService.h"
#include "FileHelper.h"



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
	msg.total_size = 4096;
	msg.arg.push_back(user);
	msg.arg.push_back(pass);
	WCHAR * build_msg = msg.BuildBlockMessage();

	gClientObj.SendMessageServer(build_msg, 4096);
}

void ClientBackgroundService::TestSignUp(wstring user, wstring pass) {
	MessageModel msg;
	msg.command = EMessageCommand::CLIENT_SIGN_UP;
	msg.arg.push_back(user);
	msg.arg.push_back(pass);
	WCHAR* build_msg = msg.BuildBlockMessage();
	gClientObj.SendMessageServer(build_msg, 4096);

}
void ClientBackgroundService::TestTransferFile(wstring user) {
	/*
	MessageModel msg;
	msg.command = EMessageCommand::CLIENT_BEGIN_TRANSFER_FILE;
	msg.total_size = 2048;
	msg.num_package = 0;
	msg.arg.push_back(L"");
	uint64_t filesize = FileHelper::FileSize(L"3k.txt");//3k.txt 3040  // 27k.txt 27378
	bool end_transfer = false;




	WCHAR* build_msg = msg.BuildBlockMessage();
	gClientObj.SendMessageServer(build_msg, 4096);
	while (end_transfer == false) {

	}
	*/
	FileTransferManagment ftm;
	ofstream myfile;
	myfile.open("3k_out", ios::out | ios::app | ios::binary);

	int total_size = FileHelper::FileSize(L"3k.txt");
	int chunk_size = 1024;
	ifstream file("3k.txt");
	cout << "using chunk size: " << chunk_size << endl;

	/* on to the actual algorithm */
	size_t total_chunks = total_size / chunk_size;
	size_t last_chunk_size = total_size % chunk_size;

	if (last_chunk_size != 0) /* if the above division was uneven */
	{
		++total_chunks; /* add an unfilled final chunk */
	}
	else /* if division was even, last chunk is full */
	{
		last_chunk_size = chunk_size;
	}
	MessageModel msg;
	msg.command = EMessageCommand::CLIENT_BEGIN_TRANSFER_FILE;
	msg.total_size = total_size;

	/* the loop of chunking */
	for (size_t chunk = 0; chunk < total_chunks; ++chunk)
	{
		msg.arg.clear();
		msg.arg.push_back(L"123");
		msg.num_package = chunk;
		

		size_t this_chunk_size =
			chunk == total_chunks - 1 /* if last chunk */
			? last_chunk_size /* then fill chunk with remaining bytes */
			: chunk_size; /* else fill entire chunk */

		  /* if needed, we also have the position of this chunk in the file
			 size_t start_of_chunk = chunk * chunk_size; */

			 /* adapt this portion as necessary, this is the fast C++ way */
		vector<char> chunk_data(this_chunk_size);
		file.read(&chunk_data[0], /* address of buffer start */
			this_chunk_size); /* this many bytes is to be read */
		char* c_chunk_data = chunk_data.data();

  /* do something with chunk_data before next iteration */
		cout << "chunk #" << chunk << endl;
		WCHAR* w_chunk_data = new WCHAR[chunk_data.size() / 2];
		int w_data_size = chunk_data.size() / 2;
		memcpy(w_chunk_data, c_chunk_data, w_data_size);
		msg.arg.push_back(w_chunk_data);

		wstring build_msg = msg.BuildMessage();
		WCHAR* pp = StringHelper::wstringToWcharFixedP(build_msg, 4096);
		ftm.AddChunk(msg, pp);
		
		//gClientObj.SendMessageServer(pp, 4096);
		/*
		for (const auto c : chunk_data) 
		{
			cout << c;
			myfile << c; 
		}
		*/
	}
	
}
  void ClientBackgroundService::TestPrivateMessage(wstring des, wstring msg)
{
	MessageModel m_msgModel;
	m_msgModel.command = EMessageCommand::CLIENT_PRIVATE_MSG; 
	m_msgModel.arg.push_back(des);
	m_msgModel.arg.push_back(msg);
	WCHAR* build_msg = m_msgModel.BuildBlockMessage();
	gClientObj.SendMessageServer(build_msg, 4096);
}

void ClientBackgroundService::TestGroupMessage(wstring group_id, wstring msg)
{
}

void ClientBackgroundService::SomeFunction()
{
	wcout << "Some func " << endl;
}

