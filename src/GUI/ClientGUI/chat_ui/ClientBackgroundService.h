#pragma once
#include <windows.h>
#include "mxdef.h"
#include "ClientSocket.h"
#include <chrono>
#include <mutex>
#include <thread>
#include "DebugHelper.h"
#include "FileHelper.h"

class ClientBackgroundService
{
public:
	int InitClient()
	{

		gClientObj.InitClient("127.0.0.1", 8084);
		if (!gClientObj.IsConnected())
		{
			MessageBox(0, L"\nFailed to initialise Client socket.", 0, 0);
			return 0;
		}

		//AfxBeginThread(recMessageThread, this);

		return 1;
	}
	void AddHwnd(HWND hwnd) {

		this->gClientObj.AddHwnd(hwnd);
	}
	friend UINT recMessageThread(LPVOID pParam);
	void CreateWorkerThread();

	void RequestPrivateMessage(string des) {

	}
	
	void OpenDialogSession(SDataPackage* package) {
		string hash_conservation_id = package->_data_items[0];
		bool chatOn = true;
		string message = "";

		while (chatOn == true) {
			cout << username << ">";
			getline(cin, message);
			SendPrivateMessage(hash_conservation_id, message);
			if (message.compare("exit") == 0) {
				chatOn = false;
			}
		}
	}

	void Login(string username, string password) {
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_SIGN_IN)
			->SetHeaderDesSrc("client", "server")
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(username);
		sdata->_data_items.push_back(password);
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
		this->username = username;
		LOG_INFO("Login() send ");
		DebugHelper::DumpArray(__DEBUG_CLIENT_LOGIN_FILE, data, PACKAGE_SIZE);
	}

	void Register(string username, string password) {
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_SIGN_UP)
			->SetHeaderDesSrc("client", "server")
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(username);
		sdata->_data_items.push_back(password);
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
		this->username = username;
		LOG_INFO("Register() send ");
		DebugHelper::DumpArray(__DEBUG_CLIENT_LOGIN_FILE, data, PACKAGE_SIZE);
	}
	

	string InitTransferFile(string hash_conversation, int chunk_size , string path, string file_name ="") {
		wstring w_path = StringHelper::utf8_decode(path);
		uint32_t file_size = FileHelper::FileSize(w_path.c_str());
		if (file_name == "") {
			file_name = path;
		}

		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_REQUEST_TRANSFER_FILE)
			->SetHeaderDesSrc(username, "server")
			->SetHeaderDesSrcHash(SHA256()(username), hash_conversation)
			->SetHeaderNumPackage(chunk_size)
			->SetHeaderTotalSize(file_size);
		sdata->_data_items.push_back(file_name);
		 
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
		TestTransferFile(hash_conversation, chunk_size, path);
		LOG_INFO("Init request file ... ");
		return sdata->GetSHA256Src();
	}
	void TestTransferFile(string hash_conversation, int chunk_size,  string path) {
		wstring w_path = StringHelper::utf8_decode(path);
		uint32_t total_size = FileHelper::FileSize(w_path.c_str());
		ifstream ifs;
		ifs.open(path, std::ios::binary);
		if (ifs.is_open()) {
			LOG_INFO("IFS open ok");

		}
		else {
			LOG_ERROR("ERROR open ifs");
		}
		LOG_INFO("Using chunk size: " + to_string(chunk_size) + "\r\n");

		
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


		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_BEGIN_TRANSFER_FILE)
			->SetHeaderDesSrc(username, "server")
			->SetHeaderDesSrcHash(SHA256()(username), hash_conversation)
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(total_size);
		

		/* the loop of chunking */
		for (size_t chunk = 0; chunk < total_chunks; ++chunk)
		{
			sdata->_data_items.clear();
			sdata->SetHeaderNumPackage(chunk);

			size_t this_chunk_size =
				chunk == total_chunks - 1 /* if last chunk */
				? last_chunk_size /* then fill chunk with remaining bytes */
				: chunk_size; /* else fill entire chunk */

			  /* if needed, we also have the position of this chunk in the file
				 size_t start_of_chunk = chunk * chunk_size; */

				 /* adapt this portion as necessary, this is the fast C++ way */
			vector<char> chunk_data(this_chunk_size);
			ifs.read(&chunk_data[0], /* address of buffer start */
				this_chunk_size); /* this many bytes is to be read */
			char* c_chunk_data = chunk_data.data();

			/* do something with chunk_data before next iteration */
			LOG_INFO("#chunk " + to_string(chunk) + "\r\n");
			
			 
			//copy(chunk_data.begin(), chunk_data.end(), back_inserter(sdata->))
			//sdata->_data_items.push_back(c_chunk_data); 
			char* msg = sdata->BuildMessageDataFileSpecial(c_chunk_data, chunk_size);
			
			gClientObj.SendMessagePackage(msg, PACKAGE_SIZE);
		}

	}
	
	void CreateGroupConversation(vector<string> list_member, string name_conversation) {
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_REQUEST_GROUP_CHAT)
			->SetHeaderDesSrc(this->username, "")
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(name_conversation);
		copy(list_member.begin(), list_member.end(), back_inserter(sdata->_data_items));
		
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
	 	LOG_INFO("CreatePrivateConversation() : Sent request ");
		//DebugHelper::DumpArray(__DEBUG_CLIENT_LOGIN_FILE, data, PACKAGE_SIZE);
	}

	void CreatePrivateConversation(string des_username, string name) {
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_REQUEST_PRIVATE_CHAT)
			->SetHeaderDesSrc("client", "server")
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(des_username);
		sdata->_data_items.push_back(name);
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
		LOG_INFO("CreateGroupConversation) : Sent request ");
	}
	void SendPrivateMessage(string hash_des, string message) {
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_SEND_PRIVATE_CHAT)
			->SetHeaderDesSrc(username, "server")
			->SetHeaderDesSrcHash(SHA256()(username), hash_des)
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(message);
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
	}
	void SendGroupMessage(string hash_des, string message) {
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::CLIENT_SEND_GROUP_CHAT)
			->SetHeaderDesSrc(username, "server")
			->SetHeaderDesSrcHash(SHA256()(username), hash_des)
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(message);
		char* data = sdata->BuildMessage();
		gClientObj.SendMessagePackage(data, PACKAGE_SIZE);
	}


	void CloseConnection() {
		gClientObj.CloseConnection();
	}
	void Shutdown() {
		gClientObj.Shutdown();
	}
	string username = "";

	ClientSocket gClientObj;

private:
};