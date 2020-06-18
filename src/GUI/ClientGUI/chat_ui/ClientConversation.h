#pragma once
#include <string>
#include <iostream>
#include <vector>
using namespace std;
class ClientConversation
{
public:
	string display_name = "";
	string hash_id = "";
	std::vector<std::string> list_online; 
	std::vector<std::string> list_member;
	std::vector<std::string> list_mess;
	bool _is_group_msg = false;
	ClientConversation(string display_name, string hash_id, bool isGroup = false) {
		this->display_name = display_name;
		this->hash_id = hash_id;
		this->_is_group_msg = isGroup;
	}
	

	~ClientConversation() {

	}
};

