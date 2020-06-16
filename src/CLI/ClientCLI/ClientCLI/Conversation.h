#pragma onceServerSocket
#include <string>
#include "ServerSocket.h"
using namespace std;
class Conversation
{
public:
	vector<SClientPacket*> _list_client;
	ServerSocket* _server;
	string id_hash;
	string id_key;
	string user_created;
	Conversation(ServerSocket* server) {
		this->_server = server;
	}

	virtual void TransferCreateHash() {
		id_key = user_created + StringHelper::random_string();
		this->id_hash = sha256(id_key);
		if (_list_client.size() < 2) {
			LOG_ERROR("TransferCreateHash - num clients < 2");
			return;
		}
		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::SERVER_RESPONSE_HASH_KEY)
			->SetHeaderDesSrc("server", "client")
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(id_hash);

		for (SClientPacket*& p_client : _list_client) {
			sdata->_data_items.push_back(p_client->username);
		}

		char* msg = sdata->BuildMessage();
		for (SClientPacket*& p_client : _list_client) {
			_server->SendMessagePackage(p_client, msg, PACKAGE_SIZE);
		}
	}
	virtual void TransferMessage(SDataPackage* package) {
		//private use raw use and destination
		//User send message not need to receive this message, otherwise send to other clients in list
		char* msg = package->data();
		stringstream ss;
		ss << "[TransferMessage] + 1 message from " << package->GetSrc() << " to " << package->GetDes() << endl;

		for (SClientPacket*& p_client : _list_client) {
			if (p_client->username == package->GetSrc()) {
				continue;
			}
			else {
				_server->SendMessagePackage(p_client, msg, PACKAGE_SIZE);
				string log_msg = ss.str();
				LOG_INFO(log_msg);
			}
		}
	}

};