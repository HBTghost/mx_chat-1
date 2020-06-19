#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include "SDataPacket.h"
class FileTransferManagement
{
public:
	vector<char> file_buff;
	string _fileName;
	uint64_t  _totalSize = 3040;
	uint32_t  _chunkSize = 1024;
	uint64_t _chunkCurrentEpoch;
	uint64_t _chunkEpoch;
	uint32_t _currentSize = 0;
	string _id;
	string _src;
	string _des;

	string _desPart;
	ofstream myfile;


	FileTransferManagement() {
		//myfile.open("debug_out", ios::out | ios::app | ios::binary);
	}

	~FileTransferManagement() {
		myfile.close();
	}
	FileTransferManagement(string file_name, uint32_t total_size, uint32_t chunk_size) {
		_fileName = file_name;
		_totalSize = total_size;
		_chunkSize = chunk_size;
		//calc

		_chunkCurrentEpoch = 0;
		_chunkEpoch = (_totalSize / _chunkSize);
		_chunkEpoch += ((_totalSize % _chunkSize) == 0) ? 0 : 1;
		myfile.open(StringHelper::random_string() + "_c_" + file_name , std::ofstream::binary);
	}
	int AddChunk(SDataPackage* msg) {
		uint32_t num_current_package = msg->GetCurrentPacket();
		_totalSize = msg->GetTotalSize();

		char* buffer = msg->ParseDataFile(_chunkSize);
		int remain_size = _totalSize - _currentSize;
		int byte_read = (remain_size < _chunkSize) ? remain_size : _chunkSize;
		myfile.write(buffer, byte_read); 
		
		/*
		for (int i = 0; i < byte_read; i++) {
			myfile << buffer[i];
		}
		*/
		_currentSize += byte_read;
		if (_currentSize == _totalSize) {
			LOG_INFO("COMPELETED TRANSFER ");
			myfile.close();
		}
		/*
		WCHAR* w_content = StringHelper::wstringToWcharP(content);
		char* data = new char[content.size() * 2 + 2];
		memcpy(data, w_content, content.size() * 2 + 2);

		*/

		return 1;
	}
};

