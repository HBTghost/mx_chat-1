#pragma once
#include "Logger.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "resource.h"
#include <ctime> 
#include <sstream>
#include "Mmsystem.h"

using namespace CPlusPlusLogging;
namespace fs = std::filesystem;
using namespace std;
class Tools
{
public:
	Tools() = default;
	~Tools() = default;

	void PlayGotMessSound() {
		PlaySound(TEXT("./res/uh-oh.wav"), NULL, SND_SYNC);
	}

	std::vector<std::string> split(std::string line, char ch = ',') {
		std::string temp;
		std::vector<std::string> parts;
		std::stringstream wss(line);
		while (getline(wss, temp, ch)) {
			parts.push_back(temp);
		}
		return parts;
	}
	std::string merge(std::vector<std::string> parts, std::string ch = ",") {
		std::string res;
		for (auto x : parts) {
			res += x + ch;
		}
		res.pop_back();
		return res;
	}

	std::string TmToString(tm* ltm) {
		int hour, min, sec, day, month, year;

		hour = ltm->tm_hour + 1;
		min = ltm->tm_min + 1;
		sec = ltm->tm_sec + 1;
		day = ltm->tm_mday;
		month = ltm->tm_mon + 1;
		year = ltm->tm_year + 1900;

		std::string res;
		res += std::to_string(hour) + ":";
		res += std::to_string(min) + ":";
		res += std::to_string(sec) + " ";
		res += std::to_string(day) + "/";
		res += std::to_string(month) + "/";
		res += std::to_string(year);

		return res;
	}

	std::string now() {
		time_t time_now = time(0);
		tm* ltm = new tm;
		localtime_s(ltm, &time_now);
		return TmToString(ltm);
	}

	bool is_exist(const fs::path& p, fs::file_status s = fs::file_status{})
	{
		return fs::status_known(s) ? fs::exists(s) : fs::exists(p);
	}
	void genNeededFolder()
	{
		if (!is_exist("data")) {
			fs::create_directory("data");
		}
		if (!is_exist("data/accounts")) {
			fs::create_directory("data/accounts");
		}
		if (!is_exist("data/messages")) {
			fs::create_directory("data/messages");
		}
	}
	std::string WstringToString(std::wstring  wstr) {
		std::string res;
		for (auto x : wstr) {
			res += x;
		}
		return res;
	}
	std::wstring StringToWstring(std::string str) {
		std::wstring res;
		for (auto x : str) {
			res += x;
		}
		return res;
	}



	std::vector<string> ReadFile(std::string path)
	{
		std::vector<string> rslt;
		std::ifstream in(path);
		if (!in)
		{
			cout << "Cannot open the File : " << path << std::endl;
			LOG_ERROR("Cannot open the File : " + path); 
			return rslt;
		}
		std::string str;
		while (std::getline(in, str))
		{
			if (str.size() > 0)
				rslt.push_back(str);
		}
		in.close();
		return rslt;
	}

	void WriteToFile(std::string path, std::vector<string> messages)
	{
		std::ofstream ofs(path);
		if (ofs.is_open())
		{
			for (string& s : messages) {
				ofs << s << endl;
			}
			ofs.close();
		}
		else {
			LOG_INFO("WriteToFile() - CANNOT OPEN FILE ");
			cout << "Unable to open file" << endl;
		};
	}
};