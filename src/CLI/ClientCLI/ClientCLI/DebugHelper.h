#pragma once
#include <iostream>
#include <string>
#include <fstream> 
#include <vector>
#include <chrono>
#include <cstdint>
#include <algorithm>



using namespace std;
static class DebugHelper
{
public:
	static void DumpArray(const char* filename, char* buff, int n) {
		std::vector<char> data;
		const char* values = buff;
		const char* end = values + n;
		data.insert(data.end(), values, end);
		auto startTime = std::chrono::high_resolution_clock::now();
		auto myfile = std::fstream(filename, std::ios::out | std::ios::binary);
		myfile.write((char*)&data[0], n);
		myfile.close();
		auto endTime = std::chrono::high_resolution_clock::now();
		cout << "Dumped " << filename  <<" time write : " <<  std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << endl;
	}
};

