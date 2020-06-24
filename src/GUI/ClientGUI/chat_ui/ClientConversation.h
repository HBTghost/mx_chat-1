#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "FileTransferManagement.h"
using namespace std;
class ClientConversation
{
public:
	string display_name = "";
	string hash_id = "";
	std::vector<std::string> list_online; 
	std::vector<std::string> list_member;
	std::vector<std::string> list_mess;
	FileTransferManagement* ftm;
	int pending_msg = 0;
	bool _is_group_msg = false;

	bool transfering = false;
	

	ClientConversation(string display_name, string hash_id, bool isGroup = false) {
		this->display_name = display_name;
		this->hash_id = hash_id;
		this->_is_group_msg = isGroup;
	}
	
	void InitFileTransferManagement(string file_name, int total_size, int chunk_size) {
		ftm = new FileTransferManagement(file_name, total_size, chunk_size);
	}
	void ProcessChunk(SDataPackage *model){
		ftm->AddChunk(model);
		if (ftm->completed == true) {
			transfering = false;
			delete ftm;
		}
	}
	


	~ClientConversation() {

	}
};

