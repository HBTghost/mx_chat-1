#pragma once
#include "Conversation.h"
#include <iostream>
#include <vector>
#include <string>
#include "mxdef.h"
using namespace std; 
class GroupConversation :
    public Conversation
{
public:
 
    GroupConversation(ServerSocket* server) : Conversation(server) {
        
    }
    ~GroupConversation() {

    }

};