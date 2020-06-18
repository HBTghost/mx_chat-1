#pragma once
#include "mxdef.h"
#include "Conversation.h"
#include <iostream>
#include <vector>
#include <string>
#include "sha256.h"
#include "framework.h"

using namespace std; 
class GroupConversation : public Conversation
{
public:
	vector<string> _list_mem;
    GroupConversation(string group_name, vector<string> list_mem) : Conversation() {
		this->conversation_name = group_name;
		this->_list_mem = list_mem;
	}
    ~GroupConversation() {

    }

    // Inherited via Conversation
    virtual char* BuildNewHashMsg() override {
		if (_list_client.size() < 2) {
			LOG_ERROR("TransferCreateHash - num clients < 2");
			return nullptr;
		}
		id_key = _list_client[0]->username + StringHelper::random_string();
		this->id_hash = SHA256()(id_key);

		SDataPackage* sdata = (new SDataPackage())
			->SetHeaderCommand(EMessageCommand::SERVER_RESPONSE_HASH_KEY)
			->SetHeaderDesSrc("server", "client")
			->SetHeaderNumPackage(0)
			->SetHeaderTotalSize(4096);
		sdata->_data_items.push_back(id_hash);// 0
		sdata->_data_items.push_back("GROUP"); // 1
		sdata->_data_items.push_back(conversation_name != ""  ? conversation_name : id_hash); //2
		copy(this->_list_mem.begin(), _list_mem.end(), back_inserter(sdata->_data_items));


		//for (SClientPacket*& p_client : _list_client) {
		//	sdata->_data_items.push_back(p_client->username);
		//}

		char* msg = sdata->BuildMessage();
		return msg;
    }
};