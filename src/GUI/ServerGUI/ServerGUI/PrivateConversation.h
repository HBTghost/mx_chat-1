#pragma once
#include "Conversation.h"
class PrivateConversation :
	public Conversation
{
public:
	 
	PrivateConversation() : Conversation() {

	}
	void AddMemberClient(PSIClientPacket& client) {
		_list_client.push_back((*client));
	}


	void TransferMessage(SDataPackage* package) override {

	}

	~PrivateConversation() {

	}

	// Inherited via Conversation
	virtual char* BuildNewHashMsg() override {
		if (_list_client.size() != 2) {
			LOG_ERROR("TransferCreateHash - num clients == 2");
			return nullptr;
		}
		id_key = _list_client[0]->username + _list_client[1]->username;
		this->id_hash = sha256(id_key);

		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::SERVER_RESPONSE_HASH_KEY)
			->SetHeaderDesSrc("server", "client")
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(id_hash);
		sdata->_data_items.push_back("PRIVATE");

		for (SClientPacket*& p_client : _list_client) {
			sdata->_data_items.push_back(p_client->username);
		}

		char* msg = sdata->BuildMessage();
		return msg;

	}
};