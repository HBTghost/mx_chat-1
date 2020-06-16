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
};