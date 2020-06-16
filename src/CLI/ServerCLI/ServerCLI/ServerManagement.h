#pragma once
#include "mxdef.h"
#include <iostream>
#include <string>
#include <list>
#include <map>
#include "Conversation.h"
#include <vector>

using namespace std;
class ServerManagement
{
public:
	map<string, Conversation*> _lcs;
	
	ServerManagement() {
	}


	void AddPrivateConversation(SDataPackage* package, list<SClientPacket*>::iterator& c_socket) {
		//not have any sha
		//string hash_des_conservation = ;

		Conversation* con = NULL;
		//PrivateConversation *pcon = new PrivateConversation(serverSocket);
	//	string hash_init = pcon->InitConversation(package, c_socket);
		
	}
};

