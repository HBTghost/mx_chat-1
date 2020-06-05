#pragma once
#include <vector>
#include <xstring>
#include <string>
#include <xstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include "MxObject.h"
#include <wtypes.h>
#include "StringHelper.h"
#define PACKAGE_MAX_SIZE 4096
using namespace std;
class MessageModel {
public:
	EMessageCommand command;
	vector<wstring> arg;
	uint32_t total_size = 0;
	uint32_t num_package = 0;
	uint32_t header_len = 2;

	~MessageModel() {
		arg.clear();
	}
	int HeaderLengthFileStruct() {
		wstring buffer_temp;
		header_len = 2;

		wchar_t sn_buffer[_MAX_ITOSTR_BASE10_COUNT];

		_itow(total_size, sn_buffer, 10);
		for (int i = 0; i < _MAX_ITOSTR_BASE10_COUNT; i++) {
			header_len++;
			if (sn_buffer[i] == '\0') { break; }
			buffer_temp.push_back(sn_buffer[i]);
		}

		_itow(num_package, sn_buffer, 10);
		for (int i = 0; i < _MAX_ITOSTR_BASE10_COUNT; i++) {
			header_len++;
			if (sn_buffer[i] == '\0') { break; }
			buffer_temp.push_back(sn_buffer[i]);
		}
		if(arg.size() > 0){ 
			header_len += arg[0].length() + 1;
		}
		return header_len;
	}
	WCHAR* BuildBlockMessage() {
		
		WCHAR* pp = StringHelper::wstringToWcharFixedP(this->BuildMessage(), 4096);
		return pp;
	}
	
	wstring BuildMessage() {
		wstring buffer_temp;
		buffer_temp.push_back(command);
		buffer_temp.push_back(L'\0');

		wchar_t sn_buffer[_MAX_ITOSTR_BASE10_COUNT];

		_itow(total_size, sn_buffer, 10);
		for (int i = 0; i < _MAX_ITOSTR_BASE10_COUNT; i++) {
			if (sn_buffer[i] == '\0') { break; }
			buffer_temp.push_back(sn_buffer[i]);
		}
		buffer_temp.push_back(L'\0');

		_itow(num_package, sn_buffer, 10);
		for (int i = 0; i < _MAX_ITOSTR_BASE10_COUNT; i++) {
			if (sn_buffer[i] == '\0') { break; }

			buffer_temp.push_back(sn_buffer[i]);
		}
		buffer_temp.push_back(L'\0');



		for (auto& s : arg) {

			buffer_temp.insert(buffer_temp.end(), s.c_str(), s.c_str() + s.size() + 1);
		}
		//WCHAR* wc = const_cast<wchar_t*>(buffer_temp.c_str());

		return buffer_temp;
	}
};
static class PackageHelper
{
public:
	static MessageModel& ParseString(wstring msg, wchar_t delim = L'\0') {
		//  struct : | COMMAND (1byte) | N | arg1 | N | arg2 | N | ... | argn | N |
		wstring temp;
		vector<wstring> parts;
		wstringstream wss(msg);
		while (std::getline(wss, temp, delim))
			parts.push_back(temp);
		MessageModel message;
		message.command = (EMessageCommand)stoi(parts[0].c_str());
		message.arg = parts;
		return message;
	}
	static char* ParseGetDataRegion(WCHAR* msg, int max_len, wchar_t delim = '\0') {
		wstring temp;
		vector<wstring> parts;
		int i;
		for (i = 0; i < max_len; i++) {
			if (msg[i] == delim) {
				temp.push_back('\0');
				parts.push_back(temp);
				if (parts.size() == 4) {
					//commamnd | total | num pack | id 
					break;
				}
				temp = L"";
				continue;
			}
			temp.push_back(msg[i]);
		}
		int package_header_len = i + 1;
		int data_size = max_len - package_header_len;
		temp = L"";
		for (i = package_header_len; i < max_len; i++) {
			temp.push_back(msg[i]);
		}
		WCHAR* data = StringHelper::wstringToWcharP(temp);
		
		char* c_data = new char[data_size * 2 ];
		memcpy(c_data, data, data_size*2);
		

		return c_data;
	}

	static MessageModel& ParseMessage(WCHAR* msg, int max_len, wchar_t delim = L'\0') {
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
		MessageModel* message = new MessageModel();
		message->command = (EMessageCommand)(parts[0].c_str()[0]);
		message->total_size = _wtoi(parts[1].c_str());
		message->num_package = _wtoi(parts[2].c_str());
		parts.erase(parts.begin(), parts.begin() + 3);
		message->arg = parts;
		//copy(parts.begin(), parts.end(), back_inserter(message.arg));
		return *message;
	}
};