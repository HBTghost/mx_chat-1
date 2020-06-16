#pragma once
#include "mxdef.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include "StringHelper.h"
#include "sha256.h"
#include "DataHelper.h"

class SDataPackage {
private:
	char _package[PACKAGE_SIZE]{ 0 };
	bool _use_raw_user = false;


	uint32_t total_size = PACKAGE_SIZE;
	uint32_t current_packet = 0;

	string raw_src;
	string sha256_hash_src;
	string raw_des;
	string sha256_hash_des;

	uint32_t num_data_item = 0;
	char delim = '\0';

public:
	//|COMMAND|TOTAL_SIZE|CURRENT_PACKET|src|des| flag raw|raw src| raw des | num_data_item|delim| ..\0... | <data>|
	//	 4		4			4			 64 64		 1		 11		11			  4			1		88				<>
	uint32_t command;
	vector<string> _data_items;
	SDataPackage() {


	}

	SDataPackage* SetHeaderCommand(EMessageCommand type) {
		this->command = type;
		return this;
	}
	SDataPackage* SetHeaderDesSrcHash(string hash_src, string hash_des) {
		if (hash_src.length() != 64 || hash_des.length() != 64) {
			throw "Hash SHA256 must contain 64 characters";
		}
		this->sha256_hash_src = hash_src;
		this->sha256_hash_des = hash_des;
		return this;
	}
	SDataPackage* SetHeaderDesSrc(string src, string des) {
		if (src.length() > 11 || des.length() > 11) {
			throw "SRC and Des must be < 11 characters";
		}
		this->_use_raw_user = true;
		this->raw_src = src;
		this->raw_des = des;
		return this;
	}
	SDataPackage* SetHeaderTotalSize(uint32_t total_size) {
		this->total_size = total_size;
		return this;
	}
	SDataPackage* SetHeaderNumPackage(uint32_t current_package) {
		this->current_packet = current_package;
		return this;
	}

	SDataPackage(char* package) {
		for (int i = 0; i < PACKAGE_SIZE; i++) {
			_package[i] = package[i];
		}

		this->ParseHeader();
		this->ParseData();
	}
	~SDataPackage() {
	}

	void DebugPackage() {
		std::ostringstream ss;
		ss << endl << "\t[CUR PAK] " << this->current_packet << endl;
		ss << "\t[SHA DES] " << this->sha256_hash_des << endl;
		ss << "\t[SHA SRC] " << this->sha256_hash_src << endl;
		ss << "\t[Raw DES] " << this->raw_src << endl;
		ss << "\t[Raw SRC] " << this->raw_des << endl;
		ss << "\t[Num DAT] " << this->num_data_item << endl;
		//copy(_data_items.begin(), _data_items.end(), ostream_iterator<int>(ss, ","));
		string debug = ss.str();
		LOG_DEBUG(debug);
	}

#pragma region PARSE_PACKAGE_FROM_BUFFEr

	void ParseHeader() {
		char temp_raw_src_des[12]{ 0 }; // \0
		char temp_sha[65]{ 0 };
		unsigned char temp_num[4]{ 0 };
		memcpy(temp_num, _package, sizeof(uint32_t));
		command = DataHelper::ConvertBitUint32(temp_num);

		memcpy(temp_num, _package + 4, sizeof(uint32_t));
		total_size = DataHelper::ConvertBitUint32(temp_num);

		memcpy(temp_num, _package + 8, sizeof(uint32_t));
		current_packet = DataHelper::ConvertBitUint32(temp_num);

		memcpy(temp_sha, _package + 12, 64);
		sha256_hash_src = string(temp_sha);

		memcpy(temp_sha, _package + 76, 64);
		sha256_hash_des = string(temp_sha);

		if (_package[140] == 0x01) {
			//true
			_use_raw_user = true;
			memcpy(temp_raw_src_des, _package + 141, 11);
			raw_src = string(temp_raw_src_des);

			memcpy(temp_raw_src_des, _package + 152, 11);
			raw_des = string(temp_raw_src_des);
		}
		else {
			_use_raw_user = false;
		}
		memcpy(temp_num, _package + 162, sizeof(uint32_t));
		num_data_item = DataHelper::ConvertBitUint32(temp_num);

		delim = _package[166];
		_package[166] = delim;


	}
	void ParseData() {
		int pdr = PACKAGE_HEADER_SIZE;
		int current_data_pos = pdr;
		string temp = "";
		for (int i = current_data_pos; i < PACKAGE_SIZE; i++) {
			if (_package[i] == delim) {
				if (temp.size() == 0) {
					continue;
				}
				else {
					_data_items.push_back(temp);
					temp = "";
					continue;
				}
			}
			temp.push_back(_package[i]);
		}
	}

#pragma endregion

#pragma region PACKAGE_BUILDER

	char* BuildMessage() {
		memset(_package, 0, sizeof(_package));
		this->BuildHeader();
		this->BuildData();
		return _package;
	}
	void BuildHeader() {
		num_data_item = _data_items.size();
		//|COMMAND|TOTAL_SIZE|CURRENT_PACKET|src|des| flag raw|raw src| raw des | num_data_item|delim| ..\0... | <data>|
		unsigned char temp_num[4]{ 0 };
		DataHelper::ConvertUint32Bit(temp_num, command);
		memcpy(_package, temp_num, sizeof(uint32_t));
		DataHelper::ConvertUint32Bit(temp_num, total_size);
		memcpy(_package + 4, temp_num, sizeof(uint32_t));
		DataHelper::ConvertUint32Bit(temp_num, current_packet);
		memcpy(_package + 8, temp_num, sizeof(uint32_t));


		//sha256_hash_src = sha256(raw_src);
		if (sha256_hash_src.length() > 0) {
			memcpy(_package + 12, sha256_hash_src.data(), 64);
		}
		if (sha256_hash_des.length() > 0) {

			//sha256_hash_des = sha256(raw_des);
			memcpy(_package + 76, sha256_hash_des.data(), 64);
		}

		if (_use_raw_user == true) {
			_package[140] = 0x01;
			memcpy(_package + 141, raw_src.data(), raw_src.length());
			memcpy(_package + 152, raw_des.data(), raw_des.length());
		}
		else {
			_package[140] = 0x00;
		}

		DataHelper::ConvertUint32Bit(temp_num, num_data_item);
		memcpy(_package + 162, temp_num, sizeof(uint32_t));
		_package[166] = delim;
	}
	void BuildData() {
		int pdr = PACKAGE_HEADER_SIZE; //position start of data region
		int current_data_pos = pdr;
		for (string& item : _data_items) {
			const char* temp = item.c_str();
			memcpy(_package + current_data_pos, temp, item.length());
			*(_package + current_data_pos + item.length()) = delim;
			current_data_pos += item.length() + 1;
		}
	}
#pragma endregion

#pragma region GET_SET_FIELDS


	string& GetSHA256Src() {
		return this->sha256_hash_src;
	}
	string& GetSHA256Des() {
		return this->sha256_hash_des;
	}
	string& GetSrc() {
		return this->raw_src;
	}
	string& GetDes() {
		return this->raw_des;
	}

#pragma endregion

	char* data() {
		return _package;
	}

};