#pragma once
#include <vector>
#include <xstring>
#include <string>
#include <xstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ClientStructure.h"
#define PACKAGE_MAX_SIZE 4096
using namespace std;
class MessageModel {
public:
	EMessageCommand command;
	vector<wstring> arg;
	~MessageModel() {
		arg.clear();
	}
	wstring BuildMessage() {
		wstring buffer_temp;
		buffer_temp.push_back(command);
		buffer_temp.push_back(L'\0');
		for (auto& s : arg) {
			buffer_temp.insert(buffer_temp.end(), s.c_str(), s.c_str() + s.size() );
		}
		//WCHAR* wc = const_cast<wchar_t*>(buffer_temp.c_str());

		return buffer_temp;
	}
};
static class MessageHelper
{
public:
	static MessageModel& ParseString(wstring msg, wchar_t delim = L'\0') {
		//  struct : | COMMAND (1byte) | N | arg1 | N | arg2 | N | ... | argn | N |
		wstring temp;
		vector<wstring> parts;
		wstringstream wss(msg);
		while (std::getline(wss, temp,delim))
			parts.push_back(temp);
		MessageModel message;
		message.command = (EMessageCommand)stoi(parts[0].c_str());
		message.arg = parts;
		return message;
	}
	static MessageModel& ParseMessage(WCHAR* msg,int max_len, wchar_t delim = L'\0') {
		//  struct : | COMMAND (1byte) | N | arg1 | N | arg2 | N | ... | argn | N |
		wstring temp;
		vector<wstring> parts;
		
		for (int i = 0; i < max_len; i++) {
			if (msg[i] == delim) {
				temp.push_back('\0');
				parts.push_back(temp);
				temp = L"";
				continue;
			}
			temp.push_back(msg[i]);
		}

		

		//parts.push_back(L"admin");
		MessageModel * message = new MessageModel();
		message->command = (EMessageCommand)(parts[0].c_str()[0]);
		parts.erase(parts.begin(), parts.begin() + 1);
		message->arg = parts;
		//copy(parts.begin(), parts.end(), back_inserter(message.arg));
		return *message;
	}
	
};

