#pragma once
#include "Conversation.h"
class PrivateConversation :
    public Conversation
{
public:
    PrivateConversation(ServerSocket *server) : Conversation(server) {

    }
    void TransferMessage(SDataPackage* package) override {
      
    }
    
    ~PrivateConversation() {

    }
};

