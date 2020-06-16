#pragma once
#include "mxdef.h"
#include "Conversation.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std; 
class GroupConversation : public Conversation
{
public:
    GroupConversation() : Conversation() {
        
    }
    ~GroupConversation() {

    }
};