#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include "PackageHelper.h"
using namespace std;
class FileTransferManagment
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
	                                                                                                                                                                                                  
	FileTransferManagment() {
		myfile.open("debug_out", ios::out | ios::app | ios::binary);
	}

	FileTransferManagment(string file_path){
		
	}
	~FileTransferManagment(){
		myfile.close();
	}
	FileTransferManagment(string file_name, uint64_t total_size, uint64_t chunk_size) {
		_fileName = file_name;
		_totalSize = total_size;
		_chunkSize = chunk_size;
		//calc
		
		_chunkCurrentEpoch = 0;
		_chunkEpoch = (_totalSize / _chunkSize);
		_chunkEpoch += ((_totalSize % _chunkSize) ==0) ? 0 :1;
	}
	int AddChunk(MessageModel& msg, WCHAR *temp ) {
		uint32_t num_current_package = msg.num_package;
		_totalSize  = msg.total_size;
		wstring id = msg.arg[0];
	
		
		char* buffer = PackageHelper::ParseGetDataRegion(temp, 4096,  '\0' );
		int remain_size = _totalSize - _currentSize;
		int byte_read = (remain_size < _chunkSize) ? remain_size : _chunkSize;
		for (int i = 0; i < byte_read; i++) {
			myfile << buffer[i];
		}
		_currentSize += byte_read;
		if (_currentSize == _totalSize) {
			wcout << "[END WRITE]" << endl;
			myfile.close();
		}
		/*
		WCHAR* w_content = StringHelper::wstringToWcharP(content);
		char* data = new char[content.size() * 2 + 2];
		memcpy(data, w_content, content.size() * 2 + 2);
		
		*/
			
		return 1;
	}
private:
};
